// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"
#include "Character/BasePlayer.h"
#include "AI_EnemyController.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

ABaseEnemy::ABaseEnemy()
{
	AIControllerClass = AAI_EnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(TAG_ENEMY);

	mMaxHealth = 10;
	mCurHealth = mMaxHealth;

	mAttackDistance = 50.f;
	mTimeBetweenAttacks = 2.f;
	mMeleeDamage = 5.f;
	bIsChasing = false;

	mMeleeBoxCollision= CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBoxCollision"));
	mMeleeBoxCollision->SetupAttachment(RootComponent);
	mMeleeBoxCollision->SetRelativeLocation(FVector(70.f,0.f,0.f));
	mMeleeBoxCollision->SetRelativeScale3D(FVector(1.f,1.f,3.f));
	mMeleeBoxCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mMeleeBoxCollision->SetCollisionProfileName(PROFILENAME_ENEMYATTACK);
}

void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mPlayerTargets);
	DOREPLIFETIME(ThisClass, bIsChasing);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	mMeleeBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMeleeBoxBeginOverlap);
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	mCurHealth -= damage;
	if(mCurHealth<=0)
	{
		mCurHealth = 0;
		Server_Death();
	}
	return damage;
}

void ABaseEnemy::AddPlayerTarget_Implementation(AActor* PlayerTarget, bool Add)
{
	if (!PlayerTarget->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		return;
	}
	ABasePlayer* player= IINT_PlayerCharacter::Execute_GetPlayerRef(PlayerTarget);
	if(Add)
	{
		if(!player->GetIsDead())
		{
			mPlayerTargets.AddUnique(player);
			Chase();
		}
	}
	//remove
	else
	{
		mPlayerTargets.Remove(player);
		if(mPlayerTargets.IsEmpty())
		{
			GoHome();
		}
		else
		{
			Chase();
		}
	}
}

ABaseEnemy* ABaseEnemy::GetEnemyRef_Implementation()
{
	return this;
}

void ABaseEnemy::EnemyMeleeAttack_Implementation()
{
	mMeleeBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mMeleeBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::Death()
{
	Super::Death();
	GetCharacterMovement()->StopMovementImmediately();
	StopAnimMontage(mMeleeAttackMontage);
	StopAnimMontage(mRangedAttackMontage);
	GetCapsuleComponent()->SetCollisionProfileName(PROFILENAME_RAGDOLL);
}

void ABaseEnemy::OnMeleeBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(TAG_PLAYER))
	{
		return;
	}
	UGameplayStatics::ApplyDamage(OtherActor, mMeleeDamage, nullptr, this, nullptr);
}

void ABaseEnemy::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	FTimerHandle tempTimer;
	switch (Result)
	{
	case EPathFollowingResult::Success:
		if (bIsChasing)
		{
			Multi_PlayMontage(mMeleeAttackMontage);
			GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::Chase, mTimeBetweenAttacks);
		}
		break;
	case EPathFollowingResult::Blocked:
		break;
	case EPathFollowingResult::OffPath:
		break;
	case EPathFollowingResult::Aborted:
		break;
	case EPathFollowingResult::Invalid:
		break;
	}
}

void ABaseEnemy::Chase_Implementation()
{
	if(bIsDead)
	{
		return;
	}
	if (mPlayerTargets.IsEmpty())
	{
		GoHome();
		return;
	}
	if(mPlayerTargets[0]->GetIsDead())
	{
		IINT_EnemyCharacter::Execute_AddPlayerTarget(this, mPlayerTargets[0], false);
		return;
	}
	AAIController* controller = Cast<AAIController>(Controller);
	VALIDCHECK(controller);
	bIsChasing = true;
	controller->ReceiveMoveCompleted.RemoveDynamic(this, &ThisClass::OnMoveCompleted);
	controller->ReceiveMoveCompleted.AddDynamic(this, &ThisClass::OnMoveCompleted);
	controller->MoveToActor(mPlayerTargets[0], mAttackDistance,false);
}

void ABaseEnemy::GoHome_Implementation()
{
	bIsChasing = false;
	if (bIsDead)
	{
		return;
	}
	AAIController* controller = Cast<AAIController>(Controller);
	VALIDCHECK(controller);
	controller->ReceiveMoveCompleted.RemoveDynamic(this, &ThisClass::OnMoveCompleted);
	controller->ReceiveMoveCompleted.AddDynamic(this, &ThisClass::OnMoveCompleted);
	controller->MoveToLocation(mInitialSpawnLoc, 5.f, false);
}
