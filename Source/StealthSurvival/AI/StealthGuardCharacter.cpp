#include "StealthGuardCharacter.h"
#include "StealthAIController.h" 

AStealthGuardCharacter::AStealthGuardCharacter()
{
	AIControllerClass = AStealthAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}