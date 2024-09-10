// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_Enemy.generated.h"

class ABaseEnemy;

UCLASS()
class PROCEDURALDUNGEON_API UAnim_Enemy : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAnim_Enemy();
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TObjectPtr<ABaseEnemy> mEnemyCharacter;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	float mSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bIsDead;
};
