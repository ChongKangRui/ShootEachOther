// Fill out your copyright notice in the Description page of Project Settings.


#include "SEOMultiplayerSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "Online.h"

void USEOMultiplayerSubsystem::CreateSession(const int MaxPlayer, const bool UseLAM, const FName SessionName)
{
    if (SessionInterface.IsValid())
    {

        FOnlineSessionSettings SessionSettings;

        SessionSettings.bIsLANMatch = UseLAM; // Set to true if LAN match
        SessionSettings.NumPublicConnections = MaxPlayer; // Adjust as needed
        SessionSettings.bUsesPresence = true;
        SessionSettings.bIsDedicated = false;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.Settings.Add(TEXT("SessionDisplayName"), FOnlineSessionSetting(SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService));
        
        SessionInterface->CreateSession(0, SessionName, SessionSettings);


        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Invalid Session Interface"));
}
void USEOMultiplayerSubsystem::FindSession(const int MaxSearchResultInt, const bool UseLAM)
{

    if (SessionInterface.IsValid())
    {
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        SessionSearch->bIsLanQuery = UseLAM; // Set to true if LAN query
        SessionSearch->MaxSearchResults = MaxSearchResultInt;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

        UE_LOG(LogTemp, Warning, TEXT("start finding session"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Invalid Session Interface"));
}

void USEOMultiplayerSubsystem::JoinSession(const FString& SessionName)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->JoinSession(0, FName(SessionName), GetSessionResultFromString(SessionName));
        UE_LOG(LogTemp, Warning, TEXT("joining valid Session Interface"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("joining Invalid Session Interface"));
    }
}



const FOnlineSessionSearchResult USEOMultiplayerSubsystem::GetSessionResultFromString(const FString Session) const
{
    for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
    {
        FOnlineSessionSetting* customDataSlot = SearchResult.Session.SessionSettings.Settings.Find("SessionDisplayName");
        if (customDataSlot) {
            FString name;
            customDataSlot->Data.GetValue(name);
            
            if (Session == name) {
                return SearchResult;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("No Search String Founded!!"));
    return FOnlineSessionSearchResult();
}

void USEOMultiplayerSubsystem::OnFindSessionComplete(bool IsSuccess)
{
    TArray<FOnlineSessionSearchResult> result = SessionSearch->SearchResults;
    TArray<FSessionInfo> OutSessionResults;
    for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
    {

        FString SessionIdStr = SearchResult.GetSessionIdStr();
        FOnlineSessionSetting* customDataSlot = SearchResult.Session.SessionSettings.Settings.Find("SessionDisplayName");

        

        UE_LOG(LogTemp, Warning, TEXT("Setting Map: %d"), SearchResult.Session.SessionSettings.Settings.Num());
        UE_LOG(LogTemp, Warning, TEXT("Max Player: %d"), SearchResult.Session.SessionSettings.NumPublicConnections);
        UE_LOG(LogTemp, Warning, TEXT("Allow Join In Progress: %s"), SearchResult.Session.SessionSettings.bAllowJoinInProgress ? TEXT("True") : TEXT("False"));
        if (customDataSlot) {
            FString sessionName;
            customDataSlot->Data.GetValue(sessionName);
            int32 CurrentNumPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
            OutSessionResults.Add(FSessionInfo(sessionName, CurrentNumPlayers, SearchResult.Session.SessionSettings.NumPublicConnections, SearchResult.PingInMs));
        }


    }

    OnFoundSession.Broadcast(OutSessionResults);

    UE_LOG(LogTemp, Warning, TEXT("session founded == %d"), OutSessionResults.Num());
    UE_LOG(LogTemp, Warning, TEXT("Success == %s"), IsSuccess ? TEXT("true") : TEXT("false"));
}

void USEOMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
    if (UWorld* world = GetWorld()) {
        if (bWasSuccessful) {
            UE_LOG(LogTemp, Warning, TEXT("Create Session Success, SessionName = %s"), *SessionName.ToString());
            //SessionInterface->StartSession(SessionName);
            SessionSearch.Reset();
            CurrentSessionName = SessionName;
            
            world->ServerTravel("/Game/Map/FirstPersonMap?listen");
            
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Create Session unSuccess"));
        }
    }
}

void USEOMultiplayerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(LogTemp, Warning, TEXT("joined Session Name: %s"), *SessionName.ToString());
    FString JoinAddress;
    SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);

    if (Result == EOnJoinSessionCompleteResult::Success) {
        if (JoinAddress != "") {
            if (APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
                pc->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
            }

        }
    }
    else {
        FString error;
        switch (Result)
        {
        case EOnJoinSessionCompleteResult::AlreadyInSession:
            error = TEXT("Already In Session");
            break;
        case EOnJoinSessionCompleteResult::SessionIsFull:
            error = TEXT("Session Is Full");
            break;
            // Add cases for other enum values as needed
        case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
            error = TEXT("Could Not Retrieve Address");
            break;
        case EOnJoinSessionCompleteResult::SessionDoesNotExist:
            error = TEXT("Session doesnt exist");
            break;
        default:
            error = TEXT("Unknown");
        }
        
        UE_LOG(LogTemp, Warning, TEXT("joined Session Name: %s"), *error);
       
    }


}

void USEOMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Get the OnlineSubsystem we want to work with

      // Get the session interface from the OnlineSubsystem
    SessionInterface = Online::GetSessionInterface();
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USEOMultiplayerSubsystem::OnFindSessionComplete);
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USEOMultiplayerSubsystem::OnCreateSessionComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USEOMultiplayerSubsystem::OnJoinSessionComplete);

    UE_LOG(LogTemp, Warning, TEXT("Multiplayer Subsystem initialization"));
}

void USEOMultiplayerSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UE_LOG(LogTemp, Warning, TEXT("Multiplayer Subsystem deinitialization"));
   
}
