// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameStateBase.h"
#include "BattlePlayerController.h"
#include "kismet/GameplayStatics.h"
#include "BattleWidgetBase.h"
#include "Net/UnrealNetwork.h"
#include "BattleGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ABattleGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattleGameStateBase, AlivePerson);
	DOREPLIFETIME(ABattleGameStateBase, TotalPerson);

}
void ABattleGameStateBase::OnRep_AlivePerson()
{
	ABattlePlayerController* pc = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
	{
		if (pc->BattleWidgetObject)
		{

			pc->BattleWidgetObject->SetAliveCount(AlivePerson);
		}
	}
}
