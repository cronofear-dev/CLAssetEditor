// MIT License


#include "CLAssetEditorUtility.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "Dom/JsonObject.h"
#include "Objects/CLWidgetContainerAsset.h"
#include "Libraries/CLAssetEditorLibrary.h"
#include "Widgets/CLWidgetContainerAssetEditorToolkit.h"

void UCLAssetEditorUtility::OpenEditor(bool bLoadValidLayout)
{
	// Get only the valid tab definitions
	const TSet<FCLTabDefinition>& validTabDefinitions = GetValidTabDefinitions();
	if(validTabDefinitions.Num() == 0 || validTabDefinitions.Num() != TabDefinitions.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("No valid tabs in (%s). Please make sure that all the tabs have a valid TabID and EditorUtilityWidgetBlueprint"), *GetPathName());
	}
	WidgetContainerAsset->TabDefinitions = validTabDefinitions;
	WidgetContainerAsset->LayoutName = GetLayoutName();
	
	// Load most valid layout first
	if(bLoadValidLayout)
	{
		LoadValidLayout();
	}
	
	SpawnedWidgets.Empty();
	
	// Get UAssetEditorSubsystem
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	// Pass params to WidgetContainerAsset
	WidgetContainerAsset->AssetEditorUtilityOwner = this; //StaticClass()->GetDefaultObject<UCLAssetEditorUtility>();
	WidgetContainerAsset->MainTabName = MainTabName;

	// Reopen editor for WidgetContainerAsset
	AssetEditorSubsystem->CloseAllEditorsForAsset(WidgetContainerAsset);
	AssetEditorSubsystem->OpenEditorForAsset(WidgetContainerAsset->GetPathName());

	// Get widgets references created in OpenEditorForAsset (This has the disadvantage of not allowing duplicate EditorUtilityWidgetBlueprint)
	// Can be fixed by reimplementing OpenEditorForAsset()
	for (const auto& tabDefinition : TabDefinitions)
	{
		FName id = tabDefinition.TabID;
		auto widget = tabDefinition.EditorUtilityWidgetBlueprint->GetCreatedWidget();
		if(!ensure(widget))
		{
			UE_LOG(LogTemp, Error, TEXT("Widget is null. Please make sure that all the widgets registerd are spawned in the main window"));
			continue;
		}
		SpawnedWidgets.FindOrAdd(id, widget);
	}

	// Reset parameters that are only required for init
	WidgetContainerAsset->LayoutName = "";
	WidgetContainerAsset->TabDefinitions.Empty();
}

UEditorUtilityWidget* UCLAssetEditorUtility::GetEditorUtilityWidgetByID(FName TabID, TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass)
{
	return SpawnedWidgets.FindRef(TabID);
}

UEditorUtilityWidget* UCLAssetEditorUtility::GetFirstEditorUtilityWidgetOfClass(TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass)
{
	for(auto& [id, widget] : SpawnedWidgets)
	{
		if(EditorUtilityWidgetClass == widget->GetClass())
		{
			return widget;
		}
	}
	return nullptr;
}

void UCLAssetEditorUtility::OnEditorRefresh_Internal()
{
	UAssetEditorSubsystem* assetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	assetEditorSubsystem->CloseAllEditorsForAsset(WidgetContainerAsset);
	
	// Create a TWeakLambdaDelegate to hold a reference to the lambda
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this, [this](/*params*/)
	{
		Run();
		OnEditorRefresh();
	});
	
	GEditor->GetTimerManager()->SetTimerForNextTick(TimerDelegate);
	
}

void UCLAssetEditorUtility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Leave if it's not the CDO
	if(!HasAnyFlags(RF_ClassDefaultObject)) return;
	
	// auto Name1 = PropertyChangedEvent.MemberProperty->GetFName();
	// auto Name2 = PropertyChangedEvent.Property->GetFName();

	// Get the name of the edited property, and the names of the properties we want to compare
	const auto editedName = PropertyChangedEvent.GetPropertyName();
	const auto tabIDName = GET_MEMBER_NAME_CHECKED(FCLTabDefinition, TabID);
	const auto tabDefinitionsName = GET_MEMBER_NAME_CHECKED(UCLAssetEditorUtility, TabDefinitions);

	// If we edited the definitions of the TabIDs in any way, then reset the layout
	if(editedName == tabIDName || editedName == tabDefinitionsName)
	{
		const TSet<FName> tabDefinitionIds = GetValidTabIds();

		const int numMatches = tabDefinitionIds.Intersect(StoredTabIds).Num();
		if(numMatches != tabDefinitionIds.Num() || numMatches != StoredTabIds.Num())
		{
			UE_LOG(LogTemp, Log, TEXT("LAYOUTS RESET"));
			constexpr bool bResetDefaultLayout = true;
			ResetLayout(bResetDefaultLayout);
			StoredTabIds = tabDefinitionIds;
		}
	}
	
	// UE_LOG(LogTemp, Log, TEXT("editedName %s"), *editedName.ToString());
	// UE_LOG(LogTemp, Log, TEXT("tabIDName %s"), *tabIDName.ToString());
}

void UCLAssetEditorUtility::LoadValidLayout()
{
	// Load layout from `EditorLayout.ini`
	const FString& layoutName = GetLayoutName();
	const FString& pluginLayoutFilePath = GetLayoutFilePath();
	const TSharedRef<FTabManager::FLayout> loadedEditorLayout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, MakeEmptyLayout(*layoutName));
	
	// Load layout from `CLAssetEditor_DefaultLayouts.json`
	const TSharedPtr<FTabManager::FLayout> loadedDefaultLayout = LoadLayoutFromJsonFile(*pluginLayoutFilePath, *layoutName);
	if(!loadedDefaultLayout.IsValid() && !loadedEditorLayout.ToSharedPtr().IsValid())
	{
		// If both are invalid, reset the layout
		ResetLayout();
		return;
	}
	else if(!loadedEditorLayout.ToSharedPtr().IsValid())
	{
		// If the editor layout is invalid, load the default layout
		LoadDefaultLayout();
		return;
	}
	else if(!loadedDefaultLayout.IsValid())
	{
		// If the default layout is invalid, just use the one in the editor.
		// No need to do anything
		return;
	}
	
	//# If both layouts are valid, then compare.
	//# If they have unmatched tabs, then load the default layout.
	//# Otherwise keep using the editor layout.
	const TSet<FString>& defaultTabIdValues = UCLAssetEditorLibrary::GetAllJsonStringValuesForKey(loadedDefaultLayout.ToSharedRef()->ToJson(), "TabId");
	const TSet<FString>& editorTabIdValues = UCLAssetEditorLibrary::GetAllJsonStringValuesForKey(loadedEditorLayout->ToJson(), "TabId");
	const TSet<FString>& validAssetTabIds = GetValidTabIdsAsString();

	// If the intersection of tabIds are different, it means that the editorLayout and defaultLayout are structurably different.
	// In which case, we should attempt to load the default layout.
	const int matchesNum = defaultTabIdValues.Intersect(editorTabIdValues).Num();
	if(matchesNum != defaultTabIdValues.Num() || matchesNum != editorTabIdValues.Num())
	{
		// And finally, only load the default layout if there's a difference between the validAssetTabIds and the editorTabIdValues
		const int tabsDifferenceWithDefaultNum = defaultTabIdValues.Difference(validAssetTabIds).Num();
		if (tabsDifferenceWithDefaultNum == 0)
		{
			// Only load the default layout if it supports all the valid tab ids
			LoadDefaultLayout();
		}
	}
}

void UCLAssetEditorUtility::ResetLayout(bool bResetDefaultLayout)
{
	const FString& pluginLayoutFilePath = GetLayoutFilePath();
	const FString& layoutName = GetLayoutName();
	
	// If the asset is opened manually/normally, then always use the same default layout for any asset opened this way.
	const TSharedRef<FTabManager::FLayout> emptyLayout = MakeEmptyLayout(*layoutName);
	
	// This will save an empty layout for this asset in the `EditorLayout.ini`, replacing any existing layout
	FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, emptyLayout);

	if(bResetDefaultLayout)
	{
		// Load all layouts json from `Config/CLAssetEditor/CLAssetEditor_Layouts.json`
		const TSharedPtr<FJsonObject> allJsonLayouts = UCLAssetEditorLibrary::LoadJsonFile(*pluginLayoutFilePath);
		if(!allJsonLayouts.IsValid()) return;
		// Remove this layout from the allLayouts json
		if(allJsonLayouts->HasField(*layoutName))
		{
			allJsonLayouts->RemoveField(*layoutName);
			// Save the json back to disk
			UCLAssetEditorLibrary::SaveJsonFile(*pluginLayoutFilePath, allJsonLayouts);
		}
	}
}

