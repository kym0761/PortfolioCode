// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotBase.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../Item/MasterItem.h"
#include "Engine/StreamableManager.h"
#include "../Basic/BasicCharacter.h"
void UItemSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemThumbnail = Cast<UBorder>(GetWidgetFromName(TEXT("ItemThumbnail")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));

	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UItemSlotBase::OnItemButton);
	}
}

void UItemSlotBase::OnItemButton()
{
	UE_LOG(LogClass, Warning, TEXT("Button Clicked"));

	ABasicCharacter* player = GetOwningPlayerPawn<ABasicCharacter>();
	if (player)
	{
		player->RemoveFromInventory(SelectedItem);
		GetParent()->RemoveChild(this);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Player Not Exist..!?"));
	}
}

void UItemSlotBase::UpdateItemSlot(AMasterItem * Item)
{
	if (Item)
	{
		FStreamableManager loader;
		ItemThumbnail->SetBrushFromTexture(loader.LoadSynchronous<UTexture2D>(Item->ItemData.ItemThumnail));
		ItemName->SetText(FText::FromString(Item->ItemData.ItemName));
		ItemCount->SetText(FText::FromString(FString::FromInt(Item->ItemData.ItemCount)));

		SelectedItem = Item;
	}
}
