// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootEachOtherGameMode.h"

#include "Player/SEO_PlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "GameState/SEO_GameState.h"

AShootEachOtherGameMode::AShootEachOtherGameMode()
{
}

void AShootEachOtherGameMode::PostLogin(APlayerController* pc)
{
	Super::PostLogin(pc);
	if (!gameState) {
		gameState = GetGameState<ASEO_GameState>();
		ServerCreateTeam();
	}

	AssignTeamToPlayer(pc, GetLeastMemberOfTeam());
}

void AShootEachOtherGameMode::ServerCreateTeam()
{
	gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamA));
	gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamB));

	
		switch (gameState->GetMatchSetting().MatchType)
		{
		case EMatchType::FiveRoundThreeWin:
			//gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamA));
			//gameState->CreateTeam(GetTeamIDFromTeamEnum(ETeamType::TeamB));
			break;
		case EMatchType::ThreeRoundTwoWin:

			break;
		
		}
	
}

void AShootEachOtherGameMode::AssignTeamToPlayer(APlayerController* pc, int32 TeamId)
{
	if (ASEO_PlayerState* ps = pc->GetPlayerState<ASEO_PlayerState>()) {
		ps->SetGenericTeamId(FGenericTeamId(TeamId));

		gameState->AddPlayerToTeam(ps, TeamId);
		OnTeamIDAssigned.Broadcast(pc, TeamId);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("invalid cast ps"));
	}


}

int32 AShootEachOtherGameMode::GetLeastMemberOfTeam() const
{
	if (gameState) {
		int TeamALength = gameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamA)).GetMemberAmount();
		int TeamBLength = gameState->GetTeamInfo(GetTeamIDFromTeamEnum(ETeamType::TeamB)).GetMemberAmount();

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
