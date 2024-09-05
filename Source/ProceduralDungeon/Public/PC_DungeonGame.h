// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../ProceduralDungeon.h"
#include "GameFramework/PlayerController.h"
#include "PC_DungeonGame.generated.h"

class UDungeonGameIAs;

UCLASS()
class PROCEDURALDUNGEON_API APC_DungeonGame : public APlayerController
{
	GENERATED_BODY()
public:
	APC_DungeonGame();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region EnhancedInput
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDungeonGameIAs> mDungeonGameIAs;

public:
	UFUNCTION()
	void AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority = 0) const;
	UFUNCTION()
	void RemoveInputMapping(const UInputMappingContext* InputMapping) const;
	UFUNCTION()
	void SetInput(const bool Enabled = true) const;

	UFUNCTION()
	UDungeonGameIAs* GetDungeonGameIAs() const
	{
		return mDungeonGameIAs;
	}
#pragma endregion
};
