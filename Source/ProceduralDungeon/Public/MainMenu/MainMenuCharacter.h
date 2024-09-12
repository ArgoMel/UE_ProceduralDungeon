// Fill out your copyright notice in the Description page of Project Settings.

class UCameraComponent;

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuCharacter.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API AMainMenuCharacter : public APawn
{
	GENERATED_BODY()
public:
	AMainMenuCharacter();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mCamera;
};
