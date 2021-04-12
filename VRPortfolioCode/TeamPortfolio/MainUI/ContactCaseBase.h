// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContactCaseBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UContactCaseBase : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UContactWidgetBase*> Contact;

	class UContactWidgetBase* ContactF;
	class UContactWidgetBase* Contact1;
	class UContactWidgetBase* Contact2;
	class UContactWidgetBase* Contact3;

#pragma endregion

#pragma region Public Member
public:
	virtual void NativeConstruct() override;

	
	void MakeChilren(TArray<class AMasterItem*> PickList);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationAppear_BlueprintImplement(class UContactWidgetBase* UCTWidgetBase);


#pragma endregion
};
