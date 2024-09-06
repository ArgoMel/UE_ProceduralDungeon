// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AN_Action1.h"
#include "Interface/INT_PlayerCharacter.h"

FString UAN_Action1::GetNotifyName_Implementation() const
{
    return TEXT("Action1");
}

void UAN_Action1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
	AActor* attacker = MeshComp->GetOwner();
	if (!MeshComp->GetOwner())
	{
		return;
	}
	if (attacker->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_UseAction1(attacker);
	}
}
