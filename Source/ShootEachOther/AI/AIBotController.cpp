// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBotController.h"

#include "GameplayAbility/SEOAbilitySystemComponent.h"
#include "GameplayAbility/SEO_AbilitySet.h"
#include "Character/SEOPawnData.h"
#include "Character/ShootEachOtherCharacter.h"

#include "Player/SEO_PlayerState.h"
#include "Player/SEO_PlayerComponent.h"

#include "SEO_GlobalFunctionLibrary.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAIBotController::AAIBotController()
{
	bWantsPlayerState = true;
	PrimaryActorTick.bCanEverTick = true;

	// Create the Perception Component
	MyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Setup perception configuration
	MyPerceptionComponent->ConfigureSense(*SightConfig);
	MyPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	// Initialize behavior tree component
	//BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AAIBotController::OnPossess(APawn* PawnToProcess)
{
	Super::OnPossess(PawnToProcess);

	AI = Cast<AShootEachOtherCharacter>(PawnToProcess);

	if (!AI) {
		USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid AI Character Ref");
		return;
	}

	//Set up Ability System Component
	USEOAbilitySystemComponent* asc = AI->GetSEOAbilitySystemComponent();
	if (asc) {
		asc->SetAvatarActor(PawnToProcess);

		if (asc->IsOwnerActorAuthoritative()) {
			if (const USEO_PlayerComponent* upc = PawnToProcess->GetComponentByClass<USEO_PlayerComponent>()) {
				if (!upc->PawnData) {
					USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid Pawn Data");
					return;
				}
				if (USEO_AbilitySet* Ability = upc->PawnData->AbilitySet) {
					Ability->GiveAbilityToASC(asc);
					SetActorTickEnabled(true);
					USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "give ability to AI success");
				}

			}
		}
	}

	//Add Perception Updated function
	MyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIBotController::OnPerceptionUpdated);

	/*if (BehaviorTreeComp && BehaviorTreeComp->GetBlackboardComponent())
	{
		BehaviorTreeComp->GetBlackboardComponent()->InitializeBlackboard(*(BehaviorTreeComp->GetCurrentTree()->BlackboardAsset));
	}*/

	//BehaviorTreeComp->StartLogic();
	//BehaviorTreeComp->CacheBlackboardComponent(BlackboardComp);
	//BlackboardComp->InitializeBlackboard(*BlackboardComp->GetBlackboardAsset());
	

}

void AAIBotController::BeginPlay()
{
	Super::BeginPlay();
	SEO_PlayerState = GetPlayerState<ASEO_PlayerState>();


}

void AAIBotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!AI) {
		//USEO_GlobalFunctionLibrary::SEO_Log(this, ELogType::Error, "Invalid AI Character Ref");
		return;
	}

	if (USEOAbilitySystemComponent* asc = AI->GetSEOAbilitySystemComponent()) {
		asc->ProcessAllAbility(DeltaTime);
	}
}

ETeamAttitude::Type AAIBotController::GetTeamAttitudeTowards(const AActor& actor) const
{
	const AShootEachOtherCharacter* character = Cast<AShootEachOtherCharacter>(&actor);
	if (!character)
		return ETeamAttitude::Neutral;

	if (character->GetSEOPlayerState()->GetTeamID() == 0)
		return ETeamAttitude::Hostile;

	if (character->GetSEOPlayerState()->GetTeamID() == SEO_PlayerState->GetTeamID())
		return ETeamAttitude::Friendly;
	else
		return ETeamAttitude::Hostile;

}

void AAIBotController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
	for (const auto& Actor : UpdatedActors)
	{
		if (Actor && Actor->IsA<ACharacter>())
		{
			
				UE_LOG(LogTemp, Error, TEXT("%s"), *Actor->GetName());
				// The AI character sees the player
				// Implement logic to react to player detection
				// For example, set the player as an enemy
			
			
			
		}
	}
}
