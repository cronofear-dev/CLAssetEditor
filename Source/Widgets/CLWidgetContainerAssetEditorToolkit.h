// MIT License

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"

class UCLWidgetContainerAsset;

/**
 * This toolkit allows to fully override the asset editor content (Removing the default details panel, etc.)
 */
class FCLWidgetContainerAssetEditorToolkit : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	// Called from FCLWidgetContainerAssetTypeActions::OpenAssetEditor
	void InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UCLWidgetContainerAsset* InAsset);

	UCLWidgetContainerAsset* GetAssetRef() const { return CLWidgetContainerAsset; }

	void BindCommands();
	void ExtendToolbars();
	
public:
	
	/** IToolkit interface */
	virtual FName GetToolkitFName() const override { return "CLWidgetContainerAssetEditor"; }
	virtual FText GetBaseToolkitName() const override { return INVTEXT("Widget Container Asset Editor"); };
	virtual FString GetWorldCentricTabPrefix() const override { return "Widget Container Asset "; };
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return {}; };
	virtual FText GetToolkitName() const override;
	/** End IToolkit interface */

protected:
	/** IToolkit interface to listen*/
	virtual void SaveAsset_Execute() override;
	virtual void OnClose() override;
	void RefreshEditor();
	/** End IToolkit interface */

public:
	//~ FGCObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FCLWidgetContainerAssetEditorToolkit");
	}
	//~ End FGCObject Interface
	
private:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	// This is called then the tab is recreated, doesn't seem to be called when closed
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
private:
	UCLWidgetContainerAsset* CLWidgetContainerAsset = nullptr;
};
