// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_DungeonGame.h"
#include "Character/BasePlayer.h"
#include "Widget/HUDWidget.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Interface/INT_GameMode.h"
#include "Input/DungeonGameIAs.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameModeBase.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

APC_DungeonGame::APC_DungeonGame()
{
	bCanMove = false;

	GetClassAsset(mCharacterSelectClass, UUserWidget,"/Game/_Main/Widget/WBP_CharacterSelect.WBP_CharacterSelect_C");
	GetClassAsset(mHUDClass, UUserWidget,"/Game/_Main/Widget/WBP_HUD.WBP_HUD_C");

	GetObjectAsset(mDungeonGameIAs, UDungeonGameIAs,"/Game/_Main/Inputs/DA_DungoenGameIAs.DA_DungoenGameIAs");
}

void APC_DungeonGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mPlayerPawn);
	DOREPLIFETIME(ThisClass, bCanMove);
}

void APC_DungeonGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	SetPawn(aPawn);
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
	if(HasAuthority()&&
		!UKismetSystemLibrary::IsServer(GetWorld())&&
		!IsLocalPlayerController())
	{
		return;
	}
	mCharacterSelect = CreateWidget<UUserWidget>(this, mCharacterSelectClass);
	if(!mCharacterSelect)
	{
		return;
	}
	mCharacterSelect->AddToViewport();
	FInputModeGameAndUI inputmode;
	inputmode.SetWidgetToFocus(mCharacterSelect->GetCachedWidget());
	inputmode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputmode.SetHideCursorDuringCapture(false);
	SetInputMode(inputmode);
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
}

void APC_DungeonGame::SetPlayerCanMove_Implementation(bool CanMove)
{
	bCanMove = CanMove;
}

void APC_DungeonGame::UpdatePlayerHUD_Implementation(float HP, float MP)
{
	Client_UpdateHUD(HP,MP);
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

void APC_DungeonGame::Server_SpawnCharacter_Implementation(TSubclassOf<ABasePlayer> SelectedClass)
{
	AGameModeBase* gameMode= UGameplayStatics::GetGameMode(GetWorld());
	if (gameMode->GetClass()->ImplementsInterface(UINT_GameMode::StaticClass()))
	{
		IINT_GameMode::Execute_GetSelectedClass(gameMode, SelectedClass,this);
	}
}

void APC_DungeonGame::Client_UpdateHUD_Implementation(float HP, float MP)
{
	//if(!mHUD)
	//{
	//	return;
	//}
	if(!IsValid(mHUD))
	{
		mHUD = CreateWidget<UHUDWidget>(this, mHUDClass);
		mHUD->AddToViewport();
	}
	mHUD->UpdateHUD(HP, MP);
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
