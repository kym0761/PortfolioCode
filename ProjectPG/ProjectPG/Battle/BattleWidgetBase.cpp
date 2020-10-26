// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidgetBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBattleWidgetBase::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alive_Count")));

}

void UBattleWidgetBase::UpdateHPBar(float Percent)
{
	if (HPBar)
	{
		HPBar->SetPercent(Percent);
	}
}

void UBattleWidgetBase::SetAliveCount(int32 NewCount)
{
	if (AliveCount)
	{
		FString temp = FString::Printf(TEXT("%d Alived"), NewCount);
		AliveCount->SetText(FText::FromString(temp));
	}
}

