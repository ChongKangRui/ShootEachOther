// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SEOPlayerCameraManager.h"

void ASEOPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

}

void ASEOPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
	//For the portal effect
	OutVT.POV.AspectRatioAxisConstraint = EAspectRatioAxisConstraint::AspectRatio_MajorAxisFOV;
}
