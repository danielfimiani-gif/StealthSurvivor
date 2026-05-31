#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StealthAIController.generated.h"

UCLASS()
class AStealthAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
