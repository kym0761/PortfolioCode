// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../TeamPortfolio.h"
#include "ItemDataTable.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPORTFOLIO_API UInventory : public UActorComponent
{
	GENERATED_BODY()
#pragma region Public Member
public:	
	UPROPERTY(BlueprintReadOnly, Visibleanywhere)
	TArray<class AMasterItem*> Inven;

	UPROPERTY(BlueprintReadOnly, Visibleanywhere)
	TArray<class AMasterItem*> Equipment;

#pragma endregion

#pragma region Public Function
	// Sets default values for this component's properties
	UInventory();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SwapSlot(int32 Slot, int32 OtherSlot);

	UFUNCTION(BlueprintCallable)
	void SwapSlotInvenwithEquip(int32 InvenSlot, int32 EquipSlot);

	UFUNCTION()
	FItemDataTable GetItemData(int32 InvenIndex);

	void DataLoading();
	void PassData(TArray<class AMasterItem*> BeforeInven, TArray<class AMasterItem*> BeforeEquipment);
	bool isNull(int32 Index);
	void SetNullItem(int32 Index);

#pragma endregion

#pragma region protected Function
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion
		
};
