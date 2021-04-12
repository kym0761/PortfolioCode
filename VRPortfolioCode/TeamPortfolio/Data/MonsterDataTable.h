// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterDataTable.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Zombie0 = 0		UMETA(Display = "Zombie0"),
	Zombie1 = 1		UMETA(Display = "Zombie1"),
	Zombie2 = 2		UMETA(Display = "Zombie2"),
	Zombie3 = 3		UMETA(Display = "Zombie3"),
	Zombie4 = 4		UMETA(Display = "Zombie4"),
	Zombie5 = 5		UMETA(Display = "Zombie5"),
	Zombie6 = 6		UMETA(Display = "Zombie6"),
	Zombie7 = 7		UMETA(Display = "Zombie7"),
	Zombie8 = 8		UMETA(Display = "Zombie8"),
	DataMax = 9		UMETA(Display = "Max"),
};

/**
 *
 */
USTRUCT(BlueprintType)
struct TEAMPORTFOLIO_API FMonsterDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EMonsterType MonsterID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MonsterName;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UTexture2D>	MonsterThumnail;*/
};