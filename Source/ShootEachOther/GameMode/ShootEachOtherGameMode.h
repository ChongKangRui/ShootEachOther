// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenericTeamAgentInterface.h"
#include "MatchInfo.h"
#include "ShootEachOtherGameMode.generated.h"


class AShootEachOtherPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamIDAssigned, APlayerController*, PC, int32, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEnded, ETeamType, WinningTeam);
UCLASS(minimalapi)
class AShootEachOtherGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AShootEachOtherGameMode();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginNewRound();

	UFUNCTION(BlueprintCallable)
	void AssignTeamToPlayer(APlayerController* pc, int32 TeamId, int TeamIndex = -1);

	UFUNCTION(BlueprintCallable)
	void ResetMatchSetting();

	UFUNCTION(BlueprintPure)
	int32 GetTeamIDFromTeamEnum(const ETeamType& team) const;

	UFUNCTION(BlueprintPure)
	ETeamType GetTeamEnumFromID(const int32& genericid) const;

	void CheckIfRoundEnd();
public:
	UPROPERTY(BlueprintAssignable)
	FOnTeamIDAssigned OnTeamIDAssigned;

	UPROPERTY(BlueprintAssignable)
	FOnRoundEnded OnRoundEnded;
protected:
	virtual void PostLogin(APlayerController* pc) override;

	void ServerCreateTeam();
	
	int32 GetLeastMemberOfTeam() const;

protected:
	/*Winning team will be added into this*/
	UPROPERTY(BlueprintReadOnly)
	TArray<ETeamType> TeamMatchResultRecord;

	UPROPERTY(BlueprintReadOnly)
	class ASEO_GameState* SEO_GameState;
	

};



