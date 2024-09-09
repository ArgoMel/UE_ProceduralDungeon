// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"
#include "AI_EnemyController.h"

ABaseEnemy::ABaseEnemy()
{
	AIControllerClass = AAI_EnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(TAG_ENEMY);
}
