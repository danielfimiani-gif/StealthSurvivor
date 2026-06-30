#include "StealthHUDWidget.h"

#include "GameMode/StealthSurvivalGameState.h"
#include "Player/StealthSurvivalCharacter.h"

float UStealthHUDWidget::GetDetectionLevel() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	return GS != nullptr ? GS->GetDetectionLevel() : 0.0f;
}

FText UStealthHUDWidget::GetObjectiveText() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	if (GS != nullptr && GS->HasObjective())
	{
		return EscapeObjectiveText;
	}
	return StealObjectiveText;
}

bool UStealthHUDWidget::IsPlayerConcealed() const
{
	AStealthSurvivalCharacter* Player = GetStealthPlayer();
	if (Player == nullptr)
	{
		return false;
	}
	return Player->IsInCover() || Player->IsHidden();
}

AStealthSurvivalGameState* UStealthHUDWidget::GetStealthGameState() const
{
	UWorld* World = GetWorld();
	return World != nullptr ? World->GetGameState<AStealthSurvivalGameState>() : nullptr;
}

AStealthSurvivalCharacter* UStealthHUDWidget::GetStealthPlayer() const
{
	return Cast<AStealthSurvivalCharacter>(GetOwningPlayerPawn());
}
