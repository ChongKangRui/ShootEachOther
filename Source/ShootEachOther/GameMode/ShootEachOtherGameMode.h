// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericTeamAgentInterface.h"
#include "ShootEachOtherGameMode.generated.h"

class UMatchInfo;

UCLASS(minimalapi)
class AShootEachOtherGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShootEachOtherGameMode();

	UFUNCTION(BlueprintCallable)
	void BeginNewRound();

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
	

};



