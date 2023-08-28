// MIT License


#include "CLWidgetContainerAssetEditorToolkit.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "IBlutilityModule.h"
#include "Factory/CLWidgetContainerAsset.h"
#include "Objects/CLAssetEditorUtility.h"

void FCLWidgetContainerAssetEditorToolkit::InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UCLWidgetContainerAsset* InAsset)
{
	CLWidgetContainerAsset = Cast<UCLWidgetContainerAsset>(InAsset);

	// If the asset is opened manually/normally, then always use the same default layout for any asset opened this way.
	FString LayoutName = "CL_WidgetContainer_DefaultLayout";
	if(CLWidgetContainerAsset->AssetEditorUtilityOwner)
	{
		// Otherwise a unique layout is used per AssetEditorUtility (By name, maybe use path if this is an issue)
		UObject* AssetFromOwner = CLWidgetContainerAsset->AssetEditorUtilityOwner->GetClass()->ClassGeneratedBy;
		LayoutName = "CL_" + AssetFromOwner->GetName() + "_Layout";
	}
	
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(*LayoutName)
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
	);

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "CLWidgetContainerAssetEditor", Layout, true, true, InAsset);
}

void FCLWidgetContainerAssetEditorToolkit::ExtendToolbars()
{
	//Register Toolbar Extenders
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
}

FText FCLWidgetContainerAssetEditorToolkit::GetToolkitName() const
{
	// This method is ticking, this is why the name is stored in the CLWidgetContainerAsset asset itself
	if(CLWidgetContainerAsset->MainTabName.IsEmpty()) return FText::FromString("Asset Editor Window");
	return FText::FromString(CLWidgetContainerAsset->MainTabName);
}

void FCLWidgetContainerAssetEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(CLWidgetContainerAsset);
}

void FCLWidgetContainerAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Add category for new tabs
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Editor Utility Widgets"));

	// Register single tab spawner
	// InTabManager->RegisterTabSpawner("MyTabID", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	// {
	// 	return SNew(SDockTab)
	// 	[
	// 		SNew(SCLContainerWidget)
	// 		.InWidgetContainerAsset(this, &FCLWidgetContainerAssetEditorToolkit::GetAssetRef)
	// 	];
	// }))
	// .SetDisplayName(INVTEXT("MyTab"))
	// .SetGroup(WorkspaceMenuCategory.ToSharedRef());

	if(!CLWidgetContainerAsset->AssetEditorUtilityOwner) return;

	for (auto& tabDefinition : CLWidgetContainerAsset->AssetEditorUtilityOwner->TabDefinitions)
	{
		FText DisplayName = tabDefinition.EditorUtilityWidgetBlueprint->GetTabDisplayName();
		if(!tabDefinition.OptionalTabName.IsEmpty())
		{
			DisplayName = tabDefinition.OptionalTabName;
		}
		
		// Register tab with ID
		InTabManager->RegisterTabSpawner(tabDefinition.TabID, FOnSpawnTab::CreateUObject(tabDefinition.EditorUtilityWidgetBlueprint, &UEditorUtilityWidgetBlueprint::SpawnEditorUITab))
			.SetDisplayName(DisplayName)
			.SetGroup(WorkspaceMenuCategory.ToSharedRef());
			// .SetIcon()
		
		tabDefinition.EditorUtilityWidgetBlueprint->SetRegistrationName(tabDefinition.TabID);
		
		// Don't spawn the tabs as the layout save will take care of it
		// TSharedPtr<SDockTab> NewDockTab = InTabManager->TryInvokeTab(tabDefinition.TabID);

		// This may be needed to register the spawned widget so we can get it later
		IBlutilityModule* BlutilityModule = FModuleManager::GetModulePtr<IBlutilityModule>("Blutility");
		if (tabDefinition.EditorUtilityWidgetBlueprint)
		{
			ensure(tabDefinition.EditorUtilityWidgetBlueprint);
			BlutilityModule->AddLoadedScriptUI(tabDefinition.EditorUtilityWidgetBlueprint);
		}
	}
}

void FCLWidgetContainerAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	if(!CLWidgetContainerAsset->AssetEditorUtilityOwner) return;
	
	for (auto& tabDefinition : CLWidgetContainerAsset->AssetEditorUtilityOwner->TabDefinitions)
	{
		InTabManager->UnregisterTabSpawner(tabDefinition.TabID);
	}
}
