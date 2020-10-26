// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBlackboardComponent* BBComponent;

	void SetCurrentState(EZombieState NewState);

	void SetTarget(FVector NewLocation);

	void SetPlayer(AActor* Player);
};
