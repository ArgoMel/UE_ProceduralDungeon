// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Character/BasePlayer.h"
#include "Player_Healer.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API APlayer_Healer : public ABasePlayer
{
	GENERATED_BODY()
public:
	APlayer_Healer();
public:
	virtual void Server_Action1_Implementation() override;
	virtual void Server_Action2_Implementation() override;
public:
	void UseAction1_Implementation();
	void UseAction2_Implementation();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TArray<UAnimMontage*> mAction1Montages;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mAction2Montage;
};
