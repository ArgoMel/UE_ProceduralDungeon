// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_DungeonGame.h"
#include "Character/BasePlayer.h"
#include "Widget/HUDWidget.h"
#include "Widget/GameStatsWidget.h"
#include "Widget/UpgradeScreenWidget.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Interface/INT_GameMode.h"
#include "Input/DungeonGameIAs.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

APC_DungeonGame::APC_DungeonGame()
{
	mGold = 0;
	mKillCount = 0;
	bCanMove = false;

	//mCharacterSelectClass = FSoftClassPath("/Game/_Main/Widget/WBP_CharacterSelect.WBP_CharacterSelect_C").TryLoadClass<UUserWidget>();
	//GetClassAsset(mCharacterSelectClass, UUserWidget,"/Game/_Main/Widget/WBP_CharacterSelect.WBP_CharacterSelect");
	GetClassAsset(mHUDClass, UUserWidget,"/Game/_Main/Widget/WBP_HUD.WBP_HUD_C");
	GetClassAsset(mGameStatsClass, UUserWidget,"/Game/_Main/Widget/WBP_GameStats.WBP_GameStats_C");
	GetClassAsset(mUpgradeScreenClass, UUserWidget,"/Game/_Main/Widget/WBP_UpgradeScreen.WBP_UpgradeScreen_C");

	GetObjectAsset(mDungeonGameIAs, UDungeonGameIAs,"/Game/_Main/Inputs/DA_DungoenGameIAs.DA_DungoenGameIAs");
}

void APC_DungeonGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mPlayerPawn);
	DOREPLIFETIME(ThisClass, mGold);
	DOREPLIFETIME(ThisClass, bCanMove);
}

void APC_DungeonGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (HasAuthority() &&
		mPlayerPawn->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_InitializeHUD(mPlayerPawn);
	}
}

void APC_DungeonGame::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	mPlayerPawn = InPawn;
}

void APC_DungeonGame::BeginPlay()
{
	Super::BeginPlay();
	CreateMainMenu();
	SetShowMouseCursor(true);
}

void APC_DungeonGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	inputSubSystem->ClearAllMappings();
	SetInput(true);
	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(input))
	{
		return;
	}
	EDungeonGameIAs::BindInput_TriggerOnly(input, mDungeonGameIAs->mMove, this, &ThisClass::MoveTriggered);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, mDungeonGameIAs->mPlayerStats, this, &ThisClass::Server_GetStats, &ThisClass::PlayerStatsCompleted, &ThisClass::PlayerStatsCompleted);
}

void APC_DungeonGame::SetPlayerCanMove_Implementation(bool CanMove)
{
	bCanMove = CanMove;
}

void APC_DungeonGame::UpdatePlayerHUD_Implementation(float HP, float MP)
{
	Client_UpdateHUD(HP,MP,mGold);
}

void APC_DungeonGame::PlayerRespawn_Implementation(FVector PlayerSpawnLoc, TSubclassOf<ABasePlayer> PlayerClass)
{
	const FVector offset = FVector(0.f,0.f,50.f);
	FActorSpawnParameters param = FActorSpawnParameters();
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABasePlayer* player= GetWorld()->SpawnActor<ABasePlayer>(PlayerClass, PlayerSpawnLoc+ offset,FRotator::ZeroRotator, param);
	Possess(player);
	bCanMove = true;
}

void APC_DungeonGame::PlayerSelectScreenChoice_Implementation(TSubclassOf<ABasePlayer> SelectedClass)
{
	mCharacterSelect->RemoveFromParent();
	Server_SpawnCharacter(SelectedClass);
}

void APC_DungeonGame::PlayerFirstSpawn_Implementation(TSubclassOf<ABasePlayer> PlayerClass, FVector SpawnLoc)
{
	const FVector offset = FVector(0.f, 0.f, 50.f);
	FActorSpawnParameters param = FActorSpawnParameters();
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABasePlayer* player = GetWorld()->SpawnActor<ABasePlayer>(PlayerClass, SpawnLoc + offset, FRotator::ZeroRotator, param);
	Possess(player);
	bCanMove = true;
}

void APC_DungeonGame::AddGold_Implementation(int32 Amount)
{
	mGold += Amount;
	if (mPlayerPawn->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_InitializeHUD(mPlayerPawn);
	}
}

void APC_DungeonGame::AddKill_Implementation()
{
	++mKillCount;
}

APC_DungeonGame* APC_DungeonGame::GetDungeonPCRef_Implementation()
{
	return this;
}

FPlayerStats APC_DungeonGame::GetPlayerStats_Implementation()
{
	FPlayerStats playerStats;
	playerStats.PlayerName = PlayerState->GetPlayerName();
	playerStats.Gold = mGold;
	playerStats.KillCount = mKillCount;
	if (mPlayerPawn->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		ABasePlayer* player = IINT_PlayerCharacter::Execute_GetPlayerRef(mPlayerPawn);
		if(IsValid(player))
		{
			playerStats.HealthPCT = player->GetCurHP() / player->GetMaxHP();
			playerStats.ManaPCT = player->GetCurMP() / player->GetMaxMP();
		}
	}
	return playerStats;
}

void APC_DungeonGame::UpdateUpgradeScreen_Implementation(bool Show)
{
	TArray<FAbilityUpgrade> upgrades;
	if (Show&&
		mPlayerPawn->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_GetAbilityUpgrades(mPlayerPawn, upgrades);
	}
	Client_UpdateUpgradeScreen(Show, upgrades);
}

