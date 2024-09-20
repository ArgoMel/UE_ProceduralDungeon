// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCameraSystem.h"
#include "Camera/CameraComponent.h"

#define LOW_LES_TEXTURE TEXT("LowLesTex")

APixelCameraSystem::APixelCameraSystem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PixelCameraSystem"));
	SetRootComponent(RootComponent);

	mPixelCameraTransform = CreateDefaultSubobject<USceneComponent>(TEXT("PixelCameraTransform"));
	mPixelCameraTransform->SetupAttachment(RootComponent);

	mPixelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PixelCamera"));
	mPixelCamera->SetupAttachment(RootComponent);

	mUpscaledDisplay = CreateDefaultSubobject<USceneComponent>(TEXT("UpscaledDisplay"));
	mUpscaledDisplay->SetupAttachment(mPixelCamera);

	mViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	mViewCamera->SetupAttachment(mUpscaledDisplay);

	mQuadCanvas = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuadCanvas"));
	mQuadCanvas->SetupAttachment(mUpscaledDisplay);
	mQuadCanvas->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_QuadCanvas(TEXT("/Pixel3D/Mesh/SM_QuadCanvas.SM_QuadCanvas"));
	if (SM_QuadCanvas.Succeeded()) 
	{
		mQuadCanvas->SetStaticMesh(SM_QuadCanvas.Object);
	}
	else 
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed GetObjectAsset : %s"), *mQuadCanvas->GetFName().ToString());
	}
}

void APixelCameraSystem::BeginPlay()
{
	Super::BeginPlay();	
	for (int32 i = 0; i < mQuadCanvas->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* mtrl = mQuadCanvas->CreateDynamicMaterialInstance(i);
		mMaterialArray.Add(mtrl);
	}
}

void APixelCameraSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APixelCameraSystem::ResizeCanvas(float Aspect, float CanvasHeight)
{
	mQuadCanvas->SetRelativeScale3D(FVector(CanvasHeight * Aspect, CanvasHeight, 1.f));
}

void APixelCameraSystem::SetCanvasRenderTexture(UTexture* RenderTexture)
{
	for (auto& mtrl : mMaterialArray)
	{
		mtrl->SetTextureParameterValue(LOW_LES_TEXTURE, RenderTexture);
	}
}

