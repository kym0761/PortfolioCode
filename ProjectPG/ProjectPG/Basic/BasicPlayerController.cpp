// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerController.h"
#include "BasicPlayerCameraManager.h"
ABasicPlayerController::ABasicPlayerController()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}