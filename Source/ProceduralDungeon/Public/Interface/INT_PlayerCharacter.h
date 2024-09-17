// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_PlayerCharacter.generated.h"

class ABasePlayer;
class APlayer_Swordsman;

UINTERFACE(MinimalAPI)
class UINT_PlayerCharacter : public UInterface
{
	GENERATED_BODY()
};

class PROCEDURALDUNGEON_API IINT_PlayerCharacter
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsAttacking(bool Attacking);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseAction1();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseAction2();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseAction3();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseAction4();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ABasePlayer* GetPlayerRef();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	APlayer_Swordsman* GetPlayerSwordsmanRef();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeHUD();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HealPlayer(float HealAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddKill();
};
