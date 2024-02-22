// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SEO_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USEO_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;
	
};
