// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponInstance.h"
#include "Character/ShootEachOtherCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	

	DefaultsSceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("DefaultsSceneRoot"));
	SetRootComponent(DefaultsSceneRoot);
	ShootVFXPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ShootVFXPoint"));
	ShootVFXPoint->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	WeaponShootTraceStart = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("WeaponShootTraceStart"));
	WeaponShootTraceStart->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	HandIK_L = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("HandIK Left"));
	HandIK_L->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	ADSCameraPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ADS Camera Point"));
	ADSCameraPoint->AttachToComponent(DefaultsSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void AWeaponBase::SetWeaponData(UWeaponInstance* WI)
{
	WeaponInstance = WI;
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

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



USceneComponent* AWeaponBase::GetTraceStart() const
{
	return WeaponShootTraceStart;
}

