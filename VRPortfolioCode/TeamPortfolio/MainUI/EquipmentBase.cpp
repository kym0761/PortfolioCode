// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentBase.h"
#include "UI_PC.h"
#include "Components/Button.h"
#include "ItemSlotBase.h"
#include "Components/WrapBox.h"
#include "../Item/MasterItem.h"
#include "ItemWidgetBase.h"

void UEquipmentBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipmentBase::SetSlot(int Index, AMasterItem * Item)
{
	UItemSlotBase* EmptySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index * 2 + 1));

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

void UEquipmentBase::UpdateEquipment(TArray<class AMasterItem*> Equipment)
{
	for (int i = 0; i < Equipment.Num(); ++i)
	{
		SetSlot(i, Equipment[i]);
	}
}

void UEquipmentBase::OnExitButtonEquipment()
{
	AUI_PC* PC = GetOwningPlayer<AUI_PC>();
	PC->UnToggle_EquipWidget();
}

void UEquipmentBase::SetExitButton()
{
	if (Exit)
	{
		Exit->OnClicked.AddDynamic(this, &UEquipmentBase::OnExitButtonEquipment);
	}
}
