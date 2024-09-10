// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim_Enemy.h"
#include "Character/BaseEnemy.h"
#include "Interface/INT_EnemyCharacter.h"

UAnim_Enemy::UAnim_Enemy()
{
	mSpeed = 0.f;
	bIsDead = false;
}

void UAnim_Enemy::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* pawn = TryGetPawnOwner();
	if (!IsValid(pawn))
	{
		FTimerHandle tempTimer;
		GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::NativeInitializeAnimation, 0.2f);
		return;
	}
	if (pawn->GetClass()->ImplementsInterface(UINT_EnemyCharacter::StaticClass()))
	{
		mEnemyCharacter =IINT_EnemyCharacter::Execute_GetEnemyRef(pawn);
	}
	else
	{
		FTimerHandle tempTimer;
		GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::NativeInitializeAnimation, 0.2f);
	}
}

void UAnim_Enemy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(mEnemyCharacter))
	{
		return;
	}
	mSpeed = mEnemyCharacter->GetVelocity().Length();
	bIsDead = mEnemyCharacter->GetIsDead();
}
