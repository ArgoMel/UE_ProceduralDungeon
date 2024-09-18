// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayer.h"
#include "Input/DungeonGameIAs.h"
#include "Interface/INT_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABasePlayer::ABasePlayer()
{
	bUseControllerRotationYaw = false;
	Tags.Add(TAG_PLAYER);

	mMaxMana = 100.f;
	mCurMana = mMaxMana;
	mAction1ManaCost = 0.f;
	mAction2ManaCost = 0.f;
	mAction3ManaCost = 0.f;
	mAction4ManaCost = 0.f;
	mAction1Damage = 0.f;
	mAction2Damage = 0.f;
	mAction3Damage = 0.f;
	mAction4Damage = 0.f;
	mManaRegenOverTime = 0.025f;
	bCurrentlyAttacking = false;

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->SetupAttachment(GetCapsuleComponent());
	mSpringArm->SetRelativeRotation(FRotator(-60.f,0.f,0.f));
	mSpringArm->TargetArmLength=1250.f;
	mSpringArm->bInheritPitch=false;
	mSpringArm->bInheritRoll=false;
	mSpringArm->bInheritYaw=false;
	mSpringArm->bDoCollisionTest=false;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);

	GetCharacterMovement()->RotationRate=FRotator(0.f,720.f,0.f);
	GetCharacterMovement()->bOrientRotationToMovement=true;
}

void ABasePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mMaxMana);
	DOREPLIFETIME(ThisClass, mCurMana);
	DOREPLIFETIME(ThisClass, mAction1ManaCost);
	DOREPLIFETIME(ThisClass, mAction2ManaCost);
	DOREPLIFETIME(ThisClass, mAction3ManaCost);
	DOREPLIFETIME(ThisClass, mAction4ManaCost);
	DOREPLIFETIME(ThisClass, mAction1Damage);
	DOREPLIFETIME(ThisClass, mAction2Damage);
	DOREPLIFETIME(ThisClass, mAction3Damage);
	DOREPLIFETIME(ThisClass, mAction4Damage);
	DOREPLIFETIME(ThisClass, mManaRegenOverTime);
	DOREPLIFETIME(ThisClass, mAbailableUpgrade);
	DOREPLIFETIME(ThisClass, bCurrentlyAttacking);
}

void ABasePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	mPlayerController = Cast<APlayerController>(NewController);
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::Server_UpdateHUD,0.3f);
}

void ABasePlayer::OnRep_Owner()
{
	Super::OnRep_Owner();
	mPlayerController = Cast<APlayerController>(Owner);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!input || 
		!mPlayerController||
		!mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		return;
	}
	const UDungeonGameIAs* dungeonGameIAs = IINT_PlayerController::Execute_GetDungeonGameIAs(mPlayerController);
	if (!dungeonGameIAs)
	{
		return;
	}
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction1, this, &ThisClass::Server_Action1, &ThisClass::Server_Action1End, &ThisClass::Server_Action1End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction2, this, &ThisClass::Server_Action2, &ThisClass::Server_Action2End, &ThisClass::Server_Action2End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction3, this, &ThisClass::Server_Action3, &ThisClass::Server_Action3End, &ThisClass::Server_Action3End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction4, this, &ThisClass::Server_Action4, &ThisClass::Server_Action4End, &ThisClass::Server_Action4End);
}

float ABasePlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	mCurHealth -= damage;
	Server_UpdateHUD();
	if (mCurHealth <= 0)
	{
		mCurHealth = 0;
		Server_Death();
	}
	return damage;
}

void ABasePlayer::SetIsAttacking_Implementation(bool Attacking)
{
	if(HasAuthority())
	{
		bCurrentlyAttacking = Attacking;	
		if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
		{
			IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, true);
		}
	}
}

ABasePlayer* ABasePlayer::GetPlayerRef_Implementation()
{
	return this;
}

void ABasePlayer::InitializeHUD_Implementation()
{
	Server_UpdateHUD();
}

void ABasePlayer::HealPlayer_Implementation(float HealAmount)
{
	if (bIsDead)
	{
		return;
	}
	mCurHealth =FMath::Clamp(mCurHealth+HealAmount,0.f,mMaxHealth);
	Server_UpdateHUD();
}

void ABasePlayer::AddKill_Implementation()
{
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_AddKill(mPlayerController);
	}
}

void ABasePlayer::UpdateUpgradeScreen_Implementation(bool ShowScreen)
{
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_UpdateUpgradeScreen(mPlayerController, ShowScreen);
	}
}

void ABasePlayer::GetAbilityUpgrades_Implementation(TArray<FAbilityUpgrade>& AbailableUpgrade)
{
	GenerateAbilityUpgradeList();
	AbailableUpgrade = mAbailableUpgrade;
}

void ABasePlayer::UpgradeAbility_Implementation(int32 Action, int32 ActionSub)
{
}

void ABasePlayer::Server_Death_Implementation(AActor* Player)
{
	Super::Server_Death_Implementation(Player);
	Server_RespawnPlayer();
}

void ABasePlayer::Death()
{
	Super::Death();
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
}

void ABasePlayer::RegenManaTimer()
{
	mCurMana = FMath::Clamp(mCurMana+ mManaRegenOverTime,0.f,mMaxMana);
	Server_UpdateHUD();
	if(mCurMana==mMaxMana)
	{
		Server_RegenManaTimerStop();
	}
}

void ABasePlayer::GenerateAbilityUpgradeList()
{
	mAbailableUpgrade.Empty();
	FAbilityUpgrade ability1;
	ability1.ActionName = FText::FromString(TEXT("생성 필요"));
	ability1.ActionDesc = FText::FromString(TEXT("게을러서 이렇게함"));
	ability1.GoldCost = 99999;
	mAbailableUpgrade.Add(ability1);
}

bool ABasePlayer::CanUseMana(float ManaCost, bool ReduceInstant)
{
	if(ManaCost > mCurMana)
	{
		return false;
	}
	if(ReduceInstant)
	{
		mCurMana -= ManaCost;
		Server_UpdateHUD();
		Server_RegenMana();
	}
	return true;
}

void ABasePlayer::Server_Action1_Implementation()
{
}

void ABasePlayer::Server_Action1End_Implementation()
{
}

void ABasePlayer::Server_Action2_Implementation()
{
}

void ABasePlayer::Server_Action2End_Implementation()
{
}

void ABasePlayer::Server_Action3_Implementation()
{
}

void ABasePlayer::Server_Action3End_Implementation()
{
}

void ABasePlayer::Server_Action4_Implementation()
{
}

void ABasePlayer::Server_Action4End_Implementation()
{
}

void ABasePlayer::Server_UpdateHUD_Implementation()
{
	if (!IsValid(mPlayerController))
	{
		FTimerHandle tempTimer;
		GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::Server_UpdateHUD, 0.2f);
		return;
	}
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_UpdatePlayerHUD(mPlayerController, mCurHealth / mMaxHealth, mCurMana / mMaxMana);
	}
}

void ABasePlayer::Server_RespawnPlayer_Implementation()
{
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::RespawnPlayer, 2.f);
}

void ABasePlayer::RespawnPlayer()
{
	Destroy();
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_PlayerRespawn(mPlayerController, mInitialSpawnLoc, mCharacterClass);
	}
}

void ABasePlayer::Server_RegenMana_Implementation()
{
	if(mCurMana>=mMaxMana||
		mManaRegenTimer.IsValid())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(mManaRegenTimer,this,&ThisClass::RegenManaTimer,0.01f,true);
}

void ABasePlayer::Server_RegenManaTimerStop_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(mManaRegenTimer);
}
