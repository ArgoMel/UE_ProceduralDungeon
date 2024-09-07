// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player_Swordsman.h"
#include "PC_DungeonGame.h"
#include "Interface/INT_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

constexpr float BlinkDistance = 2000.f;

APlayer_Swordsman::APlayer_Swordsman()
{
	PrimaryActorTick.bCanEverTick = true;

	AddObjectAsset(mAction1Montages, Swordsman_Attack_A_Fast_Montage,UAnimMontage,"/Game/_Main/Player/Swordsman/Swordsman_Attack_A_Fast_Montage.Swordsman_Attack_A_Fast_Montage");
	AddObjectAsset(mAction1Montages, Swordsman_Attack_B_Fast_Montage, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_B_Fast_Montage.Swordsman_Attack_B_Fast_Montage");
	AddObjectAsset(mAction1Montages, Swordsman_Attack_C_Fast_Montage, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_C_Fast_Montage.Swordsman_Attack_C_Fast_Montage");
	AddObjectAsset(mAction1Montages, Swordsman_Attack_D_Fast_Montage, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_D_Fast_Montage.Swordsman_Attack_D_Fast_Montage");

	GetObjectAsset(mLerpCurve, UCurveFloat,"/Game/_Main/Player/Swordsman/C_Blink.C_Blink");

	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Greystone_Novaborn(TEXT(
		"/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn"));
	if (Greystone_Novaborn.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Greystone_Novaborn.Object);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed GetObjectAsset : %s"), *GetNameSafe(this));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Swordsman(TEXT(
		"/Game/_Main/Player/Swordsman/ABP_Swordsman.ABP_Swordsman_C"));
	if (Greystone_Novaborn.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Swordsman.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed GetClassAsset : %s"), *GetNameSafe(this));
	}
}

void APlayer_Swordsman::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mStartBlinkLoc);
	DOREPLIFETIME(ThisClass, mEndBlinkLoc);
}

void APlayer_Swordsman::BeginPlay()
{
	Super::BeginPlay();
	FOnTimelineFloat lerpTLF;
	lerpTLF.BindDynamic(this, &ThisClass::UpdateBlinkLerpCurve);
	if (IsValid(mLerpCurve))
	{
		mBlinkTL.AddInterpFloat(mLerpCurve, lerpTLF);
	}
	FOnTimelineEvent OnTimelineFinishedFunc;
	OnTimelineFinishedFunc.BindDynamic(this, &ThisClass::CompleteBlink);
	mBlinkTL.SetTimelineFinishedFunc(OnTimelineFinishedFunc);
}

void APlayer_Swordsman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (mBlinkTL.IsPlaying() || mBlinkTL.IsReversing())
	{
		mBlinkTL.TickTimeline(DeltaTime);
	}
}

void APlayer_Swordsman::UseAction1_Implementation()
{
	if(HasAuthority())
	{
		FVector boxPos=GetActorLocation()+GetActorForwardVector()*150.f;
		FVector boxExtent=FVector(90.f,50.f,100.f);
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors = {this};
		TArray<AActor*> results;
		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), boxPos, boxExtent, objectTypes,nullptr, ignoreActors, results);
		for(auto& result: results)
		{
			UGameplayStatics::ApplyDamage(result,5.f,nullptr,this,nullptr);
		}
	}
}

void APlayer_Swordsman::UseAction2_Implementation()
{
}

void APlayer_Swordsman::UseAction3_Implementation()
{
}

void APlayer_Swordsman::UseAction4_Implementation()
{
}

void APlayer_Swordsman::Server_Action1_Implementation()
{
	Super::Server_Action1_Implementation();
	if(bCurrentlyAttacking||bIsDead)
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	Multi_PlayMontage(mAction1Montages[FMath::RandRange(0, mAction1Montages.Num()-1)]);
}

void APlayer_Swordsman::Server_Action2_Implementation()
{
	Super::Server_Action2_Implementation();
	if (bCurrentlyAttacking || bIsDead)
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	Blink();
	Client_Blink();
}

void APlayer_Swordsman::Server_Action3_Implementation()
{
	Super::Server_Action3_Implementation();
}

void APlayer_Swordsman::Server_Action4_Implementation()
{
	Super::Server_Action4_Implementation();
}

void APlayer_Swordsman::Client_Blink_Implementation()
{
	Blink();
}

void APlayer_Swordsman::Blink()
{
	GetCapsuleComponent()->SetCollisionProfileName(PROFILENAME_RAGDOLL);
	FHitResult result;
	mStartBlinkLoc=GetActorLocation();
	mEndBlinkLoc= mStartBlinkLoc+GetActorForwardVector() * BlinkDistance;
	bool isHit=UKismetSystemLibrary::LineTraceSingle(GetWorld(), mStartBlinkLoc, mEndBlinkLoc, UEngineTypes::ConvertToTraceType(ECC_Visibility),false,TArray<AActor*>(),EDrawDebugTrace::None, result,true);
	if(isHit)
	{
		mEndBlinkLoc = result.ImpactPoint;
	}
	mBlinkTL.PlayFromStart();
}

void APlayer_Swordsman::UpdateBlinkLerpCurve(float Value)
{
	FVector loc=FMath::Lerp(mStartBlinkLoc, mEndBlinkLoc,Value);
	SetActorLocation(loc);
}

void APlayer_Swordsman::CompleteBlink()
{
	GetCapsuleComponent()->SetCollisionProfileName(PROFILENAME_PAWN);
}
