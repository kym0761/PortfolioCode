// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckStateByDistance.h"
#include "../MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckStateByDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster || !Player)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Distance(Monster->GetActorLocation(), Player->GetActorLocation());

	switch (Condition)
	{
	case ETaskCondition::Less:
	{
		if (Distance < Range)
		{
			Monster->SetCurrentState(TargetState);
		}
	}
	break;

	case ETaskCondition::Greater:
	{
		if (Distance > Range)
		{
			Monster->SetCurrentState(TargetState);
		}
	}
	break;
	}

	return EBTNodeResult::Succeeded;
}
