// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarBase.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UHpBarBase::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
}

void UHpBarBase::UpdateHpBar(float Percent)
{
	HpBar->SetPercent(Percent);
}

void UHpBarBase::BeginDestroy()
{
	Fuc_DeleSingle_ThreeParam.Unbind();	

	Super::BeginDestroy();
}

void UHpBarBase::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (Fuc_DeleSingle_ThreeParam.IsBound() == true)
	{
		Fuc_DeleSingle_ThreeParam.Execute(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()), InGeometry.GetAbsolutePosition(), CN_Activate);
	}	
}

void UHpBarBase::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (Fuc_DeleSingle_ThreeParam.IsBound() == true)
	{
		Fuc_DeleSingle_ThreeParam.Execute(FVector2D::ZeroVector, FVector2D::ZeroVector, CN_Deactivate);
	}	
}

FReply UHpBarBase::NativeOnMouseMove(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	FEventReply Reply = UWidgetBlueprintLibrary::Handled();

	if (Fuc_DeleSingle_ThreeParam.IsBound() == true)
	{
		Fuc_DeleSingle_ThreeParam.Execute(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()), InGeometry.GetAbsolutePosition(), CN_NonChange);
		
		//부모 떼고 자식의 로컬좌표
		//InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) 
	}

	return Reply.NativeReply;
}