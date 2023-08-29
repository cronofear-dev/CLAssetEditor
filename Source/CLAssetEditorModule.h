// Copyright Epic Games, Inc. All Rights Reserved.

// TODO:
// Note: A nice side effect of the current implementation is that if 2 AssetEditorUtility use the same WidgetContainerAsset, they will replace each other when run after another (can't create 2 windows at the same time). If 2 windows are needed at the same time, use 2 different assets.
// - [ ] Add a toolbar button (ExtendToolbars() in the Editor toolkit) to the asset editor to refresh the editor (Execute CLAssetEditorUtility->Run). Maybe extend toolbar using widget editor?
// - [ ] See if it's viable to support multiple widgets of the same type on the same window
	// Currently UCLAssetEditorUtility::OpenEditor->..->GetCreatedWidget() returns only the last created widget of a given type.
	// Since the widgets are spawned in FCLWidgetContainerAssetEditorToolkit::RegisterTabSpawners(). The solution would be to maybe listen to when each spawn is made/create the widget results here and pass it to the owner.
// - [ ] Add listeners for the custom editor (this is related to previous todos). Listeners for when the editor is saved/closed/etc. Tabs are spawned/closed/etc.
// - [ ] Fix issue where widgets are not bound if they are not opened when the editor is opened. Maybe force them open? Small issue since the intended use is to have all widgets open.
// - [ ] Make sure everything is well implemented, particularly the EditorToolkit (Because it was halfly done), see  https://github.com/Sythenz/AssetEditorTemplate for reference
// - [ ] Add an option to easily extend the toolbar with custom menu/buttons. Maybe even use editor widget for that?
// - [ ] Bug: When a widget editor is unregistered from an AssetEditorUtility, the tab that belonged to that editor won't be able to be removed (can be removed by restarting the editor layours or by re-adding the widget then closing the tab).
		// A potential fix to this issue is to close tabs on close window, reopen them on windows open. A better fix would be checking the tabs and seeing if any is invalid, maybe use the tab manager in the toolkit?
// - [ ] For each tab, add the option to navigate to the utility widget asset (or edit it).
// - [ ] Similarly, add the option in the asset editor to navigate to the utility object/edit it.
// Issues:
// When the editor is restarted and the option for reopening the previous windows is selected, the widgets containers will be opened and be empty (This is by design and the asset containers can be reused). I may add a feature so asset containers can save the last editor utility that run them (So they can run it themselves).

#pragma once

#include "Modules/ModuleInterface.h"

class FCLWidgetContainerAssetEditorToolkit;
class IAssetTypeActions;

class FCLAssetEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/** End IModuleInterface implementation */

private:
	// CreatedAssetTypeActions are stored here, so they can be unregistered during shutdown
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
