// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SEOMatchSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USEOMatchSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
};
