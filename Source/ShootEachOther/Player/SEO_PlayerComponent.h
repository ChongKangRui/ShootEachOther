// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagCollection.h"
#include "SEO_PlayerComponent.generated.h"


class USEO_AttributeSet;
class USEOAbilitySystemComponent;
//class UInputConfig;
class USEOPawnData;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTEACHOTHER_API USEO_PlayerComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	USEO_PlayerComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData", ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const USEOPawnData> PawnData;

public:
	UFUNCTION(BlueprintCallable)
	void SetNativeInputEnable(bool Disable = false);

protected:

	USEO_AttributeSet* m_AttributeSet;
	TObjectPtr<APawn> m_Pawn;

	bool DisableNativeInput;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void InitializeInputBinding(UInputComponent* IC);
	/** Called for movement input */
	void Input_Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Input_Look(const FInputActionValue& Value);

	void Input_AbilityTagPressed(FGameplayTag tag);
	void Input_AbilityTagReleased(FGameplayTag tag);

	UFUNCTION()
	void OnRep_PawnData();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void InitializeInputContext();

	friend class AShootEachOtherPlayerController;


	

};
