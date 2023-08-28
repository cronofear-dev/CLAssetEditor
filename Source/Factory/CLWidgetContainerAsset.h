// MIT License

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CLWidgetContainerAsset.generated.h"

class UCLAssetEditorUtility;
class UEditorUtilityWidget;
class UEditorUtilityWidgetBlueprint;
/**
 * 
 */
UCLASS(BlueprintType)
class CLASSETEDITOR_API UCLWidgetContainerAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UCLAssetEditorUtility* AssetEditorUtilityOwner;

	UPROPERTY()
	FString MainTabName;
};
