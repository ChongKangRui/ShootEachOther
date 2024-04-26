// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "BotSpawnComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTEACHOTHER_API UBotSpawnComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UBotSpawnComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void AddOneBot(const int32 TeamID, const int Index);
	UFUNCTION(BlueprintCallable)
	void RemoveBot(const int32 TeamID, const int index);

public:
	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<class AAIController> BotControllerClass;

protected:
	void BeginPlay() override;

protected:
	class ASEO_GameState* owner;

};
