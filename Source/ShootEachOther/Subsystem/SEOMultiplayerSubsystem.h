// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "SEOMultiplayerSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SHOOTEACHOTHER_API FSessionInfo {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayer;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayer;

	UPROPERTY(BlueprintReadOnly)
	int32 Ping;

	FSessionInfo() : SessionName("Session"), CurrentPlayer(0), MaxPlayer(0), Ping(0) {}
	FSessionInfo(FString name, int32 currentPlayer, int32 maxPlayer, int32 ping) : SessionName(name), CurrentPlayer(currentPlayer), MaxPlayer(maxPlayer), Ping(ping){}


};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundSession, const TArray<FSessionInfo>&, sessionInfo);
UCLASS()
class SHOOTEACHOTHER_API USEOMultiplayerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void CreateSession(const int MaxPlayer, const bool UseLAM = false, const FName SessionName = "Session");

	UFUNCTION(BlueprintCallable)
	void FindSession(const int MaxSearchResult, const bool UseLAM = false);

	UFUNCTION(BlueprintCallable)
	void JoinSession(const FString& sessionID);

	
	virtual void OnFindSessionComplete(bool IsSuccess);
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:
	/*This will be call after all the session being added to array*/
	UPROPERTY(BlueprintAssignable)
	FOnFoundSession OnFoundSession;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/*Helper function*/
	const FOnlineSessionSearchResult GetSessionResultFromString(const FString Session) const;

private:
	IOnlineSessionPtr SessionInterface;
	//TArray<FSessionInfo> SessionInfos;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	/*Use for host server so later can use this to destroy session*/
	FName CurrentSessionName;

};
