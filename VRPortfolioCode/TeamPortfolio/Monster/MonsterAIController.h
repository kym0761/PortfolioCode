// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Monster.h"

#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBlackboardComponent* BBComponent;

	void SetCurrnetState(EMonsterState NewState);

	void SetTarget(FVector NewLocation);

	void SetPlayer(AActor* Player);

	void SetSpeedValue();
	
};
