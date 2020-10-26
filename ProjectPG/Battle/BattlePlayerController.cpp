// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"
#include "BattleWidgetBase.h"
#include "../Item/ItemToolTipBase.h"
#include "InventoryWidgetBase.h"
#include "../Basic/BasicCharacter.h"
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (BattleWidgetClass)
		{
			BattleWidgetObject = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
			if (BattleWidgetObject)
			{
				BattleWidgetObject->AddToViewport();
				bShowMouseCursor = false;
				SetInputMode(FInputModeGameOnly());
			}
		}

		if (ItemToolTipClass)
		{
			ItemToolTipObject = CreateWidget<UItemToolTipBase>(this, ItemToolTipClass);
			if (ItemToolTipObject)
			{
				ItemToolTipObject->AddToViewport();
				HideItemToolTip();
			}
		}

		if (InventoryWidgetClass)
		{
			InventoryWidgetObject = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			if (InventoryWidgetObject)
			{
				InventoryWidgetObject->AddToViewport();
				HideInventory();
			}
		}

	}
}

void ABattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Inventory", IE_Pressed, this, &ABattlePlayerController::ToggleInventory);
}

void ABattlePlayerController::ShowItemToolTip(FString ItemName)
{
	if (ItemToolTipObject)
	{
		ItemToolTipObject->SetItemName(ItemName);
		ItemToolTipObject->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABattlePlayerController::HideItemToolTip()
{
	if (ItemToolTipObject)
	{
		ItemToolTipObject->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void ABattlePlayerController::ShowInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ABattlePlayerController::HideInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void ABattlePlayerController::ToggleInventory()
{
	if (InventoryWidgetObject)
	{
		if (InventoryWidgetObject->GetVisibility() == ESlateVisibility::Visible)
		{
			HideInventory();
		}
		else
		{
			ABasicCharacter* player = Cast<ABasicCharacter>(GetPawn());

			if (player)
			{
				InventoryWidgetObject->UpdateInventory(player->Inventory);
				ShowInventory();
			}

		}
	}
}
