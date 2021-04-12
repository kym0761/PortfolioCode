// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "EquipmentBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UEquipmentBase : public UInventoryWidgetBase
{
	GENERATED_BODY()
	
#pragma region Public Function
public:
	virtual void NativeConstruct() override;
	virtual void SetSlot(int Index, class AMasterItem* Item) override;
	void UpdateEquipment(TArray<class AMasterItem*> Equipment);

	UFUNCTION()
	void OnExitButtonEquipment();
	virtual void SetExitButton() override;

#pragma endregion
};
