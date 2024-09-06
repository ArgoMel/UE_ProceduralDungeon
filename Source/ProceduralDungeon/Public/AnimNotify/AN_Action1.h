// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Action1.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API UAN_Action1 : public UAnimNotify
{
	GENERATED_BODY()
protected:
	FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
