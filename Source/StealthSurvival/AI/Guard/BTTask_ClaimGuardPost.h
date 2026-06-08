#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ClaimGuardPost.generated.h"

UCLASS()
class UBTTask_ClaimGuardPost: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ClaimGuardPost();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="SmartObject")
	FGameplayTag GuardPostTag;
	
	UPROPERTY(EditAnywhere, Category="SmartObject", meta=(ClampMin="0"))
	float SearchRadius = 2000.f;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector GuardPostLocationKey;
};
