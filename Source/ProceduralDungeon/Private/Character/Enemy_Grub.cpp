// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy_Grub.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy_Grub::AEnemy_Grub()
{
	GetObjectAsset(mMeleeAttackMontage, UAnimMontage, "/Game/_Main/Enemy/Grub/Grub_Clot_Worm_React_Heavy_Front_Montage.Grub_Clot_Worm_React_Heavy_Front_Montage");

	GetObjectAsset(mDeathSound, USoundBase, "/Game/_Main/Enemy/Grub/GrubDeath_Cue.GrubDeath_Cue");

	mDropPercent = 50.f;

	GetMesh()->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.25f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Enemy_Clot_Worm(TEXT(
		"/Game/InfinityBladeAdversaries/Enemy/Enemy_Clot_Worm/SK_Enemy_Clot_Worm.SK_Enemy_Clot_Worm"));
	if (SK_Enemy_Clot_Worm.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Enemy_Clot_Worm.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed GetObjectAsset : %s"), *GetNameSafe(this));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_EnemyGrub(TEXT(
		"/Game/_Main/Enemy/Grub/ABP_EnemyGrub.ABP_EnemyGrub_C"));
	if (ABP_EnemyGrub.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_EnemyGrub.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed GetClassAsset : %s"), *GetNameSafe(this));
	}

	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}
