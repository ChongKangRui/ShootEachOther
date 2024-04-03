// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SEO_GameState.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	int32 TeamID;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<APlayerState>> Players;


	FLinearColor TeamColor;

	/*Mainly refering to member amount, not the length of the team*/
	int GetMemberAmount() const {
		return MemberAmount;
	}

	void AddMemberAmount() {
		MemberAmount++;
	}

	void RemovePlayer(APlayerState* pc) {
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

UCLASS()
class SHOOTEACHOTHER_API ASEO_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FTeamInfo> GetTeamsInfo() const;

	UFUNCTION(BlueprintCallable)
	FTeamInfo GetTeamInfo(int TeamID) const;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddPlayerToTeam(APlayerState* pc, int32 TeamID, int index = -1);

	UFUNCTION(Server, Reliable)
	void CreateTeam(int32 TeamID);
protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<FTeamInfo> TeamInfo;


private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
