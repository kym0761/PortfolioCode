// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerState.h"
#include "Net/UnrealNetwork.h"
void ABattlePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ABattlePlayerState, PlayerRank);

}
