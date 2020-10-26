// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameInstance.h"

FString UPGGameInstance::GetUserID()
{
	if (UserID.IsEmpty())
	{
		UserID = FString::Printf(TEXT("Noname%d"), FMath::RandRange(1, 1000));

	}
	return UserID;
}

void UPGGameInstance::SetUserID(FString NewUserID)
{
	UserID = NewUserID;
}
