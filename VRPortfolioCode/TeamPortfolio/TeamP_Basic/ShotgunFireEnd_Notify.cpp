// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunFireEnd_Notify.h"
#include "TeamP_AnimInstance.h"
#include "TeamP_BasicPlayer.h"

void UShotgunFireEnd_Notify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(MeshComp->GetOwner());
	
	if (Player)
	{
		UE_LOG(LogClass,Warning,TEXT("Notify"))
		if (Player->bIsShotgun)
		{
			Player->bFireShotgun = false;
		}
		Player->bIsWeaponChange = false;
	}
}
