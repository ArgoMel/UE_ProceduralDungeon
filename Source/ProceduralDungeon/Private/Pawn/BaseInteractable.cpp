// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/BaseInteractable.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	mCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(mCapsule);

	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mSphere->SetupAttachment(mCapsule);
	mSphere->SetSphereRadius(150.f);
	mSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mSphere->SetCollisionProfileName(PROFILENAME_REACTONLYPAWN);
	mSphere->SetIsReplicated(true);

	mSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	mSkeletalMesh->SetupAttachment(mCapsule);
	mSkeletalMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mSkeletalMesh->SetCollisionProfileName(PROFILENAME_NOCOLLISION);
}

void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	mSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
}

void ABaseInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseInteractable::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(!OtherActor->ActorHasTag(TAG_PLAYER))
		{
			return;
		}
		if (OtherActor->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
		{
			IINT_PlayerCharacter::Execute_UpdateUpgradeScreen(OtherActor,true);
		}
	}
}

void ABaseInteractable::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		if (!OtherActor->ActorHasTag(TAG_PLAYER))
		{
			return;
		}
		if (OtherActor->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
		{
			IINT_PlayerCharacter::Execute_UpdateUpgradeScreen(OtherActor, false);
		}
	}
}

