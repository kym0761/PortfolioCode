// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BossObject.Generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FBossProjectileInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Cooldown;
};

UENUM(BlueprintType)
enum class EHandState : uint8
{
	Open = 0		UMETA(Display = "Open"),
	CanGrab = 1		UMETA(Display = "CanGrab"),
	Grab = 2		UMETA(Display = "Grab"),
};