// Fill out your copyright notice in the Description page of Project Settings.


#include "ContactWidgetBase.h"
#include "Components/TextBlock.h"

void UContactWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ContactText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBox")));
}

void UContactWidgetBase::SetText(FString NewText)
{
	if (ContactText)
	{
		ContactText->SetText(FText::FromString(NewText));
	}
}
