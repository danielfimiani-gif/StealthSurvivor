#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StealthHUDWidget.generated.h"

class AStealthSurvivalGameState;
class AStealthSurvivalCharacter;

UCLASS(Abstract)
class UStealthHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="HUD")
	float GetDetectionLevel() const;
	
	UFUNCTION(BlueprintCallable, Category="HUD")
	FText GetObjectiveText() const;
	
	UFUNCTION(BlueprintCallable, Category="HUD")
	bool IsPlayerConcealed() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText StealObjectiveText = FText::FromString(TEXT("Obtené la tarjeta"));
	
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText EscapeObjectiveText = FText::FromString(TEXT("Escapá a la zona segura"));
	
private:
	AStealthSurvivalGameState* GetStealthGameState() const;
	AStealthSurvivalCharacter* GetStealthPlayer() const;
};
