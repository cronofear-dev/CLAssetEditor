// CubyLab Games™ All Rights Reserved


#include "CLAssetEditorToolbarCommands.h"
#include "InputCoreTypes.h"

#define LOCTEXT_NAMESPACE "FCLAssetEditorToolbarCommands" // UI_COMMAND needs to be within a namespace apparently

void FCLAssetEditorToolbarCommands::RegisterCommands()
{
	// FInputChord(EModifierKey::Control, EKeys::R) - Add this to Refresh using CTRL+R
	UI_COMMAND(ReRunEditorUtility, "Refresh", "Refresh", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
