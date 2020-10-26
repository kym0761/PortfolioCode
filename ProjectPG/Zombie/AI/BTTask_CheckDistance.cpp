// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "AIController.h"
#include "../../Basic/EnemyAIController.h"
#include "BehaviorTree/BlackBoardComponent.h"

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* aiController =Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	
	if (aiController)
	{
		//load Blackboard Data
		AActor* player = Cast<AActor>(aiController->BBComponent->GetValueAsObject(TEXT("Player")));

		AEnemyCharacter* pawn = aiController->GetPawn<AEnemyCharacter>();
		if (pawn)
		{
			float distance = FVector::Distance(pawn->GetActorLocation(), player->GetActorLocation());

			switch (TargetCondition)
			{

			case ECondition::LessThan:
			{
				if (distance < TargetDistance)
				{
					pawn->SetCurrentState(TargetState);
				}
			}
				break;
			case ECondition::GreaterThan:
			{
				if (distance > TargetDistance)
				{
					pawn->SetCurrentState(TargetState);
				}
			}
				break;
			}

		}
	}
	

	return EBTNodeResult::Succeeded;
}