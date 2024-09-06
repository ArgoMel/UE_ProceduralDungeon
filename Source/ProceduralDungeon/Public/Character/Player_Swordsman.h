// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BasePlayer.h"
#include "Player_Swordsman.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API APlayer_Swordsman : public ABasePlayer
{
	GENERATED_BODY()
public:
	APlayer_Swordsman();
public:
	void UseAction1_Implementation();
	void UseAction2_Implementation();
	void UseAction3_Implementation();
	void UseAction4_Implementation();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	TArray<UAnimMontage*> mAction1Montages;

public:
	virtual void Server_Action1_Implementation() override;
	virtual void Server_Action2_Implementation() override;
	virtual void Server_Action3_Implementation() override;
	virtual void Server_Action4_Implementation() override;
};
