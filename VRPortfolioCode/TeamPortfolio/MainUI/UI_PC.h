// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI_PC.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API AUI_PC : public APlayerController
{
	GENERATED_BODY()

#pragma region Public Member
#pragma region UI
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UResultFadeOutBase> ResultWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UResultFadeOutBase* ResultWidgetObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UMainUIBase> MainWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UMainUIBase* MainWidgetObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class ULobby_WidgetBase> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class ULobby_WidgetBase* LobbyWidgetObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class ULobby_ReadyWidget> ReadyWidgetClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class ULobby_ReadyWidget* ReadyWidgetObject;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	bool bInvenToggle = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	bool bEquipToggle = false;

#pragma endregion

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inven")
	class UInventory* Inventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inven")
	TArray<class AMasterItem*> PickItemList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Player")
	bool IsDefencePlayer = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	bool UITestMode = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Lobby")
	bool IsReady = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool IsMasterLobby = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Boss")
	TSubclassOf<class ABossCharacter> Boss;
#pragma endregion


#pragma region Public Function
public:
	AUI_PC();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Destroyed() override;
	class UMainUIBase* GetMainUI();

	void SettingUI();

	UFUNCTION(BlueprintCallable)
	void SettingUI_TEST();

	void AddResultWidget();
	void AddTimeWidget();
	void SettingReadyWidget();

	UFUNCTION(Client, Reliable)
	void AddReadyWidget();
	void AddReadyWidget_Implementation();

	void Toggle_InvenWidget();
	void UnToggle_InvenWidget();

	void Toggle_EquipWidget();
	void UnToggle_EquipWidget();

	UFUNCTION(Client , Reliable)
	void S2C_SettingisDefence();
	void S2C_SettingisDefence_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_SpawnandPossess();
	void C2S_SpawnandPossess_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_ReplaceWorld(FName Tag);
	void C2S_ReplaceWorld_Implementation(FName Tag);

	UFUNCTION(Server, Reliable)
	void C2S_SetIsReady(bool bValue);
	void C2S_SetIsReady_Implementation(bool bValue);

	void AddPickItem(class AMasterItem* AddItem);
	void RemovePickItem(class AMasterItem* RemoveItem);

	void Pickup();

	UFUNCTION(Server, Reliable)
	void C2S_CheckPickupItem(class AMasterItem* PickupItem);
	void C2S_CheckPickupItem_Implementation(class AMasterItem* PickupItem);

	UFUNCTION(Client, Reliable)
	void S2C_InsertItem(class AMasterItem* PickupItem);
	void S2C_InsertItem_Implementation(class AMasterItem* PickupItem);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	bool HealItemUse();

#pragma endregion
};
