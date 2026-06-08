#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReleaseGuardPost.generated.h"

UCLASS()
class UBTTask_ReleaseGuardPost : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ReleaseGuardPost();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
