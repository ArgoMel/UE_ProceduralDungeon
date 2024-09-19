// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"

#define PROFILENAME_RAGDOLL TEXT("ragdoll")
#define PROFILENAME_PAWN TEXT("Pawn")
#define PROFILENAME_BLINK TEXT("Blink")
#define PROFILENAME_ENEMYATTACK TEXT("EnemyAttack")
#define PROFILENAME_PLAYERATTACK TEXT("PlayerAttack")
#define PROFILENAME_REACTONLYPAWN TEXT("ReactOnlyPawn")
#define PROFILENAME_NOCOLLISION TEXT("NoCollision")

#define TAG_PLAYER TEXT("Player")
#define TAG_ENEMY TEXT("Enemy")

#define MAP_FIRSTLEVEL TEXT("Level_01")
#define MAP_MAINMENU TEXT("MainMenu")

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Gold,
	HealthPotion,
	ManaPotion,
	Max UMETA(Hidden)
};

#pragma region ValidMecro
#define NULLCHECK(POINTER)\
if (!POINTER)\
{\
	return;\
}

#define VALIDCHECK(POINTER)\
if (!IsValid(POINTER))\
{\
	UE_LOG(LogTemp, Warning, TEXT("%s 유효하지 않음"), *POINTER->GetFName().ToString());\
	return;\
}

#define VALIDCHECK_RETURNVALUE(POINTER,VALUE)\
if (!IsValid(POINTER))\
{\
	UE_LOG(LogTemp, Warning, TEXT("%s 유효하지 않음"), *POINTER->GetFName().ToString());\
	return VALUE;\
}
#pragma endregion

#pragma region LoadAsset
/*    Object Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 오브젝트의 클래스
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Object;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetObjectAsset : %s"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}

#define AddObjectAsset(RETURN_POINTER,INDEX, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> RETURN_POINTER##INDEX(TEXT(PATH));\
if (RETURN_POINTER##INDEX.Succeeded()) \
{\
    RETURN_POINTER.Add(RETURN_POINTER##INDEX.Object);\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed AddObjectAsset : %s"),TEXT(PATH));\
}
/*    Class Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 클래스 타입
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetClassAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FClassFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Class;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetClassAsset : %s"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}
#pragma endregion