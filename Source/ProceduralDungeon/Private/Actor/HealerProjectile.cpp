// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealerProjectile.h"
#include "Interface/INT_PlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DamageEvents.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

AHealerProjectile::AHealerProjectile()
{
	GetObjectAsset(mHitParticle,UParticleSystem,"/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_ThunderBallHit.P_ky_ThunderBallHit'")

	mEnemiesToHit = 0;
	mHealPerHit = 0.f;

	mParticleSystem->SetRelativeScale3D(FVector(0.25f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_ky_thunderBall(TEXT(
	"/Game/FXVarietyPack/Particles/P_ky_thunderBall.P_ky_thunderBall"));
	if (P_ky_thunderBall.Succeeded())
	{
		mParticleSystem->SetTemplate(P_ky_thunderBall.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed GetObjectAsset : Serath_Gold"));
	}
}

void AHealerProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mHitEnemies);
	DOREPLIFETIME(ThisClass, mHealPerHit);
	DOREPLIFETIME(ThisClass, mHealPerHit);
}

void AHealerProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()&& OtherActor->ActorHasTag(TAG_ENEMY))
	{
		if(mHitEnemies.Find(OtherActor)==INDEX_NONE)
		{
			FVector startPos = GetActorLocation();
			Multi_SpawnParticle(mHitParticle, startPos);

			mHitEnemies.Add(OtherActor);
			OtherActor->TakeDamage(mDamage, FDamageEvent(), UGameplayStatics::GetPlayerController(GetWorld(), 0), GetOwner());

			if (GetOwner()->GetClass()->ImplementsInterface(UINT_PlayerCharacter::StaticClass()))
			{
				IINT_PlayerCharacter::Execute_HealPlayer(GetOwner(), mHealPerHit);
			}


			if(mEnemiesToHit<=0)
			{
				Destroy();
				return;
			}

			AActor* closestEnemy = nullptr;
			float radius = 1500.f;
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
			TArray<AActor*> results;
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), startPos, radius, objectTypes, nullptr, mHitEnemies, results);
			for (auto& result : results)
			{
				if (!result->ActorHasTag(TAG_ENEMY))
				{
					continue;
				}
				if (!closestEnemy)
				{
					closestEnemy = result;
				}
				else
				{
					float newDist = FVector::Dist2D(startPos, result->GetActorLocation());
					float oldDist = FVector::Dist2D(startPos, closestEnemy->GetActorLocation());
					if (newDist < oldDist)
					{
						closestEnemy = result;
					}
				}
			}

			if (closestEnemy)
			{
				FTransform transtorm = FTransform(UKismetMathLibrary::FindLookAtRotation(startPos, closestEnemy->GetActorLocation()), startPos, FVector(1.f));
				AHealerProjectile* projectile = GetWorld()->SpawnActorDeferred<AHealerProjectile>(AHealerProjectile::StaticClass(), transtorm);
				if (projectile)
				{
					projectile->Owner = GetOwner();
					projectile->mEnemiesToHit = mEnemiesToHit - 1;
					projectile->mHealPerHit = mHealPerHit;
					projectile->SetSpeed(mSpeed);
					projectile->mDamage = mDamage;
					projectile->FinishSpawning(transtorm);
				}
			}
			Destroy();
		}
	}
}
