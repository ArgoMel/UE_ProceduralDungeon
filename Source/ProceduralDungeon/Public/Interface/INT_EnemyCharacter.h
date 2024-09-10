// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_EnemyCharacter.generated.h"

class ABaseEnemy;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UINT_EnemyCharacter : public UInterface
{
	GENERATED_BODY()
};

class PROCEDURALDUNGEON_API IINT_EnemyCharacter
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ABaseEnemy* GetEnemyRef();
};
