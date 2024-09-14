// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player_Healer.h"
#include "Interface/INT_PlayerController.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Engine/DamageEvents.h"

APlayer_Healer::APlayer_Healer()
{
	bUseAutoCollision = false;

	mMaxHealth = 50.f;
	mCurHealth = mMaxHealth;

	mCharacterClass = GetClass();
	mMaxMana = 200.f;
	mCurMana = mMaxMana;
	mAction1ManaCost = 0.f;
	mAction2ManaCost = 0.f;
	mAction3ManaCost = 0.f;
	mAction4ManaCost = 0.f;
	mAction1Damage = 5.f;
	mAction2Damage = 0.f;
	mAction2Damage = 0.f;
	mAction4Damage = 0.f;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SerathGold(TEXT(
	//	"/Game/ParagonSerath/Characters/Heroes/Serath/Skins/Tier1/Serath_Gold/Meshes/SerathGold.SerathGold"));
	//if (SerathGold.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SerathGold.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed GetObjectAsset : Serath_Gold"));
	//}
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Player_Healer(TEXT(
	//	"/Game/_Main/Player/Healer/ABP_Player_Healer.ABP_Player_Healer_C"));
	//if (ABP_Player_Healer.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ABP_Player_Healer.Class);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed GetClassAsset : ABP_Player_Healer"));
	//}
}

void APlayer_Healer::Server_Action1_Implementation()
{
	Super::Server_Action1_Implementation();
	if (bCurrentlyAttacking ||
		bIsDead ||
		!CanUseMana(mAction1ManaCost))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	Multi_PlayMontage(mAction1Montages[FMath::RandRange(0, mAction1Montages.Num() - 1)]);
}

void APlayer_Healer::UseAction1_Implementation()
{
	if (HasAuthority())
	{
		FVector boxPos = GetActorLocation() + GetActorForwardVector() * 150.f;
		FVector boxExtent = FVector(90.f, 50.f, 100.f);
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors = { this };
		TArray<AActor*> results;
		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), boxPos, boxExtent, objectTypes, nullptr, ignoreActors, results);
		for (auto& result : results)
		{
			if (result->ActorHasTag(TAG_PLAYER))
			{
				continue;
			}
			result->TakeDamage(mAction1Damage, FDamageEvent(), nullptr, this);
		}
	}
}
