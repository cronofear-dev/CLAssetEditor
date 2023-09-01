// CubyLab Games™ All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "CLEdSubsystemTest.generated.h"

/**
 * 
 */
UCLASS()
class CLASSETEDITOR_API UCLEdSubsystemTest : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "CLAssetEditor")
	FString VisibleAnywhere;

	UPROPERTY(VisibleDefaultsOnly, Category = "CLAssetEditor")
	FString VisibleDefaultsOnly;

	UPROPERTY(VisibleInstanceOnly, Category = "CLAssetEditor")
	FString VisibleInstanceOnly;

	UPROPERTY(EditAnywhere, Category = "CLAssetEditor")
	FString EditAnywhere;

	UPROPERTY(EditDefaultsOnly, Category = "CLAssetEditor")
	FString EditDefaultsOnly;
	
	UPROPERTY(EditInstanceOnly, Category = "CLAssetEditor")
	FString EditInstanceOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CLAssetEditor")
	TArray<FString> Array;

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", CallInEditor)
	void WillThisWork();

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor")
	void GetAllEditorSubsystems(TArray<UEditorSubsystem*>& EditorSubsystem);
};
