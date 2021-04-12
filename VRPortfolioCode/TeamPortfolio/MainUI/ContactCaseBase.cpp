// Fill out your copyright notice in the Description page of Project Settings.


#include "ContactCaseBase.h"
#include "../Item/MasterItem.h"
#include "ContactWidgetBase.h"

void UContactCaseBase::NativeConstruct()
{
	Super::NativeConstruct();

	ContactF = Cast<UContactWidgetBase>(GetWidgetFromName(TEXT("FocusContactWidget")));
	Contact1 = Cast<UContactWidgetBase>(GetWidgetFromName(TEXT("ContactWidget1")));
	Contact2 = Cast<UContactWidgetBase>(GetWidgetFromName(TEXT("ContactWidget2")));
	Contact3 = Cast<UContactWidgetBase>(GetWidgetFromName(TEXT("ContactWidget3")));

	Contact.Add(ContactF);
	Contact.Add(Contact1);
	Contact.Add(Contact2);
	Contact.Add(Contact3);
}

void UContactCaseBase::MakeChilren(TArray<AMasterItem*> PickList)
{
	int Index = 0;

	for (Index; Index != PickList.Num(); ++Index)
	{
		if (Contact[Index]->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayAnimationAppear_BlueprintImplement(Contact[Index]);
		}

		Contact[Index]->SetText(PickList[Index]->ItemData.ItemName);		
	}

	for (Index; Index != 4; ++Index)
	{
		Contact[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
}
