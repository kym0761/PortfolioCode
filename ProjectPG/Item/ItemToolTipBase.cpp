// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTipBase.h"
#include "Components/TextBlock.h"
void UItemToolTipBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_ItemName")));
}

void UItemToolTipBase::SetItemName(FString NewName)
{
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(NewName));
	}
}
