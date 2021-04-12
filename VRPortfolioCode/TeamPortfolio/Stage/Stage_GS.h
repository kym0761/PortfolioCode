// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Stage_GS.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API AStage_GS : public AGameStateBase
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = "OnRep_LeftTime", Category = "Network")
	int LeftTime = 120;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = "OnRep_ResultWG", Category = "Network")
	bool IsExistResultUI = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Network")
	bool CanGotoNext = false;

#pragma endregion

#pragma region Public Function
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetGotoNext(bool bValue);

	UFUNCTION()
	void OnRep_LeftTime();

	UFUNCTION()
	void OnRep_ResultWG();

	UFUNCTION(Client, Reliable)
	void CreateTimeUI();
	void CreateTimeUI_Implementation();

	UFUNCTION(Client, Reliable)
	void CreateResultUI();
	void CreateResultUI_Implementation();

#pragma endregion
};
