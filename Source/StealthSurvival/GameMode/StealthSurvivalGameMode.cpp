#include "StealthSurvivalGameMode.h"
#include "StealthSurvivalGameState.h"

AStealthSurvivalGameMode::AStealthSurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameStateClass = AStealthSurvivalGameState::StaticClass();
}

AStealthSurvivalGameState* AStealthSurvivalGameMode::GetStealthSurvivalGameState() const
{
	return GetGameState<AStealthSurvivalGameState>();
}

void AStealthSurvivalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr)
	{
		return;
	}
	
	if (GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	const float Delta = (ActiveWatchers > 0)
		? DetectionRiseRate * DeltaSeconds
		: -DetectionFallRate * DeltaSeconds;
	
	const float NewLevel = GS->GetDetectionLevel() + Delta;
	GS->SetDetectionLevel(NewLevel);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, 
			FColor::Yellow, 
			FString::Printf(TEXT("Detection: %.2f | Watchers: %d"),
			GS->GetDetectionLevel(),ActiveWatchers));
	}
	if (NewLevel >= 1.f)
	{
		TriggerLose();
	}
}

void AStealthSurvivalGameMode::AddWatcher()
{
	++ActiveWatchers;
}

void AStealthSurvivalGameMode::RemoveWatcher()
{
	ActiveWatchers = FMath::Max(0, ActiveWatchers - 1);
}

void AStealthSurvivalGameMode::NotifiObjectiveStolen()
{
	if (AStealthSurvivalGameState* GS = GetStealthSurvivalGameState())
	{
		GS->SetHasObjective(true);
	}
}

void AStealthSurvivalGameMode::NotifyPlayerReachedExit()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr)
	{
		return;
	}
	
	if (GS->HasObjective())
	{
		TriggerWin();
	}
}

void AStealthSurvivalGameMode::TriggerWin()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr || GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	GS->SetMatchState(EStealthMatchState::Won);
}

void AStealthSurvivalGameMode::TriggerLose()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr || GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	GS->SetMatchState(EStealthMatchState::Lost);
	GS->SetDetectionLevel(1.f);
}
