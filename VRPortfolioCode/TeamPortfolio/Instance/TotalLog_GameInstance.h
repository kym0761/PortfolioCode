// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Data/MonsterDataTable.h"
#include "../Item/ItemDataTable.h"
#include "TotalLog_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UTotalLog_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UDataTable* ItemDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UDataTable* MonsterDataTable;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<FMonsterDataTable> MonsterDataArray;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<FItemDataTable> ItemDataArray;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Record")
	TMap<EMonsterType, int> Monster_Record;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	bool isDefencePlayer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	TArray<class AMasterItem*> Inven;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	TArray<class AMasterItem*> Equip;

#pragma endregion

#pragma region Public Function
public:
	UFUNCTION(BlueprintCallable)
	FItemDataTable GetItemData(int Index) const;

	UFUNCTION(BlueprintCallable)
	FMonsterDataTable GetMonsterData(int Index) const;

	void SettingMonsterData();
	void SettingItemData();
	void SettingLogData();

	virtual void Init() override;
	virtual void PostLoad() override;

#pragma endregion
};
