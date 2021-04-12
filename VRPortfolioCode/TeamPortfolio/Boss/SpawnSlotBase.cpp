// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSlotBase.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "BossCharacter.h"
#include "BossProjectileBase.h"
#include "BossObject.h"


void USpawnSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ImageBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ImageBorder")));

	CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CooldownBar")));

	SelectButton = Cast<UButton>(GetWidgetFromName(TEXT("SelectButton")));

	if (SelectButton)
	{
		//SelectButton->OnClicked.AddDynamic(this, &USpawnSlotBase::OnButtonClicked);
		SelectButton->OnReleased.AddDynamic(this, &USpawnSlotBase::OnButtonClicked);
	}


	APlayerController* pc = GetOwningPlayer();
	if (pc)
	{
		//Get BossPlayer so Can Access to SpawnClass.
		ABossCharacter* boss = Cast<ABossCharacter>(pc->GetPawn());
		if (boss)
		{
			int32 index = GetSlotNumber();
			if (index < boss->ProjectileClasses.Num())
			{
				//Access To CDO and get the Info.
				//FMonsterSpawnInfo monsterSpawnInfo = boss->SpawnClasses[index]->GetDefaultObject<AMonsterSpawnProjectile>()->MonsterSpawnInfo;
				FBossProjectileInfo projectileInfo = boss->ProjectileClasses[index]->GetDefaultObject<ABossProjectileBase>()->ProjectileInfo;

				//set UMG Icon.
				if (projectileInfo.Thumbnail)
				{
					ImageBorder->SetBrushFromTexture(projectileInfo.Thumbnail);
				}
			}
		}
	}


}

void USpawnSlotBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	UpdateCooldown();
}

void USpawnSlotBase::OnButtonClicked()
{
	//UE_LOG(LogClass, Warning, TEXT("%s's button Clicked."), *GetName());

	APlayerController* pc = GetOwningPlayer();
	if (pc &&pc->IsLocalPlayerController())
	{
		//Get BossPlayer so Can Access to SpawnClass.
		ABossCharacter* boss = Cast<ABossCharacter>(pc->GetPawn());
		if (boss)
		{
			int32 index = GetSlotNumber();

			if (index < boss->ProjectileClasses.Num() && boss->ProjectileClasses[index])
			{
				if (boss->bIsGrabbed)
				{
					UE_LOG(LogClass, Warning, TEXT("Projectile Spawn Failed. You Already Hold Something."));
					return;
				}

				if (boss->SpawnCooldown[index] < boss->MaxSpawnCooldown[index])
				{
					UE_LOG(LogClass, Warning, TEXT("Projectile Spawn Failed. Waiting for the Cooldown"));
					return;
				}

				//Call from Server To Spawn a Projectile.
				boss->SpawnProjectile(index);

			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("Invalid. you are trying to access %d. Current Spawn Class Size is %d"), index, boss->ProjectileClasses.Num());
			}

		}
	}

}

void USpawnSlotBase::UpdateCooldown()
{

	//Update Cooldown In Slot ProgressBar
	APlayerController* pc = GetOwningPlayer();
	if (pc && pc->IsLocalPlayerController())
	{
		ABossCharacter* boss = Cast<ABossCharacter>(pc->GetPawn());
		if (boss)
		{
			int32 index = GetSlotNumber();
			if (index < boss->SpawnCooldown.Num() && CooldownBar)
			{
				CooldownBar->SetPercent(boss->SpawnCooldown[index] / boss->MaxSpawnCooldown[index]);
			}
		}
	}
}

int32 USpawnSlotBase::GetSlotNumber() const
{
	//Get Number. ex) "SpawnSlot7" == 7
	FString widgetName = GetName();
	widgetName.RemoveFromStart(TEXT("SpawnSlot"));
	return FCString::Atoi(*widgetName);
}
