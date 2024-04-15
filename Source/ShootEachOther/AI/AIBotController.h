// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIBotController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API AAIBotController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIBotController();

	void OnPossess(APawn* PawnToProcess) override;
};
