// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_AIProperty.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API UDA_AIProperty : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/*Minimum deviation for X and Y*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot")
	FVector2D MinRandomDeviation;
	/*Maximum deviation for X and Y*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot")
	FVector2D MaxRandomDeviation;

	/*This float will time into the shoot rate so it get the maximum delay*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot")
	float ShootDelay = 2.0f;
};
