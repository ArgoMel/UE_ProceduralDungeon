// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "AI_EnemyController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class PROCEDURALDUNGEON_API AAI_EnemyController : public AAIController
{
	GENERATED_BODY()
public:
	AAI_EnemyController();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	TObjectPtr<UAISenseConfig_Sight> mAISenseConfig_Sight;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UAIPerceptionComponent> mAIPerception;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TObjectPtr<APawn> mEnemyPawn;

protected:
	UFUNCTION()
	void OnEnemyTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
