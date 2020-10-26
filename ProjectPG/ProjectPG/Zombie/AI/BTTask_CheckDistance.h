// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Basic/EnemyCharacter.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckDistance.generated.h"

UENUM(BlueprintType)
enum class ECondition : uint8
{
	LessThan = 0 UMETA(Display = "<"),
	GreaterThan = 1 UMETA(Display = ">")
};

/**
 * 
 */
UCLASS()
class PROJECTPG_API UBTTask_CheckDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
		ECondition TargetCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
		float TargetDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
		EZombieState TargetState;
};
