// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidgetBase.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "../Item/MasterItem.h"
#include "Engine/StreamableManager.h"
#include "../Instance/TotalLog_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void UItemWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemThumnail = Cast<UBorder>(GetWidgetFromName(TEXT("ItemThumnail")));	
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
}

void UItemWidgetBase::UpdateItemSlot(AMasterItem* ParamItem)
{
	if (ParamItem)
	{
		Item = ParamItem;

		if (ParamItem->ItemData.ItemIndex == CN_NullItemIndex)
		{
			Item->ItemIndex = Item->ItemData.ItemIndex;
			this->SetVisibility(ESlateVisibility::Collapsed);
		}

		FStreamableManager Loader;
		
		ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(Item->ItemData.ItemThumnail));
		UpdateCount();
	}
}

AMasterItem* UItemWidgetBase::GetItem()
{
	return Item;
}

void UItemWidgetBase::UseConsumeItem()
{
	if (Item->ItemData.ItemCount == 1)
	{
		UTotalLog_GameInstance*	GameInstance = Cast<UTotalLog_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (IsValid(GameInstance))
		{
			Item->ItemData.SetItemData(GameInstance->GetItemData(CN_NullItemIndex));
			Item->ItemIndex = Item->ItemData.ItemIndex;
			this->SetVisibility(ESlateVisibility::Collapsed);
		}		
	}
	else
	{
		Item->ItemData.ItemCount -= 1;
	}

	//사용 효과를 내야함.....

}

void UItemWidgetBase::UpdateCount()
{
	ItemCount->SetText(FText::FromString(FString::FromInt(Item->ItemData.ItemCount)));
}
