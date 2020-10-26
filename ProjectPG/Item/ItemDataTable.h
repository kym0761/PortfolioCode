// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 
{
	Unknown = 0 UMETA(Display = "Unknown"),
	Consume = 1 UMETA(Display = "Consume"),
	Equip = 2 UMETA(Display = "Equip")
};
UENUM(BlueprintType)
enum class ESlotType :uint8 
{
	NotUse = 0 UMETA(Display = "NotUse"),
	WeaponRightHand = 1 UMETA(Display = "WeaponRightHand"),
	Weapon = 2 UMETA(Display = "Weapon"),
	WeaponBack01 = 3 UMETA(Display = "WeaponBack01")
};

/**
 * 
 */
USTRUCT()
struct PROJECTPG_API FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()//GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType ItemEquipSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemFloat01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UTexture2D> ItemThumnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UStaticMesh> ItemMesh;
};
