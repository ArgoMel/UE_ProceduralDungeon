// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/MainMenuCharacter.h"
#include "Camera/CameraComponent.h"

AMainMenuCharacter::AMainMenuCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(GetRootComponent());
	mCamera->bUsePawnControlRotation=true;
}

void AMainMenuCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
