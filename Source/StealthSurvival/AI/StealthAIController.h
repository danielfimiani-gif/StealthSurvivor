#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "StealthAIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class AStealthAIController : public AAIController
{
	GENERATED_BODY()
public:
	AStealthAIController();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus); 
};
