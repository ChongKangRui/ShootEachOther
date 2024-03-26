// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UWeaponInstance;

UCLASS()
class SHOOTEACHOTHER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetWeaponData(UWeaponInstance* WI);

	UFUNCTION(BlueprintCallable)
	UWeaponInstance* GetWeaponData() const;

	UFUNCTION(BlueprintCallable)
	void SetMeshOwnerCanSee(bool CanOwnerSee);

	USceneComponent* GetTraceStart() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetMeleeTraceStart() const;

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetMeleeTraceEnd() const;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> DefaultsSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> ShootVFXPoint;

	/*Only use this when ads on weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> WeaponShootTraceStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> HandIK_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> ADSCameraPoint;
	
	/*Use this to keep the instancce reference when drop weapon*/
	UPROPERTY(BlueprintReadOnly, Replicated)
	TObjectPtr<UWeaponInstance> WeaponInstance;

	/*Store the array at the beginplay, later can use to hide or show to owner*/
	TMap<USkeletalMeshComponent*, TObjectPtr<USkeletalMesh>> skeletalMeshes;
	TMap<UStaticMeshComponent*, TObjectPtr<UStaticMesh>> staticMeshes;

private:
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
