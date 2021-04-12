// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPCM.h"
#include "TeamP_BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"


ABasicPCM::ABasicPCM() 
{
}

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);


	ATeamP_BasicPlayer* Player = GetOwningPlayerController()->GetPawn<ATeamP_BasicPlayer>();

	if (Player) {
		
		float TargetFOV = Player->bIsIronsight ? IronsightFOV : NormalFOV;

		float ResultFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 20.0f);

		SetFOV(ResultFOV);

		FVector TargetPosition = Player->bIsCrouched ? Player->CrouchedSpringArmPosition : Player->NormalSpringArmPosition;

		FVector ResultSpringArmPosition = FMath::VInterpTo(Player->SpringArm->GetRelativeLocation(), TargetPosition, DeltaTime, 15.0f);

		Player->SpringArm->SetRelativeLocation(ResultSpringArmPosition);

	}

	//APlayerController* playerController = GetOwningPlayerController();
	//if (playerController)
	//{
	//	if (TargetCameraPitch != 0.0f)
	//	{
	//		playerController->SetControlRotation(FRotator);
	//	}
	//}


}

void ABasicPCM::Recoil()
{
	float TargetFOV = FMath::FRandRange(0.2f, 0.5f);

	float ResultFOV = FMath::Lerp(GetFOVAngle(), TargetFOV, 0.1f);

	SetFOV(ResultFOV);
}
