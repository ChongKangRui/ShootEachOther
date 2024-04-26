// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GM_MainMenu.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "Online.h"


AGM_MainMenu::AGM_MainMenu()
{
   
}

//void AGM_MainMenu::CreateSession(const int MaxPlayer, TEnumAsByte<ECreateSessionResults>& Result, const bool UseLAM, const FName SessionName)
//{
//    if (SessionInterface.IsValid())
//    {
//
//        FOnlineSessionSettings SessionSettings;
//
//        SessionSettings.bIsLANMatch = UseLAM; // Set to true if LAN match
//        SessionSettings.NumPublicConnections = MaxPlayer; // Adjust as needed
//        SessionSettings.bUsesPresence = true;
//        SessionSettings.bIsDedicated = false;
//        SessionSettings.bAllowJoinInProgress = true;
//        SessionSettings.bShouldAdvertise = true;
//        SessionSettings.Settings.Add(TEXT("SessionDisplayName"), FOnlineSessionSetting(SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService));
//        UE_LOG(LogTemp, Warning, TEXT("Hey Adding it from create %d"), SessionSettings.Settings.Num());
//        UE_LOG(LogTemp, Warning, TEXT("Invalid Session Interface %s"), *SessionSettings.SessionIdOverride);
//   
//        SessionInterface->CreateSession(0, SessionName, SessionSettings);
//
//        
//        return;
//    }
//    UE_LOG(LogTemp, Warning, TEXT("Invalid Session Interface"));
//    Result = ECreateSessionResults::Fail;
//}
//void AGM_MainMenu::FindSession(const int MaxSearchResultInt, const bool UseLAM)
//{
//   
//    if (SessionInterface.IsValid())
//    {
//        SessionIDs.Empty();
//        SessionSearch = MakeShareable(new FOnlineSessionSearch());
//        SessionSearch->bIsLanQuery = UseLAM; // Set to true if LAN query
//        SessionSearch->MaxSearchResults = MaxSearchResultInt;
//        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//
//       
//        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
//        
//        UE_LOG(LogTemp, Warning, TEXT("start finding session"));
//        return;
//    }
//    UE_LOG(LogTemp, Warning, TEXT("Invalid Session Interface"));
//}
//
//void AGM_MainMenu::JoinSession(const FString& sessionID)
//{
//    if (SessionInterface.IsValid())
//    {
//        SessionInterface->JoinSession(0, FName(sessionID), GetSessionResultFromString(sessionID));
//    }
//}
//
//TArray<FString> AGM_MainMenu::GetFoundedSession() const
//{
//    return SessionIDs;
//}
//
//const FOnlineSessionSearchResult AGM_MainMenu::GetSessionResultFromString(const FString Session) const
//{
//    for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
//    {
//        if (Session == SearchResult.GetSessionIdStr()) {
//            return SearchResult;
//        }
//    }
//    UE_LOG(LogTemp, Warning, TEXT("No Search String Founded!!"));
//    return FOnlineSessionSearchResult();
//}
//
//void AGM_MainMenu::OnFindSessionComplete(bool IsSuccess)
//{
//    TArray<FOnlineSessionSearchResult> result = SessionSearch->SearchResults;
//    
//        for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
//        {
//           
//            FString SessionIdStr = SearchResult.GetSessionIdStr();
//            SessionIDs.Add(SessionIdStr);
//            FOnlineSessionSetting* s = SearchResult.Session.SessionSettings.Settings.Find("SessionDisplayName");
//            UE_LOG(LogTemp, Warning, TEXT("Setting Map: %d"), SearchResult.Session.SessionSettings.Settings.Num());
//            UE_LOG(LogTemp, Warning, TEXT("Max Player: %d"), SearchResult.Session.SessionSettings.NumPublicConnections);
//            UE_LOG(LogTemp, Warning, TEXT("Allow Join In Progress: %s"), SearchResult.Session.SessionSettings.bAllowJoinInProgress ? TEXT("True") : TEXT("False"));
//            if (!s) {
//                UE_LOG(LogTemp, Warning, TEXT("Invalid datatype"));
//               
//            }
//            else {
//                FString a;
//                s->Data.GetValue(a);
//            }
//           
//           
//        }
//
//    UE_LOG(LogTemp, Warning, TEXT("session founded == %d"), result.Num());
//    UE_LOG(LogTemp, Warning, TEXT("Success == %s"), IsSuccess ? TEXT("true") : TEXT("false"));
//}
//
//void AGM_MainMenu::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
//    if (UWorld* world = GetWorld()) {
//        if (bWasSuccessful) {
//            UE_LOG(LogTemp, Warning, TEXT("Create Session Success"));
//            //SessionInterface->StartSession(SessionName);
//            world->ServerTravel("/Game/Map/FirstPersonMap?listen");
//        }
//        else {
//            UE_LOG(LogTemp, Warning, TEXT("Create Session unSuccess"));
//        }
//    }
//}
//
//void AGM_MainMenu::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//    UE_LOG(LogTemp, Warning, TEXT("joined Session Name: %s"), *SessionName.ToString());
//    FString s = TEXT(">>>>?????");
//    switch (Result) {
//    case EOnJoinSessionCompleteResult::Success:
//        s = TEXT("Success");
//        break;
//    case EOnJoinSessionCompleteResult::UnknownError:
//         s = TEXT("Unknown Error");
//        break;
//    }
//    UE_LOG(LogTemp, Warning, TEXT("join session result %s"), *s);
//    
//
//}


