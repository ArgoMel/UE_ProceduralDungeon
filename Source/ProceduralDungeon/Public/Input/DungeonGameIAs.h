#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Engine/DataAsset.h"
#include "DungeonGameIAs.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(BlueprintType)
class PROCEDURALDUNGEON_API UDungeonGameIAs : public UDataAsset
{
	GENERATED_BODY()
public:
	UDungeonGameIAs();
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputMappingContext> mDefaultMappingContext;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mMove;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mAction1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mAction2;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mAction3;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mAction4;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	int32 mDefaultMapPriority;
};

namespace EDungeonGameIAs
{
	template<class T, class FuncType>
	void BindInput_StartOnly(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc)
	{
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
	}
	template<class T,class FuncType>
	void BindInput_TriggerOnly(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType TriggerFunc)
	{
		if(TriggerFunc)
		{
			Input->BindAction(Action,ETriggerEvent::Triggered,Obj, TriggerFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_StartTriggerComplete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc,FuncType TriggerFunc, FuncType CompleteFunc)
	{
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (TriggerFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_TriggerComplete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType CompleteFunc)
	{
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_StartCompleteCancel(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType CompleteFunc, FuncType CancelFunc)
	{
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
		if (CancelFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Canceled, Obj, CancelFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_StartTriggerCompleteCancel(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType TriggerFunc, FuncType CompleteFunc, FuncType CancelFunc)
	{
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (TriggerFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
		if (CancelFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Canceled, Obj, CancelFunc);
		}
	}
}