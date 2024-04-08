// Fill out your copyright notice in the Description page of Project Settings.


#include "SEOGameInstance.h"

void USEOGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	//Super::LoadComplete(LoadTime, MapName);
}

void USEOGameInstance::SetMatchSetting(const FMatchSetting setting)
{
	MatchSetting = setting;
}

FMatchSetting USEOGameInstance::GetMatchSetting() const
{
	return MatchSetting;
}
