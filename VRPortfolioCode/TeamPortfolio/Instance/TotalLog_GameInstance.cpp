// Fill out your copyright notice in the Description page of Project Settings.


#include "TotalLog_GameInstance.h"


FItemDataTable UTotalLog_GameInstance::GetItemData(int Index) const
{
	if (ItemDataArray.IsValidIndex(Index))
	{
		return ItemDataArray[Index];
	}
	else
	{
		return FItemDataTable();
	}
}

void UTotalLog_GameInstance::SettingMonsterData()
{
	for (int Index = 0; ; ++Index)
	{
		FMonsterDataTable* Data = MonsterDataTable->FindRow<FMonsterDataTable>(*FString::FromInt(Index), TEXT("MonsterID"));
		
		if (Data == nullptr)
		{
			break;
		}

		MonsterDataArray.Add(*Data);
	}

	/*for (int Index = 0; Index != 6; ++Index)
	{
		MonsterDataArray.Add(*MonsterDataTable->FindRow<FMonsterDataTable>(*FString::FromInt(Index), TEXT("MonsterID")));
	}*/
}

void UTotalLog_GameInstance::SettingItemData()
{
	for(int Index = 0; ; ++Index)
	{
		FItemDataTable* Data = ItemDataTable->FindRow<FItemDataTable>(*FString::FromInt(Index), TEXT("ItemIndex"));
		
		if (Data == nullptr)
		{
			break;
		}		

		ItemDataArray.Add(*Data);
	}
}

void UTotalLog_GameInstance::SettingLogData()
{
	//Monster_Record.Reserve(MonsterDataArray.Num());

	for (int Index = 0; Index != (int)EMonsterType::DataMax; ++Index)
	{
		Monster_Record.Add(EMonsterType(Index), 0);
	}
	
}

void UTotalLog_GameInstance::Init()
{
	Super::Init();

	SettingMonsterData();
	SettingItemData();
	SettingLogData();
}

void UTotalLog_GameInstance::PostLoad()
{
	Super::PostLoad();
}

FMonsterDataTable UTotalLog_GameInstance::GetMonsterData(int Index) const
{
	if (MonsterDataArray.IsValidIndex(Index))
	{
		return MonsterDataArray[Index];
	}
	else
	{
		return FMonsterDataTable();
	}	
}