// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIBotController.generated.h"

/**
 * 
 */
class UDA_AIProperty;
class AShootEachOtherCharacter;

UCLASS()
class SHOOTEACHOTHER_API AAIBotController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIBotController();

	virtual void OnPossess(APawn* PawnToProcess) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& actor)const override;

public:
	UFUNCTION(BlueprintPure, Category = "AI")
	const UDA_AIProperty* GetAIProperty() const;

	UFUNCTION(BlueprintPure, Category = "AI")
	const AActor* GetCurrentTarget() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void ClearTarget();

protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData")
	TObjectPtr<const UDA_AIProperty> AIData;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UAIPerceptionComponent> MyPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:

	TObjectPtr<class ASEO_PlayerState> SEO_PlayerState;
	AShootEachOtherCharacter* AI;

	AShootEachOtherCharacter* Target;
};
