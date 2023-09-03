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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
	FName TabID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
    FText OptionalTabName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
	UEditorUtilityWidgetBlueprint* EditorUtilityWidgetBlueprint;

	// Only hash the TabID, and use it as key for TSets and TMaps
	friend uint32 GetTypeHash(const FCLTabDefinition& InDefinition)
	{
		return GetTypeHash(InDefinition.TabID);
	}

	// add a const operator== (Set.add needs it)
	bool operator==(const FCLTabDefinition& Other) const
	{
		return TabID == Other.TabID;
	}
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
	UCLWidgetContainerAsset* WidgetContainerAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
	FString MainTabName = "Asset Editor Window";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CLAssetEditor")
	TSet<FCLTabDefinition> TabDefinitions;

private:
	UPROPERTY()
	TMap<FName, UEditorUtilityWidget*> SpawnedWidgets;
	
	// This property is stored in the asset itself, set by PostEditChangeProperty.
	// It's used to check if any TabId has been changed inside
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TSet<FName> StoredTabIds;
#endif
	
public:
	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor")
	void OpenEditor(bool bLoadValidLayout = true);

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DeterminesOutputType="EditorUtilityWidgetClass"))
	UEditorUtilityWidget* GetEditorUtilityWidgetByID(FName TabID, TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DeterminesOutputType="EditorUtilityWidgetClass"))
	UEditorUtilityWidget* GetFirstEditorUtilityWidgetOfClass(TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEditorSave();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEditorRefresh();
	void OnEditorRefresh_Internal();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEditorClose();

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/**
	 * Will load the editor layout if it's structured in a similar way to the default layout (have the same tab ids).
	 * Otherwise it will load the default layout.
	 * This is done as it's assumed that the default layout has the "true" valid structure (shared through VCS) and should always be preferred.
	 */
	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DevelopmentOnly))
	void LoadValidLayout();
	
	/**
	 * Reset the tabs layout for this asset (will create an empty layout in `EditorLayout.ini`)
	 * If `bResetDefaultLayout` is true, it will also remove this layout from `Config/Layouts/CLAssetEditor_DefaultLayouts.json`
	 */
	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DevelopmentOnly))
	void ResetLayout(bool bResetDefaultLayout = true);

	/**
	 * Saves a default layout for this asset.
	 * Default Layouts are saved as: Project/Config/Layouts/CLAssetEditor_DefaultLayouts.json
	 * Default Layouts are meant to be version controlled (If a user doesn't have a layout the first time an editor is opened, the default layout will be loaded).
	 */
	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DevelopmentOnly))
	bool SaveDefaultLayout();

	UFUNCTION(BlueprintCallable, Category = "CLAssetEditor", meta=(DevelopmentOnly))
	void LoadDefaultLayout();

private:
	// Gets an empty layout for this asset (using its name)
	TSharedRef<FTabManager::FLayout> MakeEmptyLayout(const FString& LayoutName);

	FString GetLayoutName() const;

	static FString GetLayoutFilePath();

	static TSharedPtr<FTabManager::FLayout> LoadLayoutFromJsonFile(const FString& LayoutFilePath, const FString& LayoutName);

	TSet<FCLTabDefinition> GetValidTabDefinitions() const;

	TSet<FName> GetValidTabIds() const;
	
	TSet<FString> GetValidTabIdsAsString() const;
};
