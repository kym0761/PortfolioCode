// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInfoBase.h"
#include "Components/TextBlock.h"

void UWeaponInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponName = Cast<UTextBlock>(GetWidgetFromName(TEXT("WeaponName")));
	BulletNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("BulletNum")));
	BulletMaxNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("BulletMaxNum")));
}

void UWeaponInfoBase::SetItemName(FString NewName)
{
	if (WeaponName)
	{
		WeaponName->SetText(FText::FromString(NewName));
	}
}

void UWeaponInfoBase::SetIBulletNum(int Num)
{
	if (BulletNum)
	{
		FString newText = FString::Printf(TEXT("%d"), Num);
		BulletNum->SetText(FText::FromString(newText));
	}
}

void UWeaponInfoBase::SetIBulletMaxNum(int Num)
{
	if (BulletMaxNum)
	{
		FString newText = FString::Printf(TEXT("%d"), Num);
		BulletMaxNum->SetText(FText::FromString(newText));
	}
}
