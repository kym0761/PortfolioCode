// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LookPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

void UBTService_LookPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	APawn* Monster = OwnerComp.GetAIOwner()->GetPawn();
	if (Player && Monster)
	{
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(
			Monster->GetActorLocation(),
			Player->GetActorLocation()
		);
		FRotator TargetLook = FMath::RInterpTo(Monster->GetActorRotation(),
			LookRot,
			DeltaSeconds,
			15.0f)
			;
		Monster->SetActorRotation(TargetLook);
	}

}
