// MIT License

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CLWidgetContainerAsset.generated.h"

struct FCLTabDefinition;
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
	/*We store the current active owner only to send events*/
	UPROPERTY()
	UCLAssetEditorUtility* AssetEditorUtilityOwner;

	UPROPERTY()
	FString LayoutName;

	/*Used as the main editor tab name*/
	UPROPERTY()
	FString MainTabName;

	/*Used for building the tabs*/
	UPROPERTY()
	TSet<FCLTabDefinition> TabDefinitions;
};
