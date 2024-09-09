// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player_Swordsman.h"
#include "PC_DungeonGame.h"
#include "Interface/INT_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
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
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(bIsBlocking&&
		PlayerHitCheck(DamageCauser))
	{
		damage = 0.f;
	}
	return damage;
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
			UGameplayStatics::ApplyDamage(result, 15.f, nullptr, this, nullptr);
		}
	}
}

APlayer_Swordsman* APlayer_Swordsman::GetPlayerSwordsmanRef_Implementation()
{
	return this;
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
	mBlinkBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Blink();
	Client_Blink();
}

void APlayer_Swordsman::Server_Action3_Implementation()
{
	Super::Server_Action3_Implementation();
	if (bCurrentlyAttacking || bIsDead)
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
	if (bCurrentlyAttacking || bIsDead)
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
	if(OtherActor==this)
	{
		return;
	}
	if (HasAuthority())
	{
		UGameplayStatics::ApplyDamage(OtherActor, 15.f, nullptr, this, nullptr);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetFName().ToString());
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

bool APlayer_Swordsman::PlayerHitCheck(AActor* DamageCauser) const
{
	FVector dir=(GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
	dir.Z = 0.f;
	dir.Normalize();
	FVector forwardVec = GetActorForwardVector();
	forwardVec.Z = 0.f;
	forwardVec.Normalize();
	return forwardVec.Dot(dir) <= -0.4;
}
