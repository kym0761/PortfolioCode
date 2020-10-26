// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidgetBase.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTPG_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UProgressBar* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UTextBlock* AliveCount;

	virtual void NativeConstruct() override;

	void UpdateHPBar(float Percent);
	void SetAliveCount(int32 NewCount);
};
