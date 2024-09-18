// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player_Swordsman.h"
#include "Interface/INT_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DamageEvents.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

constexpr float BlinkDistance = 2000.f;

APlayer_Swordsman::APlayer_Swordsman()
{
	PrimaryActorTick.bCanEverTick = true;

	mMaxHealth = 100.f;
	mCurHealth = mMaxHealth;

	mCharacterClass = GetClass();
	mMaxMana=100.f;
	mCurMana= mMaxMana;
	mAction1ManaCost = 0.f;
	mAction2ManaCost = 25.f;
	mAction3ManaCost = 0.f;
	mAction4ManaCost = 25.f;
	mAction1Damage = 5.f;
	mAction2Damage = 10.f;
	mAction2Damage = 0.f;
	mAction4Damage = 25.f;

	AddObjectAsset(mAction1Montages, 0,UAnimMontage,"/Game/_Main/Player/Swordsman/Swordsman_Attack_A_Fast_Montage.Swordsman_Attack_A_Fast_Montage");
	AddObjectAsset(mAction1Montages, 1, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_B_Fast_Montage.Swordsman_Attack_B_Fast_Montage");
	AddObjectAsset(mAction1Montages, 2, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_C_Fast_Montage.Swordsman_Attack_C_Fast_Montage");
	AddObjectAsset(mAction1Montages, 3, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Attack_D_Fast_Montage.Swordsman_Attack_D_Fast_Montage");
	GetObjectAsset(mAction4Montage, UAnimMontage, "/Game/_Main/Player/Swordsman/Swordsman_Ability_R_Montage.Swordsman_Ability_R_Montage");

	GetObjectAsset(mAction4Particle, UParticleSystem, "/Game/_Main/Player/Swordsman/P_Swordsman_Greystone_Novaborn_LeapAOE_Constellations.P_Swordsman_Greystone_Novaborn_LeapAOE_Constellations");

	GetObjectAsset(mLerpCurve, UCurveFloat, "/Game/_Main/Player/Swordsman/C_Blink.C_Blink");

	bIsBlinking = false;
	bIsBlocking = false;

	mAction4ParticleLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("Action4ParticleLoc"));
	mAction4ParticleLoc->SetupAttachment(RootComponent);
	mAction4ParticleLoc->SetRelativeLocation(FVector(40.f, -200.f, -80.f));

	mBlinkBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlinkBox"));
	mBlinkBox->SetupAttachment(RootComponent);
	mBlinkBox->SetRelativeLocation(FVector(65.f, 0.f, 35.f));
	mBlinkBox->SetRelativeScale3D(FVector(1.f, 1.75f, 5.f));
	mBlinkBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mBlinkBox->SetCollisionProfileName(PROFILENAME_BLINK);

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
	if (ABP_Swordsman.Succeeded())
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
	DOREPLIFETIME(ThisClass, bIsBlinking);
	DOREPLIFETIME(ThisClass, bIsBlocking);
}

void APlayer_Swordsman::BeginPlay()
{
	Super::BeginPlay();
	mBlinkBox->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBlinkBoxBeginOverlap);

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

float APlayer_Swordsman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(bIsBlocking&&
		PlayerBlockCheck(DamageCauser))
	{
		return 0.f;
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayer_Swordsman::GenerateAbilityUpgradeList()
{
	mAbailableUpgrade.Empty();
	FAbilityUpgrade ability1;
	ability1.Action = 1;
	ability1.ActionSub = 0;
	ability1.ActionName = FText::FromString(TEXT("소드 스윙"));
	ability1.ActionDesc = FText::FromString(TEXT("데미지 10 증가"));
	ability1.GoldCost = 25;
	mAbailableUpgrade.Add(ability1);

	FAbilityUpgrade ability2_0;
	ability2_0.Action = 2;
	ability2_0.ActionSub = 0;
	ability2_0.ActionName = FText::FromString(TEXT("순간이동"));
	ability2_0.ActionDesc = FText::FromString(TEXT("마나 소모량 감소"));
	ability2_0.GoldCost = 100;
	mAbailableUpgrade.Add(ability2_0);

	FAbilityUpgrade ability2_1;
	ability2_1.Action = 2;
	ability2_1.ActionSub = 1;
	ability2_1.ActionName = FText::FromString(TEXT("순간이동"));
	ability2_1.ActionDesc = FText::FromString(TEXT("데미지 5 증가"));
	ability2_1.GoldCost = 50;
	mAbailableUpgrade.Add(ability2_1);

	FAbilityUpgrade ability3_0;
	ability3_0.Action =4;
	ability3_0.ActionSub = 0;
	ability3_0.ActionName = FText::FromString(TEXT("그라운드 스매쉬"));
	ability3_0.ActionDesc = FText::FromString(TEXT("마나 소모량 감소"));
	ability3_0.GoldCost = 200;
	mAbailableUpgrade.Add(ability3_0);

	FAbilityUpgrade ability3_1;
	ability3_1.Action = 4;
	ability3_1.ActionSub = 1;
	ability3_1.ActionName = FText::FromString(TEXT("그라운드 스매쉬"));
	ability3_1.ActionDesc = FText::FromString(TEXT("데미지 20 증가"));
	ability3_1.GoldCost = 250;
	mAbailableUpgrade.Add(ability3_1);
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
			if(result->ActorHasTag(TAG_PLAYER))
			{
				continue;
			}
			result->TakeDamage(mAction1Damage, FDamageEvent(), UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
		}
	}
}

void APlayer_Swordsman::UseAction4_Implementation()
{
	if (HasAuthority())
	{
		Multi_SpawnParticle(mAction4Particle, mAction4ParticleLoc->GetComponentLocation(), mAction4ParticleLoc->GetComponentRotation());

		FVector spherePos = GetActorLocation();
		float sphereRadius = 350.f;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECC_Pawn) };
		TArray<AActor*> ignoreActors = { this };
		TArray<AActor*> results;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), spherePos, sphereRadius, objectTypes, nullptr, ignoreActors, results);
		for (auto& result : results)
		{
			if (result->ActorHasTag(TAG_PLAYER))
			{
				continue;
			}
			result->TakeDamage(mAction4Damage, FDamageEvent(), UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
		}
	}
}

