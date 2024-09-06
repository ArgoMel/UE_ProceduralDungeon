// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"

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

#define AddObjectAsset(RETURN_POINTER,NAME, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> RETURN_POINTER##NAME(TEXT(PATH));\
if (RETURN_POINTER##NAME.Succeeded()) \
{\
    RETURN_POINTER.Add(RETURN_POINTER##NAME.Object);\
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