// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SEOPawnData.generated.h"

/**
 * 
 */
class USEO_AbilitySet;
class UInputConfig;

UCLASS()
class SHOOTEACHOTHER_API USEOPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SEO|Ability")
	TObjectPtr<USEO_AbilitySet> AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SEO|Input")
	TObjectPtr<UInputConfig> InputSet;
};
