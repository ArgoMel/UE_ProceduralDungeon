// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI_Player.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API UAI_Player : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAI_Player();
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TObjectPtr<APawn> mPlayerPawn;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	float mSpeed;
};
