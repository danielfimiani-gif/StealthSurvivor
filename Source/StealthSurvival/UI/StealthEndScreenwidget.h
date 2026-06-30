#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StealthEndScreenWidget.generated.h"

class AStealthSurvivalGameState;

UCLASS(abstract)
class UStealthEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Para que el WBP muestre "Ganaste" / "Perdiste"
	UFUNCTION(BlueprintPure, Category="EndScreen")
	bool IsVictory() const;

	// Botones del WBP
	UFUNCTION(BlueprintCallable, Category="EndScreen")
	void RestartLevel();

	UFUNCTION(BlueprintCallable, Category="EndScreen")
	void OpenMainMenu();

protected:
	UPROPERTY(EditDefaultsOnly, Category="EndScreen")
	FName MainMenuLevelName = TEXT("Lvl_MainMenu");

private:
	AStealthSurvivalGameState* GetStealthGameState() const;
};