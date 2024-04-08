// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenericTeamAgentInterface.h"
#include "MatchInfo.h"
#include "ShootEachOtherGameMode.generated.h"


class AShootEachOtherPlayerController;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamIDAssigned, APlayerController*, PC, int32, ID);
UCLASS(minimalapi)
class AShootEachOtherGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AShootEachOtherGameMode();

	UFUNCTION(BlueprintImplementableEvent)
	void BeginNewRound();

	UFUNCTION(BlueprintPure)
	int32 GetTeamIDFromTeamEnum(const ETeamType& team) const;

	UFUNCTION(BlueprintPure)
	ETeamType GetTeamEnumFromID(const int32& genericid) const;

	UPROPERTY(BlueprintAssignable)
	FOnTeamIDAssigned OnTeamIDAssigned;

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* pc) override;
	
	void ServerCreateTeam();
	void AssignTeamToPlayer(APlayerController* pc, int32 TeamId);
	int32 GetLeastMemberOfTeam() const;


protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMatchInfo> MatchInformation;

	/*Winning team will be added into this*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FGenericTeamId> TeamMatchResultRecord;

private:
	class ASEO_GameState* gameState;

};



