// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidgetBase.generated.h"

class USpawnSlotBase;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UBossWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//Horizontal Box Has Spawn Slots.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot")
	UHorizontalBox* SlotBox;

	//Spawn Slot Array, Be Initialized in NativeContruct.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot")
	TArray<USpawnSlotBase*> SpawnSlots;

	virtual void NativeConstruct() override;

};
