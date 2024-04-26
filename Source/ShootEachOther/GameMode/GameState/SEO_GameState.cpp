// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_GameState.h"
#include "Player/SEOGameInstance.h"
#include "Net/UnrealNetwork.h"

ASEO_GameState::ASEO_GameState()
{
}

void ASEO_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		/*Game instance will store match setting at the Host main menu when host create the match*/
		if (GetGameInstance()) {
			if (USEOGameInstance* GI = Cast<USEOGameInstance>(GetGameInstance())) {
				MatchSetting = GI->GetMatchSetting();
			}
		}
	}
}

void ASEO_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TeamInfo);
	DOREPLIFETIME(ThisClass, MatchSetting);
}

TArray<FTeamInfo> ASEO_GameState::GetTeamsInfo() const
{
	return TeamInfo;
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

FMatchSetting ASEO_GameState::GetMatchSetting() const
{
	return MatchSetting;
}

void ASEO_GameState::CreateTeam_Implementation(int32 TeamID)
{
	TeamInfo.Add(FTeamInfo(TeamID, 4));
}

void ASEO_GameState::AddPlayerToTeam_Implementation(ASEO_PlayerState* ps, int32 TeamID, int index)
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
							return;
						}
					}
				}
				else {
					teamInfo.Players[index] = ps;
					teamInfo.AddMemberAmount();
				}

			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Already Contain Player"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("not valid team id"));
		}
		
	}
}

