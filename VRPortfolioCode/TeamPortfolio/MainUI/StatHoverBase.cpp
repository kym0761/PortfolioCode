// Fill out your copyright notice in the Description page of Project Settings.


#include "StatHoverBase.h"
#include "Components/TextBlock.h"

void UStatHoverBase::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBox")));
}

void UStatHoverBase::SettingText(FString NewText)
{
	if (TextBox)
	{
		TextBox->SetText(FText::FromString(NewText));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *NewText);
	}
}
