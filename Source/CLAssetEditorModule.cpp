// Copyright Epic Games, Inc. All Rights Reserved.

#include "CLAssetEditorModule.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Factory/CLWidgetContainerAssetTypeActions.h"

// ETabRole::
	// MajorTab: Behaves as a `main window` that can be docked to the editor main window. Has `File,Edit,Window,Help` menu on the top. Can contain:
		// PanelTab: This tab will behave as a separate window to the entire editor (similar to having 2 windows open, but inside the main editor itself)
		// DocumentTab: Is a type of PanelTab that can be used to display a document-style editor (i.e Blueprint details). Has a single instance per asset typically.
	// NomadTab: This has can be attached anywhere in the editor, including the main menu.

// This is the name of the tab that will be spawned when the button is clicked
// Tabs automatically are created in windows if not attached to an existing one
// static const FName CLAssetEditorTabName("CLAssetEditor");

void FCLAssetEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("EditorUtilities")), FText::FromString("Editor Utilities"));

	// Helper lambda for registering asset type actions for automatic cleanup on shutdown
	auto RegisterAssetTypeAction = [&](TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	};

	// Register type actions
	// This handles 2 things:
	// Create Asset: AssetFactory->FactoryCreateNew() is called when the asset is created
	// Open Asset: AssetTypeActions->OpenAssetEditor() -> EditorToolKit->InitAssetEditor() is called when the asset is opened
	RegisterAssetTypeAction(MakeShareable(new FCLWidgetContainerAssetTypeActions));
}

void FCLAssetEditorModule::ShutdownModule()
{
	// Unregister the asset type actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

IMPLEMENT_MODULE(FCLAssetEditorModule, CLAssetEditor)