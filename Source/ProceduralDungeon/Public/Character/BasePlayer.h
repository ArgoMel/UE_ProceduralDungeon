// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BaseCharacter.h"
#include "Interface/INT_PlayerCharacter.h"
#include "BasePlayer.generated.h"

class APC_DungeonGame;
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void SetIsAttacking_Implementation(bool Attacking);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USpringArmComponent> mSpringArm;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mCamera;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TObjectPtr<APC_DungeonGame> mPlayerController;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bCurrentlyAttacking;

public:
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
};
