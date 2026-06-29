#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionTypes.h"
#include "StealthSecurityCamera.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USpotLightComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UStateTreeComponent;

UCLASS(abstract)
class AStealthSecurityCamera : public AActor
{
	GENERATED_BODY()
	
public:
	AStealthSecurityCamera();
	
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	USceneComponent* GetCameraPivot() const {return CameraPivot;}
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	AActor* GetTargetActor() const {return TargetActor;}
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	FVector GetLastSeenLocation() const {return LastSeenLocation;}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void SetWatchingPlayer(bool bNewWatching);
	bool bIsWatchingPlayer = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* CameraPivot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* HeadMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USpotLightComponent* DetectionCone;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UAIPerceptionComponent* PerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UStateTreeComponent* StateTreeComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Camera|State")
	AActor* TargetActor = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Camera|State")
	FVector LastSeenLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Perception", meta=(ClampMin="0"))
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Perception", meta=(ClampMin="0"))
	float LoseSightRadius = 1800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Perception", meta=(ClampMin="0", ClampMax="180"))
	float PeripheralVisionAngle = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Perception", meta=(ClampMin="0"))
	float SightMaxAge = 3.f;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};