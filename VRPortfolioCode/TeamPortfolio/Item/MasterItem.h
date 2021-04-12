// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../TeamPortfolio.h"
#include "GameFramework/Actor.h"
#include "ItemDataTable.h"
//#include "../Instance/TotalLog_GameInstance.h"
#include "MasterItem.generated.h"

UCLASS()
class TEAMPORTFOLIO_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//static UTotalLog_GameInstance* GameInstance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int ItemIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItemDataTable ItemData;
#pragma endregion

#pragma region Public Function
public:
	AMasterItem();
	void SettingByIndex(int32 Index, UWorld* World);
	void SettingByIndexWithoutWorld(int32 Index);
	void LoadingMesh();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

#pragma region Protected Function
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

};
