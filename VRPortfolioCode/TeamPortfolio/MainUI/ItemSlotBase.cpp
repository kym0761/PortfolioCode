// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotBase.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../Item/Inventory.h"
#include "../Item/MasterItem.h"
#include "ItemWidgetBase.h"
#include "InventoryWidgetBase.h"
#include "ItemTooltipBase.h"
#include "MainUIBase.h"
#include "EquipmentBase.h"
#include "UI_PC.h"

void UItemSlotBase::NativeConstruct()
{
	Super::NativeConstruct();
		
	ItemWidget = Cast<UItemWidgetBase>(GetWidgetFromName(TEXT("ItemWidget")));	
}

void UItemSlotBase::UpdateItemSlot(AMasterItem* Item)
{
	if (Item)
	{
		ItemWidget->UpdateItemSlot(Item);
	}
}

void UItemSlotBase::SetInvenParent(UInventoryWidgetBase * InvenParent)
{
	InvenWidget = InvenParent;
}

void UItemSlotBase::SetMainUIRootCanvas(UCanvasPanel* Canvas)
{
	MainUIRootCanvas = Canvas;
}

FItemDataTable UItemSlotBase::GetThisItemData()
{	
	return ItemWidget->Item->ItemData;
}

bool UItemSlotBase::GetisDraging()
{
	return InvenWidget->GetisDraging();
}

void UItemSlotBase::TooltipVisible(bool bValue)
{
	if (GetThisItemData().ItemIndex == CN_NullItemIndex)
		return;

	UItemTooltipBase* pTooltip = InvenWidget->GetTooltipWidget();
	
	if (bValue == true)
	{
		pTooltip->SetTooltipsInfo(GetThisItemData().ItemName, GetThisItemData().ItemContent, GetThisItemData().ItemThumnail);
		pTooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
		pTooltip->AddToViewport(1);
	}
	else
	{
		pTooltip->SetVisibility(ESlateVisibility::Collapsed);
		pTooltip->RemoveFromParent();
	}	
}

void UItemSlotBase::SetisDraging(bool bValue)
{
	InvenWidget->SetisDraging(bValue);
	TooltipVisible(false);
}

void UItemSlotBase::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (GetisDraging() == false)
	{
		FVector2D NewPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) +
			MainUIRootCanvas->GetCachedGeometry().AbsoluteToLocal(InGeometry.GetAbsolutePosition());

		InvenWidget->GetTooltipWidget()->SetPositionInViewport(NewPosition, false);

		TooltipVisible(true);
	}
}

void UItemSlotBase::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (GetisDraging() == false)
	{
		TooltipVisible(false);
	}
}

FReply UItemSlotBase::NativeOnMouseMove(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	FEventReply Reply = UWidgetBlueprintLibrary::Handled();

	FVector2D NewPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) +
		MainUIRootCanvas->GetCachedGeometry().AbsoluteToLocal(InGeometry.GetAbsolutePosition());

	InvenWidget->GetTooltipWidget()->SetPositionInViewport(NewPosition, false);

	return Reply.NativeReply;
}

