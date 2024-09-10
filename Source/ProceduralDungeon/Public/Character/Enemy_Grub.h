// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Character/BaseEnemy.h"
#include "Enemy_Grub.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API AEnemy_Grub : public ABaseEnemy
{
	GENERATED_BODY()
public:
	AEnemy_Grub();
public:
	ABaseEnemy* GetEnemyRef_Implemantation();
};
