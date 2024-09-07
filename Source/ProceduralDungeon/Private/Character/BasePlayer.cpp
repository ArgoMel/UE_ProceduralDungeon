// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayer.h"
#include "PC_DungeonGame.h"
#include "Input/DungeonGameIAs.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABasePlayer::ABasePlayer()
{
	bUseControllerRotationYaw = false;

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->SetupAttachment(RootComponent);
	mSpringArm->SetRelativeRotation(FRotator(-60.f,0.f,0.f));
	mSpringArm->TargetArmLength=1250.f;
	mSpringArm->bInheritPitch=false;
	mSpringArm->bInheritRoll=false;
	mSpringArm->bInheritYaw=false;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);

	GetCharacterMovement()->RotationRate=FRotator(0.f,720.f,0.f);
	GetCharacterMovement()->bOrientRotationToMovement=true;
}

void ABasePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bCurrentlyAttacking);
}

void ABasePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	mPlayerController = Cast<APC_DungeonGame>(NewController);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!mPlayerController)
	{
		mPlayerController = Cast<APC_DungeonGame>(Controller);
	}
	if (!input || !mPlayerController)
	{
		return;
	}
	const UDungeonGameIAs* dungeonGameIAs = mPlayerController->GetDungeonGameIAs();
	if (!dungeonGameIAs)
	{
		return;
	}
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction1, this, &ThisClass::Server_Action1, &ThisClass::Server_Action1End, &ThisClass::Server_Action1End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction2, this, &ThisClass::Server_Action2, &ThisClass::Server_Action2End, &ThisClass::Server_Action2End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction3, this, &ThisClass::Server_Action3, &ThisClass::Server_Action3End, &ThisClass::Server_Action3End);
	EDungeonGameIAs::BindInput_StartCompleteCancel(input, dungeonGameIAs->mAction4, this, &ThisClass::Server_Action4, &ThisClass::Server_Action4End, &ThisClass::Server_Action4End);
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
