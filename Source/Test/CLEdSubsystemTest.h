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
	UPROPERTY(VisibleAnywhere)
	FString VisibleAnywhere;

	UPROPERTY(VisibleDefaultsOnly)
	FString VisibleDefaultsOnly;

	UPROPERTY(VisibleInstanceOnly)
	FString VisibleInstanceOnly;

	UPROPERTY(EditAnywhere)
	FString EditAnywhere;

	UPROPERTY(EditDefaultsOnly)
	FString EditDefaultsOnly;
	
	UPROPERTY(EditInstanceOnly)
	FString EditInstanceOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Array;

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", CallInEditor)
	void WillThisWork();

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor")
	void GetAllEditorSubsystems(TArray<UEditorSubsystem*>& EditorSubsystem);
};
