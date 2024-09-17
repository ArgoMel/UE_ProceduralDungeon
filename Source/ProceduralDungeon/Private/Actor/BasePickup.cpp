// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/BasePickup.h"
#include "Character/BasePlayer.h"
#include "Interface/INT_PlayerController.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include <Kismet/GameplayStatics.h>

ABasePickup::ABasePickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	mPickupType = EPickupType::Gold;
	mPickupAmount = 0;

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(mStaticMesh);
	mStaticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mStaticMesh->SetCollisionProfileName(PROFILENAME_NOCOLLISION);

	mPickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	mPickupSphere->SetupAttachment(mStaticMesh);
	mPickupSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mPickupSphere->SetCollisionProfileName(PROFILENAME_REACTONLYPAWN);
	mPickupSphere->SetIsReplicated(true);
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	mPickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPickupSphereBeginOverlap);
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePickup::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (!OtherActor->ActorHasTag(TAG_PLAYER))
		{
			return;
		}
		switch (mPickupType)
		{
		case EPickupType::Gold:
			GoldPickup(OtherActor);
			break;
		case EPickupType::HealthPotion:
			HealthPickup(OtherActor);
			break;
		case EPickupType::ManaPotion:
			break;
		}
	}
}

void ABasePickup::GoldPickup_Implementation(AActor* Player)
{
	APlayerController* pc=UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (pc->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_AddGold(pc, mPickupAmount);
	}
	Destroy();
}

void ABasePickup::HealthPickup_Implementation(AActor* Player)
{
	if (Player->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
	{
		ABasePlayer* player= IINT_PlayerCharacter::Execute_GetPlayerRef(Player);
		if(player->GetCurHP()>=player->GetMaxHP())
		{
			return;
		}
		IINT_PlayerCharacter::Execute_HealPlayer(Player, (float)mPickupAmount);
	}
	Destroy();
}

