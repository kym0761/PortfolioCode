// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Look.h"
#include "../../Basic/EnemyAIController.h"
#include "BehaviorTree/BlackBoardComponent.h"

void UBTService_Look::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyAIController* aiController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (aiController)
	{
		AActor* player = Cast<AActor>(aiController->BBComponent->GetValueAsObject(GetSelectedBlackboardKey()));

		APawn * pawn = aiController->GetPawn();
		
		if (pawn && player)
		{
			FVector lookVector = player->GetActorLocation() - pawn->GetActorLocation();
			lookVector.Normalize();
			FRotator lookRot = lookVector.Rotation(); //UKismetMathLibrary::FindLookAtRotation();
			
			pawn->SetActorRotation(FMath::RInterpTo(pawn->GetActorRotation(), lookRot, DeltaSeconds, 20.0f));
		}

	}


	
	//OwnerComp.GetBlackboardComponent()->GetValueAsObject();
	
}