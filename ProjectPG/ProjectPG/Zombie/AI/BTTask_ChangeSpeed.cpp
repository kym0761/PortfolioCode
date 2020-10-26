// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../../Basic/EnemyCharacter.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aiController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (aiController)
	{
		AEnemyCharacter* zombie = aiController->GetPawn<AEnemyCharacter>();

		if (zombie)
		{
			zombie->SetSpeed();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}