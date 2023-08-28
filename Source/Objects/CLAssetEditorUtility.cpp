// MIT License


#include "CLAssetEditorUtility.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "IBlutilityModule.h"
#include "Factory/CLWidgetContainerAsset.h"
#include "Libraries/CLAssetEditorLibrary.h"
#include "Toolkits/SimpleAssetEditor.h"
#include "Widgets/CLWidgetContainerAssetEditorToolkit.h"

void UCLAssetEditorUtility::OpenEditor()
{
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
	for (auto& tabDefinition : TabDefinitions)
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

	// Null AssetEditorUtilityOwner so CLWidgetContainerAssetEditorToolkit only opens a custom editor when this method is called
	// It will open a default editor if the assets are opened normally
	WidgetContainerAsset->AssetEditorUtilityOwner = nullptr;
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
