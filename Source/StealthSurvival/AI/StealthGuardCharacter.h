#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StealthGuardCharacter.generated.h"

class UBehaviorTree;

UCLASS(abstract)
class AStealthGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	
	void Die();
	
	UFUNCTION(BlueprintPure, Category = "AI")
	bool IsDead() const { return bIsDead; }
protected:
	AStealthGuardCharacter();
	
	bool bIsDead = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(ClampMin="0"))
	float DeathLifespan = 5.f;
};
