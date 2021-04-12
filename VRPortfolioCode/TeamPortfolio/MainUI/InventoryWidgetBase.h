// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../TeamPortfolio.h"
#include "../Item/ItemDataTable.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWrapBox* ItemSlots;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBorder* Drag;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton* Exit;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UMainUIBase* MainUIParent;

#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;
	void UpdateInventory(TArray<class AMasterItem*> Inventory);
	virtual void UpdateInventoryWithIndex(TArray<class AMasterItem*> Inventory, int32 FirstIndex);

	UFUNCTION(BlueprintCallable)
	int GetEmptySlot();

	
	virtual void SetSlot(int Index, class AMasterItem* Item);
	virtual void SetSlotsParent();
	void SetMainUIParent(class UMainUIBase* MainUI);
	bool GetisDraging();
	void SetisDraging(bool bValue);

	int HaveThis(class AMasterItem* FuncItem);

	FItemDataTable GetItemData(int32 Index);
	class UItemTooltipBase* GetTooltipWidget();

	UFUNCTION(BlueprintCallable)
	void SwapSlot(int32 FrontslotIndex, int32 OtherSlotIndex);

	void AddItem_Inventory(TArray<class AMasterItem*> Inventory, class AMasterItem* Item);

	UFUNCTION()
	void OnExitButton();
	virtual void SetExitButton();

#pragma endregion
};
