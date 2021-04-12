// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPWidgetBase.h"
#include "Components/Progressbar.h"

void UMonsterHPWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	HPbar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Hp")));	
}

void UMonsterHPWidgetBase::HpBarUpdate(float Percent)
{
	if (HPbar)
	{
		HPbar->SetPercent(Percent);
	}

	//MonsterHpBarUI
}
