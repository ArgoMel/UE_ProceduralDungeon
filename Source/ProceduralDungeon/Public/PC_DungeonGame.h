// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/PlayerController.h"
#include "Interface/INT_PlayerController.h"
#include "PC_DungeonGame.generated.h"

class UHUDWidget;
class UDungeonGameIAs;

UCLASS()
class PROCEDURALDUNGEON_API APC_DungeonGame : public APlayerController
	, public IINT_PlayerController
{
	GENERATED_BODY()
public:
	APC_DungeonGame();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
public:
	void SetPlayerCanMove_Implementation(bool CanMove);
	void UpdatePlayerHUD_Implementation(float HP, float MP);
	void PlayerRespawn_Implementation(FVector PlayerSpawnLoc, TSubclassOf<ABasePlayer> PlayerClass);

protected:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	TObjectPtr<APawn> mPlayerPawn;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bCanMove;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly , Category = "Widget")
	TSubclassOf<UUserWidget> mHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UHUDWidget> mHUD;

protected:
	UFUNCTION()
	virtual void MoveTriggered(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Client")
	void Client_UpdateHUD(float HP, float MP);
	virtual void Client_UpdateHUD_Implementation(float HP, float MP);

#pragma region EnhancedInput
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDungeonGameIAs> mDungeonGameIAs;

public:
	UFUNCTION()
	void AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority = 0) const;
	UFUNCTION()
	void RemoveInputMapping(const UInputMappingContext* InputMapping) const;
	UFUNCTION()
	void SetInput(const bool Enabled = true) const;

	UFUNCTION()
	UDungeonGameIAs* GetDungeonGameIAs() const
	{
		return mDungeonGameIAs;
	}
#pragma endregion
};
