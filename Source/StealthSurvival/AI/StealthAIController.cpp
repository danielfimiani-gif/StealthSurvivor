#include "StealthAIController.h"
#include "StealthGuardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AStealthAIController::AStealthAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 1800.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation  = 200.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AStealthAIController::OnTargetPerceptionUpdated);
}

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

void AStealthAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr || Blackboard == nullptr)
	{
		return;
	}
	
	static const FName TargetActorKey = TEXT("TargetActor");
	static const FName LastKnownLocationKey = TEXT("LastKnownLocation");
	static const FName HasLineOfSightKey = TEXT("HasLineOfSight");
	
	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
	if (CurrentTarget == Actor)
	{
		Blackboard->ClearValue(TargetActorKey);
		Blackboard->SetValueAsBool(HasLineOfSightKey, false);
	}
	
	const bool bIsSight = (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>());
	
	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsObject(TargetActorKey, Actor);
		Blackboard->SetValueAsVector(LastKnownLocationKey, Stimulus.StimulusLocation);
		
		if (bIsSight)
		{
			Blackboard->SetValueAsBool(HasLineOfSightKey, true);
		}
	}
	else
	{
		if (bIsSight)
		{
			Blackboard->SetValueAsBool(HasLineOfSightKey, false);
		}
		
		if (Stimulus.IsExpired())
		{
			Blackboard->ClearValue((TargetActorKey));
		}
	}
}
