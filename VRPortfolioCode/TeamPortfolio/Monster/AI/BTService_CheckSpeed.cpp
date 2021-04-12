// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckSpeed.h"
#include "../Monster.h"
#include "../MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBTService_CheckSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{		
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster)
	{
		if (Monster->CurrentState == EMonsterState::Chase)
		{
			Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->RunSpeed;
		}
	}

}
