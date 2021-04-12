// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster.h"
#include "BehaviorTree/BehaviorTree.h"



AMonsterAIController::AMonsterAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));

}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMonster* Monster = Cast<AMonster>(InPawn);
	if (Monster)
	{
		if (Monster->MonsterBT && Monster->MonsterBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*(Monster->MonsterBT->BlackboardAsset));
			BTComponent->StartTree(*(Monster->MonsterBT));

			SetSpeedValue();
			SetCurrnetState(EMonsterState::Normal);
		}
	}

}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BTComponent->StopTree();
	
}

void AMonsterAIController::SetCurrnetState(EMonsterState NewState)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)NewState);
	}
}

void AMonsterAIController::SetTarget(FVector NewLocation)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsVector(TEXT("Target"), NewLocation);
	}
}

void AMonsterAIController::SetPlayer(AActor* Player)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsObject(TEXT("Player"), Player);
	}
}

void AMonsterAIController::SetSpeedValue()
{
	AMonster* Monster = Cast<AMonster>(GetPawn());
	if (Monster)
	{
		BBComponent->SetValueAsFloat(TEXT("WalkSpeed"), Monster->WalkSpeed);
		BBComponent->SetValueAsFloat(TEXT("RunSpeed"), Monster->RunSpeed);
	}
}
