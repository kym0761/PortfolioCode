// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JetFireCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStunSoundSignature);

UCLASS(config=Game)
class AJetFireCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/*Particle for Stun Effect*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* StunEffect;

protected:

	//current Fuel
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float Fuel;
	//JetPower for upward
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 2000.0f))
		float JetPower;
	//jetpower for forward
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 500.0f))
		float JetPower_Forward;
	//Reduction ratio
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float FuelReduction;
	//Charge Ratio
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float FuelCharge;
	//not used
	UPROPERTY(VisibleAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float FuelCharge_default;
	//Lerp for in air
	UPROPERTY(EditAnywhere, Category = Fuel, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float LerpOffset;

	const float MaxFuel = 100.0f;

	//stun Count
	UPROPERTY(EditAnywhere, Category = Stun)
		float StunCount;
	//is character stunned?
	UPROPERTY(EditAnywhere, Category = Stun)
		bool IsStunned;
	//how much time to enable character from stun?
	const float MaxStunCount = 1.5f;

	//Delegate to Play Stun Sound
	UPROPERTY(BlueprintAssignable, Category = Stun)
		FStunSoundSignature StunSoundDelegate;


	UFUNCTION(BlueprintCallable, Category = "JetCharacter|Collision")
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void FireJet(float Value);
	virtual void FireJet_Implementation(float Value);

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay();

public:
	AJetFireCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//be Used in UI.
	UFUNCTION(BlueprintCallable , Category = "JetCharacter|Fuel")
		float GetCurrentFuel() const;
	//be Used in UI. 2
	UFUNCTION(BlueprintCallable, Category = "JetCharacter|Fuel")
		float GetMaxFuel() const;

	//Detect Stun.
	UFUNCTION(BlueprintCallable, Category = "JetCharacter|Stun")
		bool GetIsStunned() const;
};
