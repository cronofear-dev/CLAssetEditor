// MIT License


#include "CLAssetEditorLibrary.h"

#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
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

UObject* UCLAssetEditorLibrary::GetDefaultObjectFromObject(UObject* Object)
{
	return Object->GetClass()->GetDefaultObject();
}

UObject* UCLAssetEditorLibrary::GetDefaultObjectFromClass(UClass* Class)
{
	// Get default CDO from a given UClass
	return Class->ClassDefaultObject;
}

UClass* UCLAssetEditorLibrary::GetParentClassFromBlueprintAsset(UBlueprint* BlueprintAsset)
{
	return BlueprintAsset->ParentClass;
}

UBlueprint* UCLAssetEditorLibrary::CastToBlueprint(UObject* Object)
{
	return Cast<UBlueprint>(Object);
}

bool UCLAssetEditorLibrary::IsAssetDirty(UObject* Object)
{
	UPackage* Package = Object->GetPackage();
	return Package->IsDirty();
}

//////////////////////////////////////////////////////////////////////////

bool UCLAssetEditorLibrary::SaveJsonFile(const FString& FilePath, const TSharedPtr<FJsonObject>& JsonObject)
{
	FString jsonString;
	if (JsonToString(JsonObject, OUT jsonString))
	{
		return FFileHelper::SaveStringToFile(*jsonString, *FilePath);
	}
	return false;
}

TSharedPtr<FJsonObject> UCLAssetEditorLibrary::LoadJsonFile(const FString& FilePath)
{
	FString JsonContents;
	if (FFileHelper::LoadFileToString(OUT JsonContents, *FilePath))
	{
		return JsonFromString(*JsonContents);
	}

	return nullptr;
}

bool UCLAssetEditorLibrary::JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& OutJsonString)
{
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		return true;
	}
	return false;
}

TSharedPtr<FJsonObject> UCLAssetEditorLibrary::JsonFromString(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	if(!JsonString.IsEmpty())
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		FJsonSerializer::Deserialize(Reader, JsonObject);
	}

	return JsonObject;
}

TSet<FString> UCLAssetEditorLibrary::GetAllJsonStringValuesForKey(const TSharedPtr<FJsonObject>& JsonObject, const FString& Key)
{
	TSet<FString> AllValues;
	GetAllJsonStringValuesForKey_Internal(MakeShareable(new FJsonValueObject(JsonObject)), OUT AllValues, Key);
	return AllValues;
}

void UCLAssetEditorLibrary::GetAllJsonStringValuesForKey_Internal(const TSharedPtr<FJsonValue>& JsonValue, TSet<FString>& AllValues, const FString& Key)
{
	if (JsonValue.IsValid())
	{
		switch (JsonValue->Type)
		{
		case EJson::Object:
			{
				const TSharedPtr<FJsonObject>& JsonObject = JsonValue->AsObject();
				for (auto& Field : JsonObject->Values)
				{
					if (Field.Value->Type == EJson::String && Field.Value.IsValid() && Field.Key == Key)
					{
						AllValues.Add(Field.Value->AsString());
					}
					if (Field.Value->Type == EJson::Object || Field.Value->Type == EJson::Array)
					{
						GetAllJsonStringValuesForKey_Internal(Field.Value, AllValues, Key);
					}
				}
				break;
			}
		case EJson::Array:
			{
				const TArray<TSharedPtr<FJsonValue>>& ArrayValues = JsonValue->AsArray();
				for (const TSharedPtr<FJsonValue>& Value : ArrayValues)
				{
					if (Value->Type == EJson::Object || Value->Type == EJson::Array)
					{
						GetAllJsonStringValuesForKey_Internal(Value, AllValues, Key);
					}
				}
				break;
			}
		default:
			break;
		}
	}
}
