// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidgetBase.h"
#include "SpawnSlotBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
void UBossWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	/*Spawn Slots will be add to SpawnSlots. currently default slot Max Number is 8 (0~7)*/
	SlotBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("SlotBox")));
	if (SlotBox)
	{
		TArray<UWidget*> childrenArray = SlotBox->GetAllChildren();
		
		//UE_LOG(LogClass, Warning, TEXT("Array Size : %d"), childrenArray.Num());
		
		for (auto iter = childrenArray.CreateConstIterator(); iter; iter++)
		{
			USpawnSlotBase* slot = Cast<USpawnSlotBase>(*iter);
			if (slot)
			{
				SpawnSlots.Add(slot);
				//UE_LOG(LogClass, Warning, TEXT("Slot Add Success"));
			}
		}
	}

	

}