bool UCLAssetEditorUtility::SaveDefaultLayout()
{
	const FString& pluginLayoutFilePath = GetLayoutFilePath();
	const FString& layoutName = GetLayoutName();

	//# Load layout from `EditorLayout.ini`
	
	const TSharedRef<FTabManager::FLayout> loadedLayout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, MakeEmptyLayout(*layoutName));

	//# Save the loaded layout into `Config/CLAssetEditor/CLAssetEditor_Layouts.json`
	
	// Load `CLAssetEditor_Layouts.json` or create a new json object if it's invalid
	TSharedPtr<FJsonObject> allJsonLayouts = UCLAssetEditorLibrary::LoadJsonFile(*pluginLayoutFilePath);
	if(!allJsonLayouts.IsValid())
	{
		// doesn't exist, so make a new
		allJsonLayouts = MakeShared<FJsonObject>();
	}
	// Add/replace the relevant field of the loaded layout (layoutName)
	allJsonLayouts->SetObjectField(*layoutName, loadedLayout->ToJson());
	// Save the json object again
	return UCLAssetEditorLibrary::SaveJsonFile(*pluginLayoutFilePath, allJsonLayouts);
}

void UCLAssetEditorUtility::LoadDefaultLayout()
{
	// Load layout from json object field (by layout name)
	const TSharedRef<FTabManager::FLayout> loadedLayout = LoadLayoutFromJsonFile(*GetLayoutFilePath(), *GetLayoutName()).ToSharedRef();
	if(loadedLayout.ToSharedPtr().IsValid())
	{
		// Save it to the editor EditorLayout.ini
		FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, loadedLayout);
	}
}

TSharedRef<FTabManager::FLayout> UCLAssetEditorUtility::MakeEmptyLayout(const FString& LayoutName)
{
	const TSharedRef<FTabManager::FLayout> emptyLayout = FTabManager::NewLayout(*LayoutName)
	->AddArea
	(
		FTabManager::NewPrimaryArea()
	);
	
	return emptyLayout;
}

FString UCLAssetEditorUtility::GetLayoutName() const
{
	return "CL_" + GetClass()->ClassGeneratedBy->GetName() + "_Layout";
}

FString UCLAssetEditorUtility::GetLayoutFilePath()
{
	return FPaths::ProjectConfigDir() + "Layouts/CLAssetEditor_DefaultLayouts.json";
}

TSharedPtr<FTabManager::FLayout> UCLAssetEditorUtility::LoadLayoutFromJsonFile(const FString& LayoutFilePath, const FString& LayoutName)
{
	TSharedPtr<FTabManager::FLayout> loadedLayout = nullptr;

	//# Load all layouts json from `Config/CLAssetEditor/CLAssetEditor_Layouts.json`

	const TSharedPtr<FJsonObject> allJsonLayouts = UCLAssetEditorLibrary::LoadJsonFile(*LayoutFilePath);
	if(!allJsonLayouts.IsValid()) return loadedLayout;

	//# Get the relevant layout from the allLayouts json, and save it to the editor config
	
	const TSharedPtr<FJsonObject>* layoutJson = nullptr;
	if(allJsonLayouts->TryGetObjectField(*LayoutName, OUT layoutJson))
	{
		// Load layout from json object field (by layout name)
		loadedLayout = FTabManager::FLayout::NewFromJson(*layoutJson);
	}
	return loadedLayout;
}

TSet<FCLTabDefinition> UCLAssetEditorUtility::GetValidTabDefinitions() const
{
	TSet<FCLTabDefinition> validTabDefinitions;
	for(const FCLTabDefinition& tab : TabDefinitions)
	{
		validTabDefinitions.Add(tab);
	}
	return validTabDefinitions;
}

TSet<FName> UCLAssetEditorUtility::GetValidTabIds() const
{
	TSet<FCLTabDefinition> validTabDefinitions = GetValidTabDefinitions();
	TSet<FName> validTabIds;
	for(const FCLTabDefinition& tab : validTabDefinitions)
	{
		validTabIds.Add(tab.TabID);
	}
	return validTabIds;
}

TSet<FString> UCLAssetEditorUtility::GetValidTabIdsAsString() const
{
	TSet<FCLTabDefinition> validTabDefinitions = GetValidTabDefinitions();
	TSet<FString> validTabIds;
	for(const FCLTabDefinition& tab : validTabDefinitions)
	{
		validTabIds.Add(tab.TabID.ToString());
	}
	return validTabIds;
}
