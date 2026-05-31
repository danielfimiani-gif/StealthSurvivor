#include "StealthAIController.h"
#include "StealthGuardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AStealthAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(InPawn);
	if (Guard == nullptr) return;
	
	UBehaviorTree* BT = Guard->GetBehaviorTree();
	if (BT == nullptr) return;
	
	if (BT->GetBlackboardAsset())
	{
		UBlackboardComponent* BBComp = nullptr;
		UseBlackboard(BT->GetBlackboardAsset(), BBComp);
	}
	
	RunBehaviorTree(BT);
}