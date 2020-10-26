// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotBase.generated.h"

class UBorder;
class UTextBlock;
class UButton;
class AMasterItem;
/**
 * 
 */
UCLASS()
class PROJECTPG_API UItemSlotBase : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBorder* ItemThumbnail;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextBlock* ItemCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UButton* ItemButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemButton();

	void UpdateItemSlot(AMasterItem* Item);

	AMasterItem* SelectedItem;

	//bool bUse = false;
};
