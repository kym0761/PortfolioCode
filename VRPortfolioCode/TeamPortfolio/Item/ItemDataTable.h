// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Unknown = 0		UMETA(Display = "Unknown"),
	Consume = 1		UMETA(Display = "Consume"),
	Equip = 2		UMETA(Display = "Equip"),
};


UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Head = 0		UMETA(Display = "Head"),
	Body = 1		UMETA(Display = "Body"),
	Grove = 2		UMETA(Display = "Grove"),
	Weapon = 3		UMETA(Display = "Weapon"),
	NoneMax = 4		UMETA(Display = "NoneMax"),
};
/**
 *
 */
USTRUCT(BlueprintType)
struct TEAMPORTFOLIO_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlotType ItemEquipSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemFloat01;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UTexture2D> ItemThumnail;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UStaticMesh> ItemMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemContent;

	void SetItemData(FItemDataTable Data);
};
