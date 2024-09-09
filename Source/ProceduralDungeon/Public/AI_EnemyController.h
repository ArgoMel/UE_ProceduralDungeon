// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_EnemyController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class PROCEDURALDUNGEON_API AAI_EnemyController : public AAIController
{
	GENERATED_BODY()
public:
	AAI_EnemyController();
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	TObjectPtr<UAISenseConfig_Sight> mAISenseConfig_Sight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UAIPerceptionComponent> mAIPerception;
};
