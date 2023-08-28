// CubyLab Games™ All Rights Reserved

#pragma once


#include "Styling/AppStyle.h"
#include "Framework/Commands/Commands.h"
#include "Templates/SharedPointer.h"

class FCLAssetEditorToolbarCommands : public TCommands<FCLAssetEditorToolbarCommands>
{
public:
	/** Constructor */
	FCLAssetEditorToolbarCommands() 
		: TCommands<FCLAssetEditorToolbarCommands>("AssetEditorTemplateEditor", NSLOCTEXT("Contexts", "AssetTemplateEditor", "Asset Editor Template Editor")
		, NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	/** ReRun the Editor Utility script (CLAssetEditorUtility->Run()). This should rebuild the asset editor */
	TSharedPtr<FUICommandInfo> ReRunEditorUtility;

	/** Init Commands */
	virtual void RegisterCommands() override;
};
