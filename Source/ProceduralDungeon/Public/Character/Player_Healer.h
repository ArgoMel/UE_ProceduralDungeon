// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BasePlayer.h"
#include "Player_Healer.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API APlayer_Healer : public ABasePlayer
{
	GENERATED_BODY()
public:
	APlayer_Healer();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void Server_Action1_Implementation() override;
	virtual void Server_Action2_Implementation() override;
	virtual void Server_Action3_Implementation() override;
	virtual void Server_Action3End_Implementation() override;
	virtual void Server_Action4_Implementation() override;
public:
	void UseAction1_Implementation();
	void UseAction2_Implementation();
	void UseAction4_Implementation();

private:
	FTimerHandle mConcentrateTimer;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TArray<UAnimMontage*> mAction1Montages;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mAction2Montage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mAction4Montage;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bConcentrating;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void ConcentrateHandle();
	void ConcentrateHandle_Implementation();
};
