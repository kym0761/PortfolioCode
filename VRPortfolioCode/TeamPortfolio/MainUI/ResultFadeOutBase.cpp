// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultFadeOutBase.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "../Instance/TotalLog_GameInstance.h"
#include "../MainUI/UI_PC.h"
#include "../Data/MonsterDataTable.h"

void UResultFadeOutBase::NativeConstruct()
{
	Super::NativeConstruct();

	for (int i = 0; i != TextBoxArray_Max; ++i)
	{	
		const FString BoxName = FString::Printf(TEXT("TB_%d"), i);
		UTextBlock* NewBox = Cast<UTextBlock>(GetWidgetFromName(*BoxName));
		TextBoxArray.Add(NewBox);		
	}

	SettingText();
}

void UResultFadeOutBase::SettingText()
{
	UTotalLog_GameInstance* GI = GetGameInstance<UTotalLog_GameInstance>();

	for (int i = 0; i != TextBoxArray_Max; ++i)
	{
		FString strText = GI->GetMonsterData(i).MonsterName;
		strText += FString::Printf(TEXT(" : %d"), GI->Monster_Record[EMonsterType(i)]);
		TextBoxArray[i]->SetText(FText::FromString(strText));
	}
}
