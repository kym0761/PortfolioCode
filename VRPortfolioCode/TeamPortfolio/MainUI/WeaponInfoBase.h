// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInfoBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UWeaponInfoBase : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* WeaponName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* BulletNum;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* BulletMaxNum;
#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString NewName);

	UFUNCTION(BlueprintCallable)
	void SetIBulletNum(int Num);

	UFUNCTION(BlueprintCallable)
	void SetIBulletMaxNum(int Num);
	
#pragma endregion
};
