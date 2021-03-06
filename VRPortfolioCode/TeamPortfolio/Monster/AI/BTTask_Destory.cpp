// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Destory.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_Destory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		APawn* Pawn = AIC->GetPawn();
		if (Pawn)
		{
			Pawn->Destroy();

			return EBTNodeResult::Succeeded;

		}
	}
	return EBTNodeResult::Failed;
}
