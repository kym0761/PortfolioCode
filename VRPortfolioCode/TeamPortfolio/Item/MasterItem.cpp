// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "../Instance/TotalLog_GameInstance.h"
#include "../TeamP_Basic/TeamP_BasicPlayer.h"
#include "../MainUI/UI_PC.h"


// Sets default values
AMasterItem::AMasterItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	bReplicates = true;
	ItemIndex = CN_NullItemIndex;
}

void AMasterItem::SettingByIndex(int32 Index, UWorld* World)
{
	UTotalLog_GameInstance*	GameInstance = Cast<UTotalLog_GameInstance>(UGameplayStatics::GetGameInstance(World));

	if (IsValid(GameInstance))
	{
		ItemData = GameInstance->GetItemData(Index);
		ItemIndex = Index;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("GameInstance is not Exist"));
	}
}

void AMasterItem::SettingByIndexWithoutWorld(int32 Index)
{
	UTotalLog_GameInstance*	GameInstance = Cast<UTotalLog_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (IsValid(GameInstance))
	{
		ItemData = GameInstance->GetItemData(Index);
		ItemIndex = Index;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("GameInstance is not Exist"));
	}
}

void AMasterItem::LoadingMesh()
{
	if (ItemIndex != CN_NullItemIndex)
	{
		SettingByIndex(ItemIndex, GetWorld());

		FStreamableManager Loader;
		StaticMesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
	}
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
	LoadingMesh();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::ProcessEndOverlap);
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ATeamP_BasicPlayer* Pawn = Cast<ATeamP_BasicPlayer>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			//Pawn->AddPickItem(this);
			AUI_PC* PC = Cast<AUI_PC>(Pawn->Controller);
			if (PC)
			{
				PC->AddPickItem(this);
			}
			//AUI_PC->
		}
	}
}

void AMasterItem::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ATeamP_BasicPlayer* Pawn = Cast<ATeamP_BasicPlayer>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			AUI_PC* PC = Cast<AUI_PC>(Pawn->Controller);
			PC->RemovePickItem(this);
		}
	}
}
