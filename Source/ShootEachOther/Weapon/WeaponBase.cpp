// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponInstance.h"
#include "Character/ShootEachOtherCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DefaultsSceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("DefaultsSceneRoot"));
	SetRootComponent(DefaultsSceneRoot);
	ShootVFXPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ShootVFXPoint"));
	ShootVFXPoint->SetupAttachment(DefaultsSceneRoot);

	WeaponShootTraceStart = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("WeaponShootTraceStart"));
	WeaponShootTraceStart->SetupAttachment(DefaultsSceneRoot);

	HandIK_L = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("HandIK Left"));
	HandIK_L->SetupAttachment(DefaultsSceneRoot);

	ADSCameraPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ADS Camera Point"));
	ADSCameraPoint->SetupAttachment(DefaultsSceneRoot);

}

void AWeaponBase::SetWeaponData_Implementation(UWeaponInstance* WI)
{
	WeaponInstance = WI;
}

UWeaponInstance* AWeaponBase::GetWeaponData() const
{
	return WeaponInstance;
}

void AWeaponBase::SetMeshOwnerCanSee(bool CanOwnerSee)
{

	for (auto&[Comp, Mesh] : skeletalMeshes) {
		
		if (!CanOwnerSee) {
			Comp->SetSkeletalMesh(nullptr);
		}
		else {
			Comp->SetSkeletalMesh(Mesh);
		}
	}

	for (auto&[Comp, Mesh] : staticMeshes) {
		if (!CanOwnerSee) {
			Comp->SetStaticMesh(nullptr);
		}
		else {
			Comp->SetStaticMesh(Mesh);
		}

	}

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (AShootEachOtherCharacter* owner = Cast<AShootEachOtherCharacter>(GetOwner())) {
		if (owner->IsLocallyControlled()) {
			TArray<USkeletalMeshComponent*> SKeletalMeshComps;
			TArray<UStaticMeshComponent*> StaticMeshComps;
			GetComponents(SKeletalMeshComps);
			GetComponents(StaticMeshComps);

			

			for (USkeletalMeshComponent* smc : SKeletalMeshComps) {
				if (smc && smc->SkeletalMesh) {
					skeletalMeshes.Add(smc, smc->SkeletalMesh);
				}
			}
			for (UStaticMeshComponent* smc : StaticMeshComps) {
				if (smc && smc->GetStaticMesh()) {
					staticMeshes.Add(smc, smc->GetStaticMesh());
				}

			}
		}

	}

	
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, WeaponInstance);

}

bool AWeaponBase::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	Channel->ReplicateSubobject(WeaponInstance, *Bunch, *RepFlags);
	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}


USceneComponent* AWeaponBase::GetTraceStart() const
{
	return WeaponShootTraceStart;
}

