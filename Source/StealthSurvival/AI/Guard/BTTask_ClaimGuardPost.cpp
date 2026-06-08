#include "BTTask_ClaimGuardPost.h"

#include "AIController.h"
#include "SmartObjectRequestTypes.h"
#include "StealthAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SmartObjectSubsystem.h"
#include "GameFramework/Pawn.h"

UBTTask_ClaimGuardPost::UBTTask_ClaimGuardPost()
{
	NodeName = "Claim Guard Post";
}

EBTNodeResult::Type UBTTask_ClaimGuardPost::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AStealthAIController* Controller = Cast<AStealthAIController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Pawn = Controller->GetPawn();
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	USmartObjectSubsystem* SOSubsystem = USmartObjectSubsystem::GetCurrent(Controller->GetWorld());
	if (SOSubsystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	Controller->ReleaseGuardPost();
	
	const FVector Origin = Pawn->GetActorLocation();
	const FBox QueryBox = FBox::BuildAABB(Origin, FVector(SearchRadius));
	
	FSmartObjectRequestFilter Filter;
	FGameplayTagContainer RequiredTag;
	RequiredTag.AddTag(GuardPostTag);
	Filter.ActivityRequirements = FGameplayTagQuery::MakeQuery_MatchAllTags(RequiredTag);
	
	const FSmartObjectRequest Request(QueryBox, Filter);
	
	const FSmartObjectRequestResult Result = SOSubsystem->FindSmartObject(Request, Pawn);
	if (!Result.IsValid())
	{
		return EBTNodeResult::Failed;
	}
	
	const FSmartObjectClaimHandle ClaimHandle = SOSubsystem->MarkSlotAsClaimed(
		Result.SlotHandle,
		ESmartObjectClaimPriority::Normal
	);
	if (!ClaimHandle.IsValid())
	{
		return EBTNodeResult::Failed;
	}
	
	FTransform SlotTransform;
	if (!SOSubsystem->GetSlotTransform(ClaimHandle, SlotTransform))
	{
		SOSubsystem->Release(ClaimHandle);
		return EBTNodeResult::Failed;
	}
	
	Controller->SetClaimedGuardPost(ClaimHandle);
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsVector(GuardPostLocationKey.SelectedKeyName, SlotTransform.GetLocation());
	}
	
	return EBTNodeResult::Succeeded;
}
