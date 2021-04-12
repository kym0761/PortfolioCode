// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTooltipBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/StreamableManager.h"

void UItemTooltipBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemBodyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemTextBody")));
	ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("thumnail")));
}

void UItemTooltipBase::SetItemName(FString NewName)
{
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(NewName));
	}
}

void UItemTooltipBase::SetItemBodyText(FString NewBody)
{
	if (ItemBodyText)
	{
		ItemBodyText->SetText(FText::FromString(NewBody));
	}
}

void UItemTooltipBase::SetImage(TAssetPtr<class UTexture2D> Thumnail)
{
	if (ItemThumnail)
	{
		FStreamableManager Loader;
		ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(Thumnail));
	}
}

void UItemTooltipBase::SetTooltipsInfo(FString NewName, FString NewBody, TAssetPtr<class UTexture2D> Thumnail)
{
	SetItemName(NewName);
	SetItemBodyText(NewBody);
	SetImage(Thumnail);
}
