// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby_GS.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby_PC.h"
#include "Lobby_WidgetBase.h"


void ALobby_GS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobby_GS, LeftTime);
}

void ALobby_GS::OnRep_LeftTime()
{
	ALobby_PC* PC = Cast<ALobby_PC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->SetMessage(LeftTime);
	}
}
