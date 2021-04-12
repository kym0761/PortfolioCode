// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/WrapBox.h"
#include "Components/InvalidationBox.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "MainUIBase.h"
#include "ItemSlotBase.h"
#include "ItemWidgetBase.h"
#include "ItemTooltipBase.h"
#include "UI_PC.h"
#include "../Item/Inventory.h"
#include "../Item/MasterItem.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemSlots = Cast<UWrapBox>(GetWidgetFromName(TEXT("WrapBox_Item")));
	Drag = Cast<UBorder>(GetWidgetFromName(TEXT("Drag")));
	Exit = Cast<UButton>(GetWidgetFromName(TEXT("Exit")));

	SetExitButton();
}

void UInventoryWidgetBase::UpdateInventory(TArray<class AMasterItem*> Inventory)
{
	for (int i = 0; i < Inventory.Num(); ++i)
	{
		SetSlot(i, Inventory[i]);
	}
}

void UInventoryWidgetBase::UpdateInventoryWithIndex(TArray<class AMasterItem*> Inventory, int32 FirstIndex)
{
	SetSlot(FirstIndex, Inventory[FirstIndex]);
}

int UInventoryWidgetBase::GetEmptySlot()
{
	for (int Index = 0; Index < ItemSlots->GetChildrenCount(); ++Index)
	{
		UItemSlotBase* InventorySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index));		

		if (InventorySlot)
		{
			if (IsValid(InventorySlot->ItemWidget->Item))
			{
				if (InventorySlot->ItemWidget->Item->ItemData.ItemIndex == CN_NullItemIndex)
				{
					return Index;
				}//아이템이 존재하는데 널이 아니면 있으니 다음으로 진행
			}
			else//아이템이 없으니 생성
			{
				return Index;
			}
		}
	}

	return -1;
}

void UInventoryWidgetBase::SetSlot(int Index, AMasterItem* Item)
{
	UItemSlotBase* EmptySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index));
	
	if (EmptySlot && Item->ItemIndex == CN_NullItemIndex)
	{
		EmptySlot->UpdateItemSlot(Item);
		EmptySlot->ItemWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		EmptySlot->UpdateItemSlot(Item);
		EmptySlot->ItemWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventoryWidgetBase::SetSlotsParent()
{
	for (int index = 0; index < ItemSlots->GetChildrenCount(); ++index)
	{
		UItemSlotBase* ChildSlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(index));
		
		if (IsValid(ChildSlot))
		{
			ChildSlot->SetInvenParent(this);
			ChildSlot->SetMainUIRootCanvas(MainUIParent->RootCanvas);
		}
	}
}

void UInventoryWidgetBase::SetMainUIParent(UMainUIBase* MainUI)
{
	MainUIParent = MainUI;
}

bool UInventoryWidgetBase::GetisDraging()
{	
	return MainUIParent->isDraging;
}

void UInventoryWidgetBase::SetisDraging(bool bValue)
{
	MainUIParent->isDraging = bValue;
}

int UInventoryWidgetBase::HaveThis(AMasterItem* FuncItem)
{
	for (int Index = 0; Index < ItemSlots->GetChildrenCount(); ++Index)
	{
		UItemSlotBase* InventorySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index));

		if (InventorySlot)
		{
			if (IsValid(InventorySlot->ItemWidget->Item))
			{
				if (InventorySlot->ItemWidget->Item->ItemData.ItemIndex == FuncItem->ItemData.ItemIndex)
					return Index;
			}
		}
	}
	return -1;
}

FItemDataTable UInventoryWidgetBase::GetItemData(int32 Index)
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	return PC->Inventory->GetItemData(Index);
}

UItemTooltipBase* UInventoryWidgetBase::GetTooltipWidget()
{
	return MainUIParent->ItemHover;
}

void UInventoryWidgetBase::SwapSlot(int32 FrontslotIndex, int32 OtherSlotIndex)
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	PC->Inventory->SwapSlot(FrontslotIndex, OtherSlotIndex);
	UpdateInventoryWithIndex(PC->Inventory->Inven, FrontslotIndex);
	UpdateInventoryWithIndex(PC->Inventory->Inven, OtherSlotIndex);
}

void UInventoryWidgetBase::AddItem_Inventory(TArray<AMasterItem*> Inventory, AMasterItem* Item)
{
	if (Item->ItemData.ItemType == EItemType::Consume)
	{
		int Index = HaveThis(Item);
		if (Index == -1)
		{
			int NotEqualIndex = GetEmptySlot();
			Inventory[NotEqualIndex] = Item;
			SetSlot(NotEqualIndex, Inventory[NotEqualIndex]);
		}
		else
		{
			Inventory[Index]->ItemData.ItemCount++;
			UpdateInventoryWithIndex(Inventory, Index);
		}
	}
	else
	{
		int NotEqualIndex = GetEmptySlot();
		Inventory[NotEqualIndex] = Item;
		SetSlot(NotEqualIndex, Inventory[NotEqualIndex]);		
	}

	//Item->Destroy();
}

void UInventoryWidgetBase::OnExitButton()
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	PC->UnToggle_InvenWidget();
}

void UInventoryWidgetBase::SetExitButton()
{
	if (Exit)
	{
		Exit->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnExitButton);
	}
}
