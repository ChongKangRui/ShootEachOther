// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GM_MainMenu.generated.h"

/**
 * 
 * For now may not using this class, I use blueprint node to join online session instead
 * 
 */
UENUM(BlueprintType)
enum ECreateSessionResults : uint8 {
Success,
Fail,
};

UCLASS()
class SHOOTEACHOTHER_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_MainMenu();
	/*
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Result"))
	void CreateSession(const int MaxPlayer, TEnumAsByte<ECreateSessionResults>& Result ,const bool UseLAM = false, const FName SessionName = "Session");

	UFUNCTION(BlueprintCallable)
	void FindSession(const int MaxSearchResult, const bool UseLAM = false);

	UFUNCTION(BlueprintCallable)
	void JoinSession(const FString& sessionID);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetFoundedSession() const;

	virtual void OnFindSessionComplete(bool IsSuccess);
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
protected:
	virtual void BeginPlay() override;

private:
	const FOnlineSessionSearchResult GetSessionResultFromString(const FString Session) const;

private:
	IOnlineSessionPtr SessionInterface;
	TArray<FString> SessionIDs;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	bool SearchFinished;*/
};