FReply UItemSlotBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);//FReply::Handled().DetectDrag(this?, EKeys::LeftMouseButton);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		AUI_PC* PC = GetOwningPlayer<AUI_PC>();

		if (SlotType == ESlotParentType::Inventory)//인벤토리에서 실행
		{
			//소비재 -> 사용
			if (ItemWidget->GetItem()->ItemData.ItemType == EItemType::Consume)
			{
				ItemWidget->UseConsumeItem();
				ItemWidget->UpdateCount();
			}
			//장착템 -> 장착템 위치와 스왑
			else if (ItemWidget->GetItem()->ItemData.ItemType == EItemType::Equip)
			{				
				ESlotType SlotNum = ItemWidget->GetItem()->ItemData.ItemEquipSlot;

				AMasterItem* MasterItem = PC->Inventory->Equipment[(int)SlotNum];
				PC->Inventory->Equipment[(int)SlotNum] = PC->Inventory->Inven[SlotIndex];
				PC->Inventory->Equipment[(int)SlotNum]->ItemIndex = PC->Inventory->Equipment[(int)SlotNum]->ItemData.ItemIndex;
				PC->Inventory->Inven[SlotIndex] = MasterItem;
				UpdateItemSlot(PC->Inventory->Inven[SlotIndex]);
				PC->MainWidgetObject->EquipWindow->SetSlot((int)SlotNum, PC->Inventory->Equipment[(int)SlotNum]);				
			}
		}
		else if(SlotType == ESlotParentType::Equipment)//장비창에서 실행
		{//장착해제
			int EmptySlotNum = PC->MainWidgetObject->Inventory->GetEmptySlot();

			AMasterItem* MasterItem = PC->Inventory->Equipment[SlotIndex];
			PC->Inventory->Equipment[SlotIndex] = PC->Inventory->Inven[EmptySlotNum];
			PC->Inventory->Equipment[SlotIndex]->ItemIndex = PC->Inventory->Equipment[SlotIndex]->ItemData.ItemIndex;
			PC->Inventory->Inven[EmptySlotNum] = MasterItem;
			UpdateItemSlot(PC->Inventory->Equipment[SlotIndex]);
			PC->MainWidgetObject->Inventory->SetSlot(EmptySlotNum, PC->Inventory->Inven[EmptySlotNum]);
		}
		else if (SlotType == ESlotParentType::Shop)
		{
			//구입
			//장착 일반템 -> 그냥 한칸 차지
			//소비템 -> 합쳐져야됨
			//골드 깎아야됨
			
			//AMasterItem* MasterItem = ;
			//int ExistIndex = PC->MainWidgetObject->Inventory->HaveThis(MasterItem);
			//if (MasterItem->ItemData.ItemType == EItemType::Consume && ExistIndex != -1)//합치기
			//{
			//	PC->Inventory->Inven[ExistIndex]->ItemData.ItemCount += 1;
			//	PC->MainWidgetObject->Inventory->UpdateInventoryWithIndex(PC->Inventory->Inven, ExistIndex);
			//}
			//else//그냥 생성
			//{
			//	int EmptySlotNum = PC->MainWidgetObject->Inventory->GetEmptySlot();

			//	PC->Inventory->Inven[EmptySlotNum] = MasterItem;
			//	PC->Inventory->Equipment[SlotIndex] = PC->Inventory->Inven[EmptySlotNum];
			//	PC->Inventory->Equipment[SlotIndex]->ItemIndex = PC->Inventory->Equipment[SlotIndex]->ItemData.ItemIndex;
			//	PC->Inventory->Inven[EmptySlotNum] = MasterItem;
			//	PC->MainWidgetObject->Inventory->SetSlot(EmptySlotNum, PC->Inventory->Inven[EmptySlotNum]);
			//}
		}
	}

	TooltipVisible(false);

	return Reply.NativeReply;
}

void UItemSlotBase::AddItemOnInventory(int Index, AMasterItem * Item)
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	PC->Inventory->Inven[Index] = Item;

	PC->GetMainUI()->Inventory->SetSlot(Index, PC->Inventory->Inven[Index]);
}

void UItemSlotBase::SwapInvenWithEquip(int InvenslotIndex, int EquipslotIndex)
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	PC->Inventory->SwapSlotInvenwithEquip(InvenslotIndex, EquipslotIndex);

	PC->GetMainUI()->Inventory->SetSlot(InvenslotIndex, PC->Inventory->Inven[InvenslotIndex]);
	PC->GetMainUI()->EquipWindow->SetSlot(EquipslotIndex, PC->Inventory->Equipment[EquipslotIndex]);
}

void UItemSlotBase::RemoveEquipAddInven(int InvenslotIndex, int EquipslotIndex)
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	
}