// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
AEnemyAIController::AEnemyAIController() 
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AEnemyAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* zombie = Cast<AEnemyCharacter>(InPawn);
	if (zombie)
	{
		if (zombie->ZombieBT && zombie->ZombieBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*(zombie->ZombieBT->BlackboardAsset) );
			BTComponent->StartTree(*(zombie->ZombieBT) );

		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BTComponent)
	{
		BTComponent->StopTree();
	}

}

void AEnemyAIController::SetCurrentState(EZombieState NewState)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)NewState);
	}
}

void AEnemyAIController::SetTarget(FVector NewLocation)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsVector(TEXT("Destination"), NewLocation);
	}
}

void AEnemyAIController::SetPlayer(AActor * Player)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsObject(TEXT("Player"), Player);
	}
}
