// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "StealthSurvivalCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAIPerceptionStimuliSourceComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AStealthSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
protected:

	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* TakeDownAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float WalkSpeed = 350.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float RunSpeed = 650.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float CrouchSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Noise", meta=(ClampMin="0"))
	float WalkNoiseRange = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Noise", meta=(ClampMin="0"))
	float RunNoiseRange = 1500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Noise", meta=(ClampMin="0"))
	float CrouchNoiseRange = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Combat", meta=(ClampMin="0"))
	float TakeDownTraceDistance = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Combat", meta=(ClampMin="0"))
	float TakeDownRearDotThreshold = -0.3f;
public:

	/** Constructor */
	AStealthSurvivalCharacter();	
	
	virtual void Tick(float DeltaSeconds) override;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void ExecuteTakeDown();
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void StartSprint();
	void StopSprint();
	void ToggleCrouch();
public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:
	UFUNCTION(BlueprintPure, Category="Stealth")
	bool IsSprinting() const { return bIsSprinting; }
	
	UFUNCTION(BlueprintPure, Category="Stealth|Noise")
	float GetCurrentNoiseRange() const;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
private:
	void UpdateMovementSpeed();
	bool bIsSprinting = false;
	
	float NoiseEmissionTimer = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth|Noise", meta=(ClampMin="0.5"))
	float NoiseEmissionInterval = 0.2f;
};

