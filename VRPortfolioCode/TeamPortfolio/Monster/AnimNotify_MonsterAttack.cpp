// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterAttack.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster.h"


void UAnimNotify_MonsterAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//AActor* Monster = MeshComp->GetOwner();
	AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner());
	
	if (IsValid(Monster))
	{
		Monster->DamageProcess();
	}
}
