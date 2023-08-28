// MIT License


#include "CLWidgetContainerAssetFactory.h"
#include "CLWidgetContainerAsset.h"

UCLWidgetContainerAssetFactory::UCLWidgetContainerAssetFactory()
{
	// Define this factory so it can create CLWidgetContainerAsset
	SupportedClass = UCLWidgetContainerAsset::StaticClass();
	bCreateNew = true;
	// bEditorImport = false;
	// bEditAfterNew = true;
}

UObject* UCLWidgetContainerAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCLWidgetContainerAsset>(InParent, InClass, InName, Flags, Context);
}