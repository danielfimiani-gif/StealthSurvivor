#include "BTTask_PickRandomPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"

UBTTask_PickRandomPatrolPoint::UBTTask_PickRandomPatrolPoint()
{
	NodeName = TEXT("Pick Random Patrol Point");
}

EBTNodeResult::Type UBTTask_PickRandomPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* TaskMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	
	FNavLocation RandomPoint;
	const bool bFound = NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, RandomPoint);
	if (!bFound)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (BlackBoard == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	BlackBoard->SetValueAsVector(BlackboardKey.SelectedKeyName, RandomPoint.Location);
	return EBTNodeResult::Succeeded;
}