// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AN_MeleeAttack.h"
#include "Interface/INT_EnemyCharacter.h"

FString UAN_MeleeAttack::GetNotifyName_Implementation() const
{
	return TEXT("MeleeAttack");
}

void UAN_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* attacker = MeshComp->GetOwner();
	if (!MeshComp->GetOwner())
	{
		return;
	}
	if (attacker->GetClass()->ImplementsInterface(UINT_EnemyCharacter::StaticClass()))
	{
		IINT_EnemyCharacter::Execute_EnemyMeleeAttack(attacker);
	}
}
