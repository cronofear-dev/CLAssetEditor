// MIT License


#include "CLWidgetContainerAssetTypeActions.h"
#include "Objects/CLWidgetContainerAsset.h"
#include "Widgets/CLWidgetContainerAssetEditorToolkit.h"

FText FCLWidgetContainerAssetTypeActions::GetName() const
{
	return FText::FromString(TEXT("Editor Widget Container"));
}

FColor FCLWidgetContainerAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FCLWidgetContainerAssetTypeActions::GetCategories()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return FAssetToolsModule::GetModule().Get().FindAdvancedAssetCategory(FName("EditorUtilities")); // This category was created in the CLAssetEditorModule
	}
	return EAssetTypeCategories::Misc;
}

UClass* FCLWidgetContainerAssetTypeActions::GetSupportedClass() const
{
	return UCLWidgetContainerAsset::StaticClass();
}

void FCLWidgetContainerAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UCLWidgetContainerAsset* currentAsset = Cast<UCLWidgetContainerAsset>(*ObjIt))
		{
			const TSharedRef<FCLWidgetContainerAssetEditorToolkit> NewEditor(new FCLWidgetContainerAssetEditorToolkit());
			NewEditor->InitAssetEditor(mode, EditWithinLevelEditor, currentAsset);
		}
	}
}