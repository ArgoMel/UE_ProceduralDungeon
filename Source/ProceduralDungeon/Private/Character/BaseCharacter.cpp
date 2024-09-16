// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include <Kismet/GameplayStatics.h>

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseAutoCollision = true;

	mMaxHealth = 0;
	mCurHealth = mMaxHealth;

	bIsDead = false;

	SetRootComponent(GetCapsuleComponent());

	GetMesh()->SetCollisionProfileName(PROFILENAME_NOCOLLISION);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mInitialSpawnLoc);
	DOREPLIFETIME(ThisClass, mCurHealth);
	DOREPLIFETIME(ThisClass, mMaxHealth);
	DOREPLIFETIME(ThisClass, bIsDead);
}

void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(bUseAutoCollision)
	{
		UpdateCollisionCapsuleToFitMesh();
	}
	else
	{
		UpdateRelativeLocationToFitCapsule();
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	mInitialSpawnLoc=GetActorLocation();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::UpdateCollisionCapsuleToFitMesh()
{
	USkeletalMeshComponent* mesh = GetMesh();
	if (!IsValid(mesh))
	{
		UE_LOG(LogTemp, Error, TEXT(
			"Attempted to update collision capsule on pawn %s, but mesh wasn't valid."),*GetNameSafe(this));
		return;
	}
	USkeletalMesh* meshAsset = mesh->GetSkeletalMeshAsset();
	if (!IsValid(meshAsset))
	{
		UE_LOG(LogTemp, Error, TEXT(
			"Attempted to update collision capsule on pawn %s, but meshAsset wasn't valid."),*GetNameSafe(this));
		return;
	}
	const FBoxSphereBounds meshBounds = meshAsset->GetBounds();
	const FVector scale3D = mesh->GetRelativeScale3D();

	// set collision cylinder appropriately to mesh size
	const float newRadius = ((meshBounds.BoxExtent.X + meshBounds.BoxExtent.Y) * 0.5f)
		* 0.5f * FMath::Max(scale3D.X, scale3D.Y);
	// conversion factor based on known good capsule height from UE3 prototype.
	const float boxHeightConversionFactor = 1.324f;
	const float newHalfHeight = meshBounds.SphereRadius * 0.5f * boxHeightConversionFactor * scale3D.Z;

	// set the currently in-use cylinders
	GetCapsuleComponent()->SetCapsuleSize(newRadius, newHalfHeight-2.f);
	//GetCapsuleComponent()->SetCapsuleSize(newRadius, meshBounds.BoxExtent.Z - 4.f);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.));

	UpdateRelativeLocationToFitCapsule();
}

void ABaseCharacter::UpdateRelativeLocationToFitCapsule() const
{	
	// Update the Mesh's relative location, taking into account foot-origin vs. center-origin characters.
	FVector newRelativeLocation{ GetMesh()->GetRelativeLocation() };
	newRelativeLocation.Z = -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 2.f);
	GetMesh()->SetRelativeLocation(newRelativeLocation);
}

void ABaseCharacter::OnRep_IsDead()
{
	if(bIsDead)
	{
		HandleDeath();
	}
}

void ABaseCharacter::Multi_PlayMontage_Implementation(UAnimMontage* MontageToPlay)
{
	PlayAnimMontage(MontageToPlay);
}

void ABaseCharacter::Multi_PlaySFX_Implementation(USoundBase* Sound, FVector Loc)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Loc);
}

void ABaseCharacter::Multi_SpawnParticle_Implementation(UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation, FVector Scale)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, Location, Rotation, Scale);
}

void ABaseCharacter::Server_Death_Implementation()
{
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	Client_Death();
	Death();
}

void ABaseCharacter::Client_Death_Implementation()
{
	Death();
}

void ABaseCharacter::Death()
{
}
