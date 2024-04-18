// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIBotController.generated.h"

/**
 * 
 */
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

protected: 

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UAIPerceptionComponent> MyPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	//TObjectPtr<class UBlackboardComponent> BlackboardComp;

//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	//TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComp;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:

	TObjectPtr<class ASEO_PlayerState> SEO_PlayerState;
	class AShootEachOtherCharacter* AI;
};
