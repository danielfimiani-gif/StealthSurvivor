#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PickRandomPatrolPoint.generated.h"

UCLASS()
class UBTTask_PickRandomPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_PickRandomPatrolPoint();
	
protected:
	UPROPERTY(EditAnywhere, Category="Patrol", meta = (ClampMin="0"))
	float SearchRadius = 1500.f;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
