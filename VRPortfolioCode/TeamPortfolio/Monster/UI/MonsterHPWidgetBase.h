// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UMonsterHPWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProgressBar* HPbar;

#pragma endregion

#pragma region Public Member
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HpBarUpdate(float Percent);


#pragma endregion
};
