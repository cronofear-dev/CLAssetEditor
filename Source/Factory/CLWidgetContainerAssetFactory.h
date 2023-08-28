// MIT License

#pragma once

#include "Factories/Factory.h"
#include "CLWidgetContainerAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class UCLWidgetContainerAssetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UCLWidgetContainerAssetFactory();
	
	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface
};
