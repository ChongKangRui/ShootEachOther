// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootEachOtherGameMode.h"

#include "Player/SEO_PlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "GameState/SEO_GameState.h"
#include "GameplayTagCollection.h"

AShootEachOtherGameMode::AShootEachOtherGameMode()
{
}

void AShootEachOtherGameMode::PostLogin(APlayerController* pc)
{
	Super::PostLogin(pc);
	if (!SEO_GameState) {
		SEO_GameState = GetGameState<ASEO_GameState>();
		ServerCreateTeam();
	}

	AssignTeamToPlayer(pc, GetLeastMemberOfTeam());
}

void AShootEachOtherGameMode::ServerCreateTeam()
{
	SEO_GameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamA));
	SEO_GameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamB));

	
		switch (SEO_GameState->GetMatchSetting().MatchType)
		{
		case EMatchType::FiveRoundThreeWin:
			//gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamA));
			//gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamB));
			break;
		case EMatchType::ThreeRoundTwoWin:

			break;
		
		}
	
}

void AShootEachOtherGameMode::AssignTeamToPlayer(APlayerController* pc, int32 TeamId, int TeamIndex)
{
	if (ASEO_PlayerState* ps = pc->GetPlayerState<ASEO_PlayerState>()) {
		ps->SetGenericTeamId(FGenericTeamId(TeamId));

		SEO_GameState->AddPlayerToTeam(ps, TeamId, TeamIndex);
		OnTeamIDAssigned.Broadcast(pc, TeamId);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("invalid cast ps"));
	}


}

void AShootEachOtherGameMode::ResetMatchSetting()
{
	TeamMatchResultRecord.Empty();
	SEO_GameState->ResetAliveState();
}

void AShootEachOtherGameMode::CheckIfRoundEnd()
{
	if (SEO_GameState) {
		const FTeamInfo TeamA = SEO_GameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamA));
		const FTeamInfo TeamB = SEO_GameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamB));

		if (TeamA.AliveMember <= 0) {
			TeamMatchResultRecord.Add(GetTeamEnumFromID(TeamB.TeamID));
			OnRoundEnded.Broadcast(GetTeamEnumFromID(TeamB.TeamID));
		}
		if (TeamB.AliveMember <= 0) {
			TeamMatchResultRecord.Add(GetTeamEnumFromID(TeamA.TeamID));
			OnRoundEnded.Broadcast(GetTeamEnumFromID(TeamA.TeamID));
		}


	}
}

int32 AShootEachOtherGameMode::GetLeastMemberOfTeam() const
{
	if (SEO_GameState) {
		int TeamALength = SEO_GameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamA)).GetMemberAmount();
		int TeamBLength = SEO_GameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamB)).GetMemberAmount();

		UE_LOG(LogTemp, Error, TEXT("Team Length, %i, %i"), TeamALength, TeamBLength);

		if (TeamALength > TeamBLength) {
			return GetTeamIDFromTeamEnum(ETeamType::TeamB);
			UE_LOG(LogTemp, Error, TEXT("why team b"));
		}
		else {
			return GetTeamIDFromTeamEnum(ETeamType::TeamA);
			UE_LOG(LogTemp, Error, TEXT("why team a"));
		}
	}
	return -1;
}

int32 AShootEachOtherGameMode::GetTeamIDFromTeamEnum(const ETeamType& team) const
{
	switch (team)
	{
	case ETeamType::TeamA:
		return 1;
	case ETeamType::TeamB:
		return 2;
	case ETeamType::Solo:
		return 0;
	}
	return -1;
}

ETeamType AShootEachOtherGameMode::GetTeamEnumFromID(const int32& genericid) const
{
	switch (genericid)
	{
	case 1:
		return ETeamType::TeamA;
	case 2:
		return ETeamType::TeamB;
	case 0:
		return ETeamType::Solo;
	}
	return ETeamType();
}
