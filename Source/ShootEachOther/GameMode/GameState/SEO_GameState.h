// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SEO_GameState.generated.h"

/**
 * 
 */
class ASEO_PlayerState;
class UBotSpawnComponent;

USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	int32 TeamID;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ASEO_PlayerState>> Players;

	FLinearColor TeamColor;

	/*Mainly refering to member amount, not the length of the team*/
	int GetMemberAmount() const {
		return MemberAmount;
	}

	void AddMemberAmount() {
		MemberAmount++;
	}

	void RemovePlayer(ASEO_PlayerState* pc) {
		Players.Remove(pc);
	}

	FTeamInfo() : TeamID(-1), TeamColor(), MemberAmount(0){
		Players.Init(nullptr, 4);
		
	}

	FTeamInfo(int32 teamID, int TeamSize) : TeamID(teamID), TeamColor(), MemberAmount(0) {
		Players.Init(nullptr, TeamSize);
	}

protected:
	UPROPERTY(BlueprintReadOnly)
	int MemberAmount;


};

UENUM(BlueprintType)
enum class EMatchType : uint8 {
	/*2 teams fight*/
	ThreeRoundTwoWin,
	FiveRoundThreeWin,
	/*1vs1*/
	CowBoyDuel,
	/*No team, everyone is enemy*/
	OnlyOneSurvive
};

USTRUCT(BlueprintType)
struct FMatchSetting {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFriendlyDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMatchType MatchType = EMatchType::ThreeRoundTwoWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShoppingTimeBeforeRoundBegin = 15.0f;

	FMatchSetting() {

	}
};

UCLASS()
class SHOOTEACHOTHER_API ASEO_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASEO_GameState();

	UFUNCTION(BlueprintPure)
	FMatchSetting GetMatchSetting() const;

	UFUNCTION(BlueprintCallable)
	TArray<FTeamInfo> GetTeamsInfo() const;

	UFUNCTION(BlueprintCallable)
	FTeamInfo GetTeamInfo(int TeamID) const;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddPlayerToTeam(ASEO_PlayerState* pc, int32 TeamID, int index = -1);

	UFUNCTION(Server, Reliable)
	void CreateTeam(int32 TeamID);

protected:
	void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<FTeamInfo> TeamInfo;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FMatchSetting MatchSetting;


};
