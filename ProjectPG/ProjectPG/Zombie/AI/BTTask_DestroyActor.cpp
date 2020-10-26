// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DestroyActor.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aiController = Cast<AAIController>(OwnerComp.GetOwner());
	
	if (aiController)
	{
		APawn* pawn = aiController->GetPawn();
		if (pawn)
		{
			pawn->Destroy();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}