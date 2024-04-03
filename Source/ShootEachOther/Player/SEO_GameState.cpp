// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GameState.h"
#include "Net/UnrealNetwork.h"

TArray<FTeamInfo> ASEO_GameState::GetTeamsInfo() const
{
	return TArray<FTeamInfo>();
}


FTeamInfo ASEO_GameState::GetTeamInfo(int TeamID) const
{
	for (const FTeamInfo teamInfo : TeamInfo) {
		if (teamInfo.TeamID == TeamID) {
			return teamInfo;
		}
	}
	return FTeamInfo();
}

void ASEO_GameState::CreateTeam_Implementation(int32 TeamID)
{
	TeamInfo.Add(FTeamInfo(TeamID, 4));
}

void ASEO_GameState::AddPlayerToTeam_Implementation(APlayerState* ps, int32 TeamID, int index)
{
	UE_LOG(LogTemp, Error, TEXT("teamid in, %i"), TeamID);
	for (FTeamInfo& teamInfo : TeamInfo) {
		if (teamInfo.TeamID == TeamID) {

			if (!teamInfo.Players.Contains(ps)) {
				if (index <= -1) {
					for (int i = 0; i < teamInfo.Players.Num(); i++) {
						if (!teamInfo.Players[i]) {
							teamInfo.Players[i] = ps;
							teamInfo.AddMemberAmount();
							UE_LOG(LogTemp, Error, TEXT("add ps success 01"));
							return;
						}
					}
				}
				else {
					teamInfo.Players[index] = ps;
					teamInfo.AddMemberAmount();
					UE_LOG(LogTemp, Error, TEXT("add ps success 02"));
				}

			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Contain ps"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("not team id"));
		}
		
	}
}

void ASEO_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TeamInfo);
}
