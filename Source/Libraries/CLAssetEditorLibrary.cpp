// MIT License


#include "CLAssetEditorLibrary.h"

#include "Subsystems/EditorAssetSubsystem.h"

UObject* UCLAssetEditorLibrary::GetBlueprintAssetFromObject(UObject* Object)
{
	if(!Object) return nullptr;
	
	// Get Asset (Blueprint) from a given UObject
	// `GetClass()` be casted to `UBlueprintGeneratedClass` for additional data
	UObject* Asset = Object->GetClass()->ClassGeneratedBy;
	return  Asset;
}

UObject* UCLAssetEditorLibrary::GetBlueprintAssetFromClass(UClass* Class)
{
	return Class->ClassGeneratedBy;
}

UObject* UCLAssetEditorLibrary::GetDefaultObjectFromClass(UClass* Class)
{
	// Get default CDO from a given UClass
	return Class->ClassDefaultObject;
}

bool UCLAssetEditorLibrary::IsAssetDirty(UObject* Object)
{
	UPackage* Package = Object->GetPackage();
	return Package->IsDirty();
}
