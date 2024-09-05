// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABasePlayer::ABasePlayer()
{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->SetupAttachment(RootComponent);
	mSpringArm->SetRelativeRotation(FRotator(-60.f,0.f,0.f));
	mSpringArm->TargetArmLength=1250.f;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);

	GetCharacterMovement()->RotationRate=FRotator(0.f,720.f,0.f);
	GetCharacterMovement()->bOrientRotationToMovement=true;
}
