// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterLobby_GS.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "../MainUI/UI_PC.h"
#include "Lobby_WidgetBase.h"


void AMasterLobby_GS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterLobby_GS, LeftTime);
}

void AMasterLobby_GS::OnRep_LeftTime()
{
	AUI_PC* PC = Cast<AUI_PC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->SetMessage(LeftTime);
	}
	else if(PC)
	{
		PC->AddTimeWidget();
		PC->LobbyWidgetObject->SetMessage(LeftTime);
	}
}
