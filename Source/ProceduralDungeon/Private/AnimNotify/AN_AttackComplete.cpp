// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AN_AttackComplete.h"
#include "Interface/INT_PlayerCharacter.h"

FString UAN_AttackComplete::GetNotifyName_Implementation() const
{
    return TEXT("공격 완료");
}

void UAN_AttackComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* attacker = MeshComp->GetOwner();
	if (!MeshComp->GetOwner())
	{
		return;
	}
	if (attacker->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_SetIsAttacking(attacker, false);
	}
}