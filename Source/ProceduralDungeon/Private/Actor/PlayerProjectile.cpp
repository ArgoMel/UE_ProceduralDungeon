// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/PlayerProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>

APlayerProjectile::APlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	mSpeed = 0.f;
	mDamage = 0.f;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(mBox);
	mBox->SetBoxExtent(FVector(32.f,32.f,80.f));
	mBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mBox->SetCollisionProfileName(PROFILENAME_PLAYERATTACK);

	mParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PariticleSystem"));
	mParticleSystem->SetupAttachment(mBox);

	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	mProjectileMovement->ProjectileGravityScale=0.f;
}

void APlayerProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mSpeed);
	DOREPLIFETIME(ThisClass, mDamage);
}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	mBox->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnProjectileBeginOverlap);
}

void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APlayerProjectile::SetSpeed(float Speed)
{
	mSpeed = Speed;
	mProjectileMovement->MaxSpeed = mSpeed;
	mProjectileMovement->InitialSpeed = mSpeed;
}

void APlayerProjectile::Multi_SpawnParticle_Implementation(UParticleSystem* EmitterTemplate, FVector Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, Location);
}

