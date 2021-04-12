// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_GM.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

AActor* ABase_GM::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	if (PlayerStartArray.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);
	}

	if (IncomingName == TEXT(""))
	{		
		return PlayerStartArray[0];
	}
	else
	{
		for (int Index = 0; Index != PlayerStartArray.Num(); ++Index)
		{
			APlayerStart* PS = Cast<APlayerStart>(PlayerStartArray[Index]);

			if (IncomingName == PS->PlayerStartTag.ToString())
			{
				return PS;
			}
		}

		//없는 경우
		return PlayerStartArray[0];
	}
}

void ABase_GM::Replace_AllPlayers_Implementation()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), OutActors);

	for (int Index = 0; Index != OutActors.Num(); ++Index)
	{
		APlayerController* PC = Cast<APlayerController>(OutActors[Index]);

		if (PC->ActorHasTag(TEXT("Defence")))
		{
			AActor* Defence = FindPlayerStart_Implementation(PC, TEXT("Defence"));
			PC->GetPawn()->SetActorTransform(Defence->GetTransform());
		}
		else
		{
			AActor* Offence = FindPlayerStart_Implementation(PC, TEXT("Offence"));
			PC->GetPawn()->SetActorTransform(Offence->GetTransform());
		}
	}
}

void ABase_GM::Replace_Player_Implementation(AController* Player, FName Tag)
{
	AActor* PS = FindPlayerStart_Implementation(Player, Tag.ToString());

	if (IsValid(Player->GetPawn()))
	{
		Player->GetPawn()->SetActorTransform(PS->GetTransform());
		Player->GetPawn()->SetActorHiddenInGame(false);
	}
}
