// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Player.h"

UAI_Player::UAI_Player()
{
	mSpeed = 0.f;
}

void UAI_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* pawn = TryGetPawnOwner();
	if(!IsValid(pawn))
	{
		FTimerHandle tempTimer;
		GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::NativeInitializeAnimation,0.2f);
		return;
	}
	mPlayerPawn= pawn;
}

void UAI_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(!IsValid(mPlayerPawn))
	{
		return;
	}
	mSpeed = mPlayerPawn->GetVelocity().Length();
}
