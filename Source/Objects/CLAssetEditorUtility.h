// MIT License

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityObject.h"
#include "CLAssetEditorUtility.generated.h"

class UCLWidgetContainerAsset;
class UEditorUtilityWidget;
class UEditorUtilityWidgetBlueprint;

// USTRUCT(BlueprintType)
// struct FCLSpawnEditorResponse
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(BlueprintReadOnly)
// 	TMap<FName, UEditorUtilityWidget*> SpawnedWidgets;
// };

USTRUCT(BlueprintType)
struct FCLTabDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TabID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText OptionalTabName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEditorUtilityWidgetBlueprint* EditorUtilityWidgetBlueprint;
};

// DECLARE_DYNAMIC_DELEGATE_OneParam(FCLSpawnEditorDelegate, FCLSpawnEditorResponse, Response);

/**
 * 
 */
UCLASS()
class CLASSETEDITOR_API UCLAssetEditorUtility : public UEditorUtilityObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCLWidgetContainerAsset* WidgetContainerAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MainTabName = "Asset Editor Window";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCLTabDefinition> TabDefinitions;

private:
	TMap<FName, UEditorUtilityWidget*> SpawnedWidgets;
	
public:
	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor")
	void OpenEditor();

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DeterminesOutputType="EditorUtilityWidgetClass"))
	UEditorUtilityWidget* GetEditorUtilityWidgetByID(FName TabID, TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DeterminesOutputType="EditorUtilityWidgetClass"))
	UEditorUtilityWidget* GetFirstEditorUtilityWidgetOfClass(TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass);
	
};
