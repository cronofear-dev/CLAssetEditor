// MIT License


#include "CLWidgetContainerAssetEditorToolkit.h"

#include "CLAssetEditorToolbarCommands.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "IBlutilityModule.h"
#include "Objects/CLWidgetContainerAsset.h"
#include "Misc/FileHelper.h"
#include "Objects/CLAssetEditorUtility.h"

void FCLWidgetContainerAssetEditorToolkit::InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UCLWidgetContainerAsset* InAsset)
{
	BindCommands();
	
	CLWidgetContainerAsset = Cast<UCLWidgetContainerAsset>(InAsset);

	// If the asset is opened manually/normally, then always use the same default layout for any asset opened this way.
	FString LayoutName = "CL_WidgetContainer_DefaultLayout";
	if(!CLWidgetContainerAsset->LayoutName.IsEmpty())
	{
		LayoutName = CLWidgetContainerAsset->LayoutName;
	}
	
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(*LayoutName)
	->AddArea
	(
		FTabManager::NewPrimaryArea()
	);


	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "CLWidgetContainerAssetEditor", Layout, true, true, InAsset);
	
	// Custom Changes
	ExtendToolbars();

	// MAP SAVE ASSET ACTION
	// ToolkitCommands->MapAction(
	// FAssetEditorCommonCommands::Get().SaveAsset,
	// FExecuteAction::CreateSP( this, &FAssetEditorToolkit::SaveAsset_Execute ),
	// FCanExecuteAction::CreateSP( this, &FAssetEditorToolkit::CanSaveAsset ));
	
	// TSharedRef<FTabManager::FLayout> layout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, Layout);
	// // auto json = LayoutToUse->ToJson();
	// // json.Get().
	//
	// auto layoutString = layout->ToString();
	// // save to disk
	// auto filePath = FPaths::ProjectDir() + "Config/Layouts/" + LayoutName + ".json";
	// FFileHelper::SaveStringToFile(layoutString, *filePath);
}

void FCLWidgetContainerAssetEditorToolkit::BindCommands()
{
	FCLAssetEditorToolbarCommands::Register();
	
	const FCLAssetEditorToolbarCommands& Commands = FCLAssetEditorToolbarCommands::Get();
	
	ToolkitCommands->MapAction(Commands.ReRunEditorUtility, FExecuteAction::CreateSP(this, &FCLWidgetContainerAssetEditorToolkit::RefreshEditor));
}

void FCLWidgetContainerAssetEditorToolkit::ExtendToolbars()
{
	// Lambda like struct to fill the toolbar
	struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("ExtendToolbarItem");
			{
				ToolbarBuilder.AddToolBarButton(FCLAssetEditorToolbarCommands::Get().ReRunEditorUtility, NAME_None,
					INVTEXT("Refresh"),
					INVTEXT("Click here to Re-Run the Editor Utility handling this container"),
					FSlateIcon()
				);
				
			}
			ToolbarBuilder.EndSection();
		}
	};
	
	//Register Toolbar Extenders
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension("Asset",EExtensionHook::After,GetToolkitCommands(), FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar));

	AddToolbarExtender(ToolbarExtender);

	// FCLAssetEditorModule* AssetEditorModule = &FModuleManager::LoadModuleChecked<FCLAssetEditorModule>("CLAssetEditor");
	// AddToolbarExtender(AssetEditorModule->GetEditorToolbarExtensibilityManager()->GetAllExtenders());

	RegenerateMenusAndToolbars();
}

FText FCLWidgetContainerAssetEditorToolkit::GetToolkitName() const
{
	// This method is ticking, this is why the name is stored in the CLWidgetContainerAsset asset itself
	if(CLWidgetContainerAsset->MainTabName.IsEmpty()) return FText::FromString("Asset Editor Window");
	return FText::FromString(CLWidgetContainerAsset->MainTabName);
}


void FCLWidgetContainerAssetEditorToolkit::SaveAsset_Execute()
{
	FAssetEditorToolkit::SaveAsset_Execute();

	if(CLWidgetContainerAsset->AssetEditorUtilityOwner)
	{
		CLWidgetContainerAsset->AssetEditorUtilityOwner->OnEditorSave();
	}
}

void FCLWidgetContainerAssetEditorToolkit::OnClose()
{
	FAssetEditorToolkit::OnClose();

	if(CLWidgetContainerAsset->AssetEditorUtilityOwner)
	{
		CLWidgetContainerAsset->AssetEditorUtilityOwner->OnEditorClose();
	}
}

void FCLWidgetContainerAssetEditorToolkit::RefreshEditor()
{
	// new AssetEditorUtilityOwner
	if(CLWidgetContainerAsset->AssetEditorUtilityOwner)
	{
		CLWidgetContainerAsset->AssetEditorUtilityOwner->OnEditorRefresh_Internal();
	}
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

	if(CLWidgetContainerAsset->TabDefinitions.Num()==0) return;

	// InTabManager->UnregisterTabSpawner(FName(TEXT("advancedDetails")));
	// InTabManager->CloseAllAreas();

	for (auto& tabDefinition : CLWidgetContainerAsset->TabDefinitions)
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
		
		// tabDefinition.EditorUtilityWidgetBlueprint->SetRegistrationName(tabDefinition.TabID);
		
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

	
	// // If the asset is opened manually/normally, then always use the same default layout for any asset opened this way.
	// FString LayoutName = "CL_WidgetContainer_DefaultLayout";
	// if(CLWidgetContainerAsset->AssetEditorUtilityOwner)
	// {
	// 	// Otherwise a unique layout is used per AssetEditorUtility (By name, maybe use path if this is an issue)
	// 	UObject* AssetFromOwner = CLWidgetContainerAsset->AssetEditorUtilityOwner->GetClass()->ClassGeneratedBy;
	// 	LayoutName = "CL_" + AssetFromOwner->GetName() + "_Layout";
	// }
	
	// auto layouts = layout->ToString();
	// auto filePath = FPaths::ProjectDir() + "Config/Layouts/" + LayoutName + ".layout";
	// FFileHelper::SaveStringToFile(layouts, *filePath);
}

void FCLWidgetContainerAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	if(CLWidgetContainerAsset->TabDefinitions.Num()==0) return;
	
	for (auto& tabDefinition : CLWidgetContainerAsset->TabDefinitions)
	{
		InTabManager->UnregisterTabSpawner(tabDefinition.TabID);
	}
}
