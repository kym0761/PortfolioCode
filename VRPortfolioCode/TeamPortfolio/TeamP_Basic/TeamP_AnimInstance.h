// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TeamP_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UTeamP_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsCrouched : 1; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsSprint : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsIronsight : 1;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsShotgun : 1;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsFireAnim : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bFireShotgun : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsWeaponChange : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsReload : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsDead : 1;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsGranade : 1;




	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Status")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Status")
	float Direction;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float AimYaw;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float AimPitch;



	UFUNCTION()
	void Animnotify_ReloadEnd(UAnimNotify* Animnotify);

	UFUNCTION()
	void Animnotify_Throw_Ready(UAnimNotify* Animnotify);

	UFUNCTION()
	void Animnotify_Throw_End(UAnimNotify* Animnotify);


	uint64 bThrowReady : 1;
	 
};
