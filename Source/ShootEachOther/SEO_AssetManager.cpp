// Fill out your copyright notice in the Description page of Project Settings.


#include "SEO_AssetManager.h"
#include "AbilitySystemGlobals.h"

void USEO_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();

	UE_LOG(LogTemp, Warning, TEXT("Asset manager would like to say Hello"));
}
