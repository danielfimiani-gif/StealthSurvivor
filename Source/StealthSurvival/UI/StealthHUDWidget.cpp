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
	if (GS == nullptr)
	{
		return CardObjectiveText;
	}

	// Paso 1: tarjeta de acceso
	if (!GS->HasKey(AccessCardId))
	{
		return CardObjectiveText;
	}

	// Paso 2: robar los planos
	if (!GS->HasObjective())
	{
		return StealObjectiveText;
	}

	// Paso 3: código de acceso
	if (!GS->HasKey(EscapeCodeId))
	{
		return FindCodeObjectiveText;
	}

	// Paso 4: escapar
	return EscapeObjectiveText;
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
