// MIT License

#pragma once

#include "AssetTypeActions_Base.h"

/**
 * This defines the information about the factory and how the asset is opened.
 * - GetCategories(): In which category the asset will appear in the content browser
 * - GetTypeColor(): The color of the asset type in the content browser
 * - Etc.
 */
class FCLWidgetContainerAssetTypeActions : public FAssetTypeActions_Base
{
public:
    // ~ Begin IAssetTypeActions Interface
    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual uint32 GetCategories() override;
    virtual UClass* GetSupportedClass() const override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) override;
    // ~ End IAssetTypeActions Interface
};
