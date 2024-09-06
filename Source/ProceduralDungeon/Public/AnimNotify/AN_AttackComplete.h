// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_AttackComplete.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API UAN_AttackComplete : public UAnimNotify
{
	GENERATED_BODY()
protected:
	/*영어 쓰면 내부 컴파일 에러 발생함*/
	FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
