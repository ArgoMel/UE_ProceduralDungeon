// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BaseCharacter.h"
#include "Interface/INT_PlayerCharacter.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROCEDURALDUNGEON_API ABasePlayer : public ABaseCharacter
	, public IINT_PlayerCharacter
{
	GENERATED_BODY()
public:
	ABasePlayer();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/*서버에선만 실행*/
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetOwner(AActor* NewOwner) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	void SetIsAttacking_Implementation(bool Attacking);
	ABasePlayer* GetPlayerRef_Implementation();
	void InitializeHUD_Implementation();
	void HealPlayer_Implementation(float HealAmount);
public:
	virtual void Server_Death_Implementation() override;
	virtual void Death() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USpringArmComponent> mSpringArm;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mCamera;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "MustSet")
	TSubclassOf<ABasePlayer> mCharacterClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mMaxMana;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mCurMana;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction1ManaCost;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction2ManaCost;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction3ManaCost;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction4ManaCost;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction1Damage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction2Damage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction3Damage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mAction4Damage;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TObjectPtr<APlayerController> mPlayerController;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bCurrentlyAttacking;

public:
	UFUNCTION(BlueprintPure)
	bool CanUseMana(float ManaCost);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action1();
	virtual void Server_Action1_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action1End();
	virtual void Server_Action1End_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action2();
	virtual void Server_Action2_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action2End();
	virtual void Server_Action2End_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action3();
	virtual void Server_Action3_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action3End();
	virtual void Server_Action3End_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action4();
	virtual void Server_Action4_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Action4End();
	virtual void Server_Action4End_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_UpdateHUD();
	virtual void Server_UpdateHUD_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_RespawnPlayer();
	virtual void Server_RespawnPlayer_Implementation();
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();
};
