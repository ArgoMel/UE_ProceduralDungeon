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
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	void SetPlayerCanMove_Implementation(bool CanMove);
	void UpdatePlayerHUD_Implementation(float HP, float MP);
	void PlayerRespawn_Implementation(FVector PlayerSpawnLoc, TSubclassOf<ABasePlayer> PlayerClass);
	void PlayerSelectScreenChoice_Implementation(TSubclassOf<ABasePlayer> SelectedClass);
	void PlayerFirstSpawn_Implementation(TSubclassOf<ABasePlayer> PlayerClass, FVector SpawnLoc);

protected:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	TObjectPtr<APawn> mPlayerPawn;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bCanMove;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> mCharacterSelectClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UUserWidget> mCharacterSelect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly , Category = "Widget")
	TSubclassOf<UUserWidget> mHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UHUDWidget> mHUD;

protected:
	UFUNCTION()
	virtual void MoveTriggered(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_SpawnCharacter(TSubclassOf<ABasePlayer> SelectedClass);
	virtual void Server_SpawnCharacter_Implementation(TSubclassOf<ABasePlayer> SelectedClass);

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
