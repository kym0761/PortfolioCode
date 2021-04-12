// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemDataTable.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotBase.generated.h"

UENUM(BlueprintType)
enum class ESlotParentType : uint8
{
	Inventory = 0		UMETA(Display = "Inventory"),
	Equipment = 1		UMETA(Display = "Equipment"),
	Shop = 2			UMETA(Display = "Shop"),
};

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UItemSlotBase : public UUserWidget
{
	GENERATED_BODY()

#pragma region Public Member
public:	   
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UItemWidgetBase* ItemWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UInventoryWidgetBase* InvenWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlotParentType SlotType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCanvasPanel* MainUIRootCanvas;

#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;
	void UpdateItemSlot(class AMasterItem* Item);
	void SetInvenParent(class UInventoryWidgetBase* InvenParent);
	void SetMainUIRootCanvas(class UCanvasPanel* Canvas);
	FItemDataTable GetThisItemData();

	bool GetisDraging();
	void TooltipVisible(bool bValue);

	UFUNCTION(BlueprintCallable)
	void SetisDraging(bool bValue);

	UFUNCTION(BlueprintCallable)
	void AddItemOnInventory(int Index, class AMasterItem* Item);

	UFUNCTION(BlueprintCallable)
	void SwapInvenWithEquip(int InvenslotIndex, int EquipslotIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipAddInven(int InvenslotIndex, int EquipslotIndex);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

#pragma endregion

};
