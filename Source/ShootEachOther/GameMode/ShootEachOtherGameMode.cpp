// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootEachOtherGameMode.h"
#include "MatchInfo.h"
#include "Player/SEO_PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AShootEachOtherGameMode::AShootEachOtherGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AShootEachOtherGameMode::BeginPlay()
{
	Super::BeginPlay();
	ServerCreateTeam();
}

void AShootEachOtherGameMode::PostLogin(APlayerController* pc)
{
	Super::PostLogin(pc);
	AssignTeamToPlayer(pc, GetLeastMemberOfTeam());
}

void AShootEachOtherGameMode::ServerCreateTeam()
{
	if (MatchInformation) {
		switch (MatchInformation->MatchType)
		{
		case EMatchType::FiveRoundThreeWin:

			break;
		case EMatchType::ThreeRoundTwoWin:

			break;
		
		}
	}
}

void AShootEachOtherGameMode::AssignTeamToPlayer(APlayerController* pc, int32 TeamId)
{
	if(ASEO_PlayerState* ps = pc->GetPlayerState<ASEO_PlayerState>())
		ps->SetGenericTeamId(FGenericTeamId(TeamId));


}

int32 AShootEachOtherGameMode::GetLeastMemberOfTeam() const
{
	//int TeamAMemberNum, TeamBMemberNum;



	return int32();
}

void AShootEachOtherGameMode::BeginNewRound()
{
}
