// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AISpawner.h"
#include "Character/BaseEnemy.h"
#include "Character/Enemy_Grub.h"
#include "Components/SphereComponent.h"
#include <NavigationSystem.h>

AAISpawner::AAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	mSpawnedEnemies = 0;
	mCurrentSpawned = 0;
	bIsSpawning = false;

	mSpawnableEnemies.Add(AEnemy_Grub::StaticClass());
	mTotalEnemiesToSpawn = 10;
	mSpawnAtTime = 2;
	mSpawnTime = 2.f;
	mSpawnRadius = 1000.f;
	bDestroyAfterFinalSpawn = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);
	RootComponent->SetIsReplicated(true);

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mStaticMesh->SetupAttachment(RootComponent);
	mStaticMesh->SetCollisionProfileName(PROFILENAME_NOCOLLISION);

	mStartSpawnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StartSpawnSphere"));
	mStartSpawnSphere->SetupAttachment(mStaticMesh);
	mStartSpawnSphere->SetIsReplicated(true);
	mStartSpawnSphere->SetSphereRadius(400.f);
}

void AAISpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsSpawning);
}

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();	
	mStartSpawnSphere->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnStartSpawnSphereBeginOverlap);
}

void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAISpawner::OnStartSpawnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(!OtherActor->ActorHasTag(TAG_PLAYER)||
			bIsSpawning)
		{
			return;
		}
		bIsSpawning = true;
		SpawnEnemies();
	}
}

void AAISpawner::SpawnEnemyTimer_Implementation()
{
	UNavigationSystemV1* navSystem=UNavigationSystemV1::GetNavigationSystem(GetWorld());
	NULLCHECK(navSystem);
	FNavLocation randLoc;
	if(navSystem->GetRandomPointInNavigableRadius(GetActorLocation(),mSpawnRadius, randLoc))
	{
		int32 randIndex = FMath::RandRange(0, mSpawnableEnemies.Num()-1);
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABaseEnemy* enemy=GetWorld()->SpawnActor<ABaseEnemy>(mSpawnableEnemies[randIndex], randLoc.Location, FRotator::ZeroRotator, param);
		++mSpawnedEnemies;
		++mCurrentSpawned;
		enemy->OnEnemyDies.AddDynamic(this,&ThisClass::HandleSpawnedDeath);
		if (mSpawnedEnemies >= mTotalEnemiesToSpawn)
		{
			StopSpawning();
			SpawnEnemies();
		}
	}
}

void AAISpawner::StopSpawning_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(mSpawnEnemyTimer);
}

void AAISpawner::HandleSpawnedDeath_Implementation(ABaseEnemy* DeadEnemy)
{
	--mCurrentSpawned;
	if(!mSpawnEnemyTimer.IsValid())
	{
		SpawnEnemies();
	}
}

void AAISpawner::SpawnEnemies_Implementation()
{
	if(mSpawnedEnemies >=mTotalEnemiesToSpawn)
	{
		if(bDestroyAfterFinalSpawn)
		{
			Destroy();
		}
		return;
	}
	if (!mSpawnEnemyTimer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(mSpawnEnemyTimer, this, &ThisClass::SpawnEnemyTimer, mSpawnTime, true);
	}
}

