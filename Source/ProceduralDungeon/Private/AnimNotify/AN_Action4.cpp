// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AN_Action4.h"
#include "Interface/INT_PlayerCharacter.h"

FString UAN_Action4::GetNotifyName_Implementation() const
{
    return TEXT("Action4");
}

void UAN_Action4::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* attacker = MeshComp->GetOwner();
	if (!MeshComp->GetOwner())
	{
		return;
	}
	if (attacker->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_UseAction4(attacker);
	}
}