void APC_DungeonGame::UpdateAbility_Implementation(int32 Action, int32 SubAction, int32 GoldCost)
{
	Server_TryUpdateAbility(Action, SubAction, GoldCost);
}

void APC_DungeonGame::CreateMainMenu_Implementation()
{
	mCharacterSelect = CreateWidget<UUserWidget>(this, mCharacterSelectClass);
	if (mCharacterSelect)
	{
		mCharacterSelect->AddToViewport();
		FInputModeGameAndUI inputmode;
		inputmode.SetWidgetToFocus(mCharacterSelect->GetCachedWidget());
		inputmode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		inputmode.SetHideCursorDuringCapture(false);
		SetInputMode(inputmode);
	}
}

void APC_DungeonGame::MoveTriggered(const FInputActionValue& Value)
{
	if (!IsValid(mPlayerPawn)||!bCanMove)
	{
		return;
	}
	FVector2D value = Value.Get<FVector2D>();
	mPlayerPawn->AddMovementInput(UKismetMathLibrary::GetForwardVector(mPlayerPawn->GetControlRotation()), value.Y);
	mPlayerPawn->AddMovementInput(UKismetMathLibrary::GetRightVector(mPlayerPawn->GetControlRotation()), value.X);
}

void APC_DungeonGame::PlayerStatsCompleted()
{
	if (IsValid(mGameStats))
	{
		mGameStats->RemoveFromParent();
	}
}

void APC_DungeonGame::Server_SpawnCharacter_Implementation(TSubclassOf<ABasePlayer> SelectedClass)
{
	AGameModeBase* gameMode= UGameplayStatics::GetGameMode(GetWorld());
	if (gameMode->GetClass()->ImplementsInterface(UINT_GameMode::StaticClass()))
	{
		IINT_GameMode::Execute_GetSelectedClass(gameMode, SelectedClass,this);
	}
}

void APC_DungeonGame::Server_TryUpdateAbility_Implementation(int32 Action, int32 SubAction, int32 GoldCost)
{
	if(GoldCost>mGold)
	{
		return;
	}
	mGold -= GoldCost;
	if (mPlayerPawn->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		IINT_PlayerCharacter::Execute_InitializeHUD(mPlayerPawn);
		IINT_PlayerCharacter::Execute_UpgradeAbility(mPlayerPawn, Action, SubAction);
	}
	if (GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_UpdateUpgradeScreen(this, true);
	}
}

void APC_DungeonGame::Server_GetStats_Implementation()
{
	AGameModeBase* gm= UGameplayStatics::GetGameMode(GetWorld());
	TArray<FPlayerStats> playerStats;
	if (gm->GetClass()->ImplementsInterface(UINT_GameMode::StaticClass()))
	{
		IINT_GameMode::Execute_GetPlayerStats(gm, playerStats);
	}
	Client_GetStats(playerStats);
}

void APC_DungeonGame::Client_GetStats_Implementation(const TArray<FPlayerStats>& PlayerStats)
{
	if (!IsValid(mGameStats))
	{
		mGameStats = CreateWidget<UGameStatsWidget>(this, mGameStatsClass);
		mGameStats->Clear();
	}
	int32 index = 0;
	for (FPlayerStats playerStat : PlayerStats)
	{
		mGameStats->AddChild(index, &playerStat);
		++index;
	}
	mGameStats->AddToViewport();
}

void APC_DungeonGame::Client_UpdateHUD_Implementation(float HP, float MP, int32 Gold)
{
	if(!IsValid(mHUD))
	{
		mHUD = CreateWidget<UHUDWidget>(this, mHUDClass);
		mHUD->AddToViewport();
	}
	mHUD->UpdateHUD(HP, MP, Gold);
}

void APC_DungeonGame::Client_UpdateUpgradeScreen_Implementation(bool Show, const TArray<FAbilityUpgrade>& AbilityUpgrade)
{
	if(Show)
	{
		if (!IsValid(mUpgradeScreen))
		{
			mUpgradeScreen = CreateWidget<UUpgradeScreenWidget>(this, mUpgradeScreenClass);
			mUpgradeScreen->Clear();
		}
		int32 index = 0;
		for (FAbilityUpgrade upgrade : AbilityUpgrade)
		{
			mUpgradeScreen->AddChild(index, &upgrade,mGold);
			++index;
		}
		mUpgradeScreen->AddToViewport();
	}
	else
	{
		mUpgradeScreen->RemoveFromParent();
	}
}

void APC_DungeonGame::AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	if (!inputSubSystem->HasMappingContext(InputMapping))
	{
		inputSubSystem->AddMappingContext(InputMapping, MappingPriority);
	}
}

void APC_DungeonGame::RemoveInputMapping(const UInputMappingContext* InputMapping) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	inputSubSystem->RemoveMappingContext(InputMapping);
}

void APC_DungeonGame::SetInput(const bool Enabled) const
{
	ensureMsgf(mDungeonGameIAs, TEXT("mDungeonGameIAs is NULL!"));
	ensure(mDungeonGameIAs->mDefaultMappingContext);
	if (Enabled)
	{
		AddInputMapping(mDungeonGameIAs->mDefaultMappingContext, mDungeonGameIAs->mDefaultMapPriority);
	}
	else
	{
		RemoveInputMapping(mDungeonGameIAs->mDefaultMappingContext);
	}
}
