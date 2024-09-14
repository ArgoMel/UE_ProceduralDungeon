// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"

#define PROFILENAME_RAGDOLL TEXT("ragdoll")
#define PROFILENAME_PAWN TEXT("Pawn")
#define PROFILENAME_BLINK TEXT("Blink")
#define PROFILENAME_ENEMYATTACK TEXT("EnemyAttack")
#define PROFILENAME_NOCOLLISION TEXT("NoCollision")

#define TAG_PLAYER TEXT("Player")
#define TAG_ENEMY TEXT("Enemy")

#define MAP_FIRSTLEVEL TEXT("Test")
#define MAP_MAINMENU TEXT("MainMenu")

#pragma region ValidMecro
#define VALIDCHECK(POINTER)\
if (!IsValid(POINTER))\
{\
	UE_LOG(LogTemp, Warning, TEXT("%s ��ȿ���� ����"), *POINTER->GetFName().ToString());\
	return;\
}

#define VALIDCHECK_RETURNVALUE(POINTER,VALUE)\
if (!IsValid(POINTER))\
{\
	UE_LOG(LogTemp, Warning, TEXT("%s ��ȿ���� ����"), *POINTER->GetFName().ToString());\
	return VALUE;\
}
#pragma endregion

#pragma region LoadAsset
/*    Object Asset�� �޽��ϴ�.
*    RETURN_POINTER : ���� ������
*    CLASS_TYPE : ���� ������Ʈ�� Ŭ����
*    PATH : ��� (TEXT�� ������ �ʽ��ϴ�.)*/
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
/*    Class Asset�� �޽��ϴ�.
*    RETURN_POINTER : ���� ������
*    CLASS_TYPE : ���� Ŭ���� Ÿ��
*    PATH : ��� (TEXT�� ������ �ʽ��ϴ�.)*/
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