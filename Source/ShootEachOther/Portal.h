// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class USceneCaptureComponent2D;
class UBoxComponent;
class AShootEachOtherPlayerController;

UCLASS()
class SHOOTEACHOTHER_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

public:
	APortal* GetLinkedPortal() const;
	USceneCaptureComponent2D* GetSceneCaptureComp() const;
protected:

	virtual void Tick(float DeltaTime) override;

	void UpdateSceneCamera();
	void UpdateOverlapedCharacterTeleport();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<APortal> LinkedPortal;

	USceneComponent* SceneRoot;
	TObjectPtr<AShootEachOtherPlayerController> LocalPlayer;
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//This should help to transform the player camera vision to the portal vision vector
	FVector ConvertPlayerDirectionToLinkedPortalDirection(const FVector& Direction);
	bool IsPointCrossingPortal(const FVector& CameraLocation, const FVector& PortalLocation, const FVector& PortalNormal);
	bool IsPlayerInfront(const FVector& CameraLocation, const FVector& PortalLocation, const FVector& PortalNormal);
	bool IsIntersect(const FVector& CameraLocation, const FVector& LastCameraLocation, const FVector& PortalLocation, const FVector& PortalNormal);

	void TeleportPlayer();
	void UpdateVelocity();
private:
	bool IsPlayerOverlapping = false;

	bool LastPlayerInFront = false;
	FVector LastCameraLocation = FVector();

};
