// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/ScrollBox.h"
#include "ItemSlotBase.h"
void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemSlots = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemSlots")));
}

void UInventoryWidgetBase::UpdateInventory(TArray<AMasterItem*> Inventory)
{
	/*for (int i = ItemSlots->GetChildrenCount() - 1; i > 0; --i)
	{
		ItemSlots->RemoveChildAt(i);
	}|*/

	ItemSlots->ClearChildren();

	//for (int i = ItemSlots->GetChildrenCount() - 1; i > 0; --i)
	//{
	//	UItemSlotBase* slot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(i));
	//	if (slot)
	//	{
	//		slot->bUse = false;
	//		slot->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}

	//for (int i = 0; Inventory.Num(); i++)
	//{

	//}


	if (ItemSlotClass)
	{
		for (int i = 0; i < Inventory.Num(); ++i)
		{
			////native construct is not working.
			UItemSlotBase* itemSlot =// NewObject<UItemSlotBase>(ItemSlotClass);

			CreateWidget<UItemSlotBase>(this,ItemSlotClass);
			if (itemSlot)
			{
				itemSlot->UpdateItemSlot(Inventory[i]);

				if (ItemSlots)
				{
					ItemSlots->AddChild(itemSlot);
				}
			}
		}
	}

}

//int32 UInventoryWidgetBase::GetEmptySlot()
//{
//	for (int i = ItemSlots->GetChildrenCount() - 1; i > 0; --i)
//	{
//		UItemSlotBase* slot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(i));
//		if (slot && slot->bUse == false)
//		{
//			return i;
//		}
//	}
//
//	return -1;
//}
//
//void UInventoryWidgetBase::SetSlot(int32 Index, AMasterItem * Item)
//{
//	UItemSlotBase* slot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index));
//	if(slot && slot->bUse == false)
//	{
//		slot->UpdateItemSlot(Item);
//		slot->bUse = true;
//		slot->SetVisibility(ESlateVisibility::Visible);
//	}
//}
