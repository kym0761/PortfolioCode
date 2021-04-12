// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomNextPoint.h"
#include "../Monster.h"
#include "../MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_SetRandomNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC && AIC->GetPawn())
	{
		FVector NewTarget = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(),
			AIC->GetPawn()->GetActorLocation(),
			500.0f
			);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("Target"), NewTarget);

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
