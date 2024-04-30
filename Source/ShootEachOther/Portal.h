// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class USceneCaptureComponent2D;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateSceneCamera();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<APortal> LinkedPortal;

	USceneComponent* SceneRoot;

private:
	//This should help to transform the player camera vision to the portal vision vector
	FVector ConvertPlayerVisionToLinkedPortalVision(const FVector& Direction);
};
