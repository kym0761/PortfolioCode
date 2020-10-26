// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PrintLog.h"

void UBTService_PrintLog::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogClass, Warning, TEXT("BTService Tick"));

}