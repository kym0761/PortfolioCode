// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../TeamPortfolio.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UMainUIBase : public UUserWidget
{
	GENERATED_BODY()

#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UHpBarBase* HpBar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UInventoryWidgetBase* Inventory;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEquipmentBase* EquipWindow;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UShopBase* ShopWindow;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStatHoverBase* Hover;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UItemTooltipBase* ItemHover;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCanvasPanel* HpBarParent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCanvasPanel* RootCanvas;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWeaponInfoBase* WeaponInfo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class ULobby_ReadyWidget* ReadyWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UContactCaseBase* ContactCaseWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag")
	bool isDraging = false;
	

#pragma endregion 


#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateHpBar(float Percent);

	void ToggleInventory(bool bValue);	
	void ToggleEquipWindow(bool bValue);

	UFUNCTION()
	void ToggleHover(FVector2D LocalPosition, FVector2D AbsolutePosition, int32 Value);

	UCanvasPanel* GetRootCanvas();

#pragma endregion 
	
};
