// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../TeamPortfolio.h"
#include "Blueprint/UserWidget.h"
#include "HpBarBase.generated.h"

DECLARE_DELEGATE_ThreeParams(FDele_Single_ThreeParam, FVector2D, FVector2D, int32)
/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UHpBarBase : public UUserWidget
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar* HpBar;

#pragma endregion 

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	FDele_Single_ThreeParam Fuc_DeleSingle_ThreeParam;

	UFUNCTION(BlueprintCallable)
	void UpdateHpBar(float Percent);

	virtual void BeginDestroy() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

#pragma endregion 
};
