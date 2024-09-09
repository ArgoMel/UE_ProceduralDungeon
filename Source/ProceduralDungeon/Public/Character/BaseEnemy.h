// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/INT_EnemyCharacter.h"
#include "BaseEnemy.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API ABaseEnemy : public ABaseCharacter
	, public IINT_EnemyCharacter
{
	GENERATED_BODY()
public:
	ABaseEnemy();
};
