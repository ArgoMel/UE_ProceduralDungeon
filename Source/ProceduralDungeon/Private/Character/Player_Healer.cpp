// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player_Healer.h"
#include "Actor/HealerProjectile.h"
#include "Interface/INT_PlayerController.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Engine/DamageEvents.h"
#include <Kismet/KismetMathLibrary.h>

APlayer_Healer::APlayer_Healer()
{
	bUseAutoCollision = false;

	mMaxHealth = 50.f;
	mCurHealth = mMaxHealth;

	mCharacterClass = GetClass();
	mMaxMana = 200.f;
	mCurMana = mMaxMana;
	mAction1ManaCost = 0.f;
	mAction2ManaCost = 25.f;
	mAction3ManaCost = 1.25f;
	mAction4ManaCost = 30.f;
	mAction1Damage = 5.f;
	mAction2Damage = 25.f;
	mAction3Damage = 1.f;
	mAction4Damage = 20.f;

	bConcentrating = false;


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
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Healer(TEXT(
	//	"/Game/_Main/Player/Healer/ABP_Healer.ABP_Healer_C"));
	//if (ABP_Healer.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ABP_Healer.Class);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed GetClassAsset : ABP_Healer"));
	//}
}

void APlayer_Healer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bConcentrating);
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

void APlayer_Healer::Server_Action2_Implementation()
{
	Super::Server_Action2_Implementation();
	if (bCurrentlyAttacking ||
		bIsDead ||
		!CanUseMana(mAction2ManaCost))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	Multi_PlayMontage(mAction2Montage);
}

void APlayer_Healer::Server_Action3_Implementation()
{
	Super::Server_Action3_Implementation();
	if (bCurrentlyAttacking ||
		bIsDead ||
		!CanUseMana(mAction3ManaCost,false))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	bConcentrating = true;
	GetWorld()->GetTimerManager().SetTimer(mConcentrateTimer,this,&ThisClass::ConcentrateHandle,0.25f,true);
}

void APlayer_Healer::Server_Action3End_Implementation()
{
	bConcentrating = false;
	GetWorld()->GetTimerManager().ClearTimer(mConcentrateTimer);
}

void APlayer_Healer::Server_Action4_Implementation()
{
	Super::Server_Action4_Implementation();
	if (bCurrentlyAttacking ||
		bIsDead ||
		!CanUseMana(mAction4ManaCost))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	Multi_PlayMontage(mAction4Montage);
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

void APlayer_Healer::UseAction2_Implementation()
{
	if (HasAuthority())
	{
		FVector startPos = GetActorLocation();
		float radius = 400.f;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors;
		TArray<AActor*> results;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), startPos, radius, objectTypes, nullptr, ignoreActors, results);
		for (auto& result : results)
		{
			if (!result->ActorHasTag(TAG_PLAYER)||
				!result->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
			{
				continue;
			}
			IINT_PlayerCharacter::Execute_HealPlayer(result, mAction2Damage);
		}
	}
}

void APlayer_Healer::UseAction4_Implementation()
{
	AActor* closestEnemy = nullptr;
	if (HasAuthority())
	{
		FVector startPos = GetActorLocation();
		float radius = 1500.f;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors = {this};
		TArray<AActor*> results;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), startPos, radius, objectTypes, nullptr, ignoreActors, results);
		for (auto& result : results)
		{
			if (!result->ActorHasTag(TAG_ENEMY))
			{
				continue;
			}
			if(!closestEnemy)
			{
				closestEnemy=result;
			}
			else
			{
				float newDist=FVector::Dist2D(startPos, result->GetActorLocation());
				float oldDist=FVector::Dist2D(startPos, closestEnemy->GetActorLocation());
				if(newDist< oldDist)
				{
					closestEnemy = result;
				}
			}
		}

		if(!closestEnemy)
		{
			return;
		}
		FTransform transtorm = FTransform(UKismetMathLibrary::FindLookAtRotation(startPos, closestEnemy->GetActorLocation()),startPos ,FVector(1.f));
		AHealerProjectile* projectile= GetWorld()->SpawnActorDeferred<AHealerProjectile>(AHealerProjectile::StaticClass(), transtorm);
		if(projectile)
		{
			projectile->Owner = this;
			projectile->mEnemiesToHit = 2;
			projectile->mHealPerHit = 10.f;
			projectile->SetSpeed(1000.f);
			projectile->mDamage = mAction4Damage;
			projectile->FinishSpawning(transtorm);
		}
	}
}

void APlayer_Healer::ConcentrateHandle_Implementation()
{
	if(!bConcentrating||
		!CanUseMana(mAction3ManaCost))
	{
		Server_Action3End();
		return;
	}
	if (HasAuthority())
	{
		FVector startPos = GetActorLocation();
		float radius = 400.f;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors = {this};
		TArray<AActor*> results;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), startPos, radius, objectTypes, nullptr, ignoreActors, results);
		for (auto& result : results)
		{
			if (result->ActorHasTag(TAG_PLAYER))
			{
				continue;
			}
			result->TakeDamage(mAction3Damage, FDamageEvent(), nullptr, this);
		}
	}
}
