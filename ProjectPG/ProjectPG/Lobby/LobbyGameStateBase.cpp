// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPlayerController.h"
#include "LobbyWidgetBase.h"
#include "kismet/GameplayStatics.h"
void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameStateBase, ConnectCount);
	DOREPLIFETIME(ALobbyGameStateBase, LeftTime);
}

void ALobbyGameStateBase::OnRep_ConnectCount()
{
	ALobbyPlayerController* pc = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc && pc->LobbyWidgetObject)
	{
		pc->LobbyWidgetObject->SetConnectCount(ConnectCount);
	}
}

void ALobbyGameStateBase::OnRep_LeftTime()
{
	ALobbyPlayerController* pc = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc && pc->LobbyWidgetObject)
	{
		pc->LobbyWidgetObject->SetPublicMessage(LeftTime);
	}
}


