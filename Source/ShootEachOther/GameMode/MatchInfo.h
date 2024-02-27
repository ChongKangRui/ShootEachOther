// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MatchInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMatchType : uint8 {
	ThreeRoundTwoWin,
	FiveRoundThreeWin,
	/*1vs1*/
	CowBoyDuel,
	/*No team, everyone is enemy*/
	Chaostic
};

UENUM(BlueprintType)
enum class ETeamType : uint8 {
	Solo = 0,
	TeamA = 1,
	TeamB = 2
};

UCLASS()
class SHOOTEACHOTHER_API UMatchInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	EMatchType MatchType;

	/*Move the linear color later to a struct if necessary*/
	UPROPERTY(EditDefaultsOnly)
	TMap<ETeamType, FLinearColor> TeamMeshColor;

	UPROPERTY(EditDefaultsOnly)
	float ShoppingTimeBeforeRoundBegin = 15.0f;

	/*Determine whether player can change team, do some modification before match actually start*/
	UPROPERTY(EditDefaultsOnly)
	bool HasReadyState = true;

};
