// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

class UScrollBox;
class AMasterItem;
class UItemSlotBase;
/**
 * 
 */
UCLASS()
class PROJECTPG_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UScrollBox* ItemSlots;

	void UpdateInventory(TArray<AMasterItem*> Inventory);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemSlotBase> ItemSlotClass;

	//int32 GetEmptySlot();
	//void SetSlot(int32 Index, AMasterItem* Item);

};
