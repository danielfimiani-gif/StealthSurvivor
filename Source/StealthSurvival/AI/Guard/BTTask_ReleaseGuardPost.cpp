#include "BTTask_ReleaseGuardPost.h"

#include "AIController.h"
#include "StealthAIController.h"

UBTTask_ReleaseGuardPost::UBTTask_ReleaseGuardPost()
{
	NodeName = "Release Guard Post";
}

EBTNodeResult::Type UBTTask_ReleaseGuardPost::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AStealthAIController* Controller = Cast<AStealthAIController>(OwnerComp.GetAIOwner()))
	{
		Controller->ReleaseGuardPost();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
