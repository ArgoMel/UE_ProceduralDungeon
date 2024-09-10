// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

AAI_EnemyController::AAI_EnemyController()
{
	mAISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	mAISenseConfig_Sight->SightRadius = 1000.f;
	mAISenseConfig_Sight->LoseSightRadius = 1500.f;
	mAISenseConfig_Sight->PeripheralVisionAngleDegrees = 180.f;
	mAISenseConfig_Sight->DetectionByAffiliation = FAISenseAffiliationFilter(true,true,true);

	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	mAIPerception->ConfigureSense(*mAISenseConfig_Sight);
	mAIPerception->SetDominantSense(mAISenseConfig_Sight->GetSenseImplementation());
}

void AAI_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this,&ThisClass::OnEnemyTargetPerceptionUpdated);
}

void AAI_EnemyController::OnEnemyTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}
