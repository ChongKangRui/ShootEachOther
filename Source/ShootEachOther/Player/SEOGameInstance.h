// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameMode/GameState/SEO_GameState.h"
#include "SEOGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API USEOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	UFUNCTION(BlueprintCallable)
	void SetMatchSetting(const FMatchSetting setting);

	FMatchSetting GetMatchSetting() const;
protected:
	FMatchSetting MatchSetting = FMatchSetting();
};
