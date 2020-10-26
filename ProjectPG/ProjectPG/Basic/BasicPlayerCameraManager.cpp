// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerCameraManager.h"
#include "BasicCharacter.h"
//#include "BasicPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
ABasicPlayerCameraManager::ABasicPlayerCameraManager() 
{
	
}

void ABasicPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	ABasicCharacter* player = GetOwningPlayerController()->GetPawn<ABasicCharacter>();
	
	if (player && player->IsValidLowLevel())
	{
		float targetFOV = player->bIsIronsight ? IronsightFOV : NormalFOV;
		SetFOV(FMath::Lerp(GetFOVAngle(), targetFOV, 0.125f));
		
		//FMath::FInterpTo(GetFOVAngle(), targetFOV, DeltaTime, 20.0f);
		//FMath::VInterpTo();
		
		//FMath::Lerp(GetFOVAngle(), targetFOV, 0.5f);
	
		FVector targetPos = player->bIsCrouched ? player->CrouchSpringArmPosition : player->NormalSpringArmPosition;

		FVector result = FMath::VInterpTo(player->SpringArm->GetRelativeLocation(), targetPos, DeltaTime, 5.0f);
		player->SpringArm->SetRelativeLocation(result);

	}
}
