// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SEOPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API ASEOPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
