#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "StealthGuardCharacter.generated.h"

class UBehaviorTree;

UCLASS(abstract)
class AStealthGuardCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	
	void Die();
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamId = NewTeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	
	UFUNCTION(BlueprintPure, Category = "AI")
	bool IsDead() const { return bIsDead; }
	
	const TArray<TObjectPtr<AActor>>& GetPatrolPoints() const { return PatrolPoints; }
	
protected:
	AStealthGuardCharacter();
	
	bool bIsDead = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(ClampMin="0"))
	float DeathLifespan = 5.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Patrol")
	TArray<TObjectPtr<AActor>> PatrolPoints;
	
private:
	FGenericTeamId TeamId;
};
