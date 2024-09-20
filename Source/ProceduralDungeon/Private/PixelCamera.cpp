#include "PixelCamera.h"

APixelCamera::APixelCamera()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APixelCamera::BeginPlay()
{
	Super::BeginPlay();
}

void APixelCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