APlayer_Swordsman* APlayer_Swordsman::GetPlayerSwordsmanRef_Implementation()
{
	return this;
}

void APlayer_Swordsman::UpgradeAbility_Implementation(int32 Action, int32 ActionSub)
{
	switch (Action)
	{
	case 1:
		mAction1Damage += 10.f;
		break;
	case 2:
		switch (ActionSub)
		{
		case 0:
			mAction2ManaCost=FMath::Clamp(mAction2ManaCost-2.5f,0.f,TNumericLimits<float>::Max());
			break;
		case 1:
			mAction2Damage += 5.f;
			break;
		}
		break;
	case 3:
		break;
	case 4:
		switch (ActionSub)
		{
		case 0:
			mAction4ManaCost = FMath::Clamp(mAction4ManaCost - 2.5f, 0.f, TNumericLimits<float>::Max());
			break;
		case 1:
			mAction4Damage += 20.f;
			break;
		}
		break;
	}
}

void APlayer_Swordsman::Server_Action1_Implementation()
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
	Multi_PlayMontage(mAction1Montages[FMath::RandRange(0, mAction1Montages.Num()-1)]);
}

void APlayer_Swordsman::Server_Action2_Implementation()
{
	Super::Server_Action2_Implementation();
	if (bCurrentlyAttacking || 
		bIsDead||
		!CanUseMana(mAction2ManaCost))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	mBlinkBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Blink();
	Client_Blink();
}

void APlayer_Swordsman::Server_Action3_Implementation()
{
	Super::Server_Action3_Implementation();
	if (bCurrentlyAttacking ||
		bIsDead ||
		!CanUseMana(mAction3ManaCost))
	{
		return;
	}
	bCurrentlyAttacking = true;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, false);
	}
	bIsBlocking = true;
}

void APlayer_Swordsman::Server_Action3End_Implementation()
{
	Super::Server_Action3End_Implementation();
	if (!bIsBlocking || bIsDead)
	{
		return;
	}
	bCurrentlyAttacking = false;
	if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, true);
	}
	bIsBlocking = false;
}

void APlayer_Swordsman::Server_Action4_Implementation()
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

void APlayer_Swordsman::Client_Blink_Implementation()
{
	Blink();
}

void APlayer_Swordsman::Blink()
{
	bIsBlinking = true;
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

void APlayer_Swordsman::OnBlinkBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(TAG_PLAYER))
	{
		return;
	}
	if (HasAuthority())
	{
		OtherActor->TakeDamage(mAction2Damage, FDamageEvent(), UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}
}

void APlayer_Swordsman::UpdateBlinkLerpCurve(float Value)
{
	FVector loc=FMath::Lerp(mStartBlinkLoc, mEndBlinkLoc,Value);
	SetActorLocation(loc);
}

void APlayer_Swordsman::CompleteBlink()
{
	GetCapsuleComponent()->SetCollisionProfileName(PROFILENAME_PAWN);
	bIsBlinking = false;
	if (HasAuthority())
	{
		bCurrentlyAttacking = false;
		if (mPlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
		{
			IINT_PlayerController::Execute_SetPlayerCanMove(mPlayerController, true);
		}
		mBlinkBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool APlayer_Swordsman::PlayerBlockCheck(AActor* DamageCauser) const
{
	FVector dir=(GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
	dir.Z = 0.f;
	dir.Normalize();
	FVector forwardVec = GetActorForwardVector();
	forwardVec.Z = 0.f;
	forwardVec.Normalize();
	return forwardVec.Dot(dir) <= -0.4;
}
