// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

class UBattleWidgetBase;
class UItemToolTipBase;
class UInventoryWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECTPG_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UBattleWidgetBase* BattleWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UItemToolTipBase> ItemToolTipClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UItemToolTipBase* ItemToolTipObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UInventoryWidgetBase> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UInventoryWidgetBase* InventoryWidgetObject;


	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void ShowItemToolTip(FString ItemName);

	void HideItemToolTip();

	void ShowInventory();
	void HideInventory();
	void ToggleInventory();
};
