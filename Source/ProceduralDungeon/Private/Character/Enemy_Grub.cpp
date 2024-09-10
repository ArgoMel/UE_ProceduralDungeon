// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy_Grub.h"

AEnemy_Grub::AEnemy_Grub()
{
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
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Swordsman(TEXT(
	//	"/Game/_Main/Player/Swordsman/ABP_Swordsman.ABP_Swordsman_C"));
	//if (Greystone_Novaborn.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ABP_Swordsman.Class);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed GetClassAsset : %s"), *GetNameSafe(this));
	//}
}

ABaseEnemy* AEnemy_Grub::GetEnemyRef_Implemantation()
{
	return this;
}
