﻿// MIT License

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CLAssetEditorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CLASSETEDITOR_API UCLAssetEditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*Get the UBlueprint object from a given UObject reference*/
	UFUNCTION(BlueprintPure, Category = "CLAssetEditor", meta=(DeterminesOutputType = "Object"))
	static UObject* GetBlueprintAssetFromObject(UObject* Object);
	/*Get the UBlueprint object from a given UClass reference*/
	UFUNCTION(BlueprintPure, Category = "CLAssetEditor", meta=(DeterminesOutputType = "Class"))
	static UObject* GetBlueprintAssetFromClass(UClass* Class);

	UFUNCTION(BlueprintPure, Category = "CLAssetEditor", meta=(DeterminesOutputType = "Class"))
	static UObject* GetDefaultObjectFromClass(UClass* Class);
	

	/**
	 * See: UEditorAssetLibrary::SaveLoadedAsset for reference
	 */
	UFUNCTION(BlueprintPure, Category = "CLAssetEditor")
	static bool IsAssetDirty(UObject* Object);
};