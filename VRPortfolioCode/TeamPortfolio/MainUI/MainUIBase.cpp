// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "InventoryWidgetBase.h"
#include "StatHoverBase.h"
#include "HpBarBase.h"
#include "ItemTooltipBase.h"
#include "EquipmentBase.h"
#include "ShopBase.h"
#include "WeaponInfoBase.h"
#include "../Lobby/Lobby_ReadyWidget.h"
#include "ContactCaseBase.h"

void UMainUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UHpBarBase>(GetWidgetFromName(TEXT("HpBar_1")));
	Inventory = Cast<UInventoryWidgetBase>(GetWidgetFromName(TEXT("Inventory")));
	EquipWindow = Cast<UEquipmentBase>(GetWidgetFromName(TEXT("Equipment")));
	ShopWindow = Cast<UShopBase>(GetWidgetFromName(TEXT("Shop")));
	Hover = Cast<UStatHoverBase>(GetWidgetFromName(TEXT("Hover")));
	HpBarParent = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("HpBarParent")));
	RootCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("RootPanel")));
	ItemHover = Cast<UItemTooltipBase>(GetWidgetFromName(TEXT("ItemTooltip")));
	WeaponInfo = Cast<UWeaponInfoBase>(GetWidgetFromName(TEXT("WeaponInfo")));
	ReadyWidget = Cast<ULobby_ReadyWidget>(GetWidgetFromName(TEXT("Lobby_Ready")));
	ContactCaseWidget = Cast<UContactCaseBase>(GetWidgetFromName(TEXT("ContractCase")));

	ShopWindow->SetVisibility(ESlateVisibility::Collapsed);
	Inventory->SetVisibility(ESlateVisibility::Collapsed);
	EquipWindow->SetVisibility(ESlateVisibility::Collapsed);
	HpBar->Fuc_DeleSingle_ThreeParam.BindUFunction(this, FName("ToggleHover"));
	Hover->SetVisibility(ESlateVisibility::Collapsed);
	Hover->RemoveFromViewport();
	ItemHover->SetVisibility(ESlateVisibility::Collapsed);
	ItemHover->RemoveFromViewport();
	ReadyWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainUIBase::UpdateHpBar(float Percent)
{
	if (HpBar)
	{
		HpBar->UpdateHpBar(Percent);
	}
}

void UMainUIBase::ToggleInventory(bool bValue)
{
	if (bValue)
	{
		Inventory->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainUIBase::ToggleEquipWindow(bool bValue)
{
	if (bValue)
	{
		EquipWindow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EquipWindow->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainUIBase::ToggleHover(FVector2D LocalPosition, FVector2D AbsolutePosition, int32 Value)
{
	FVector2D NewPosition = LocalPosition + CN_PaddingVector + 
							RootCanvas->GetCachedGeometry().AbsoluteToLocal(AbsolutePosition);

	if (Value == CN_Activate)
	{
		Hover->AddToViewport();
		Hover->SetPositionInViewport(NewPosition, false);
		Hover->SettingText(FString("HP : 0"));
		Hover->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(Value == CN_Deactivate)
	{
		Hover->SetVisibility(ESlateVisibility::Collapsed);
		Hover->RemoveFromViewport();
	}
	else
	{
		Hover->SetPositionInViewport(NewPosition, false);
		Hover->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UCanvasPanel* UMainUIBase::GetRootCanvas()
{
	return RootCanvas;
}
