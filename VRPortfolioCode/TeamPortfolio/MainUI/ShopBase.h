// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "ShopBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UShopBase : public UInventoryWidgetBase
{
	GENERATED_BODY()
	
#pragma region Public Function
public:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnExitButtonShop();
	virtual void SetExitButton() override;
#pragma endregion
};
