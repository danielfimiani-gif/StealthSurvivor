// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthSurvivalCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "StealthSurvival.h"
#include "Perception/AISense_Hearing.h"
#include  "StealthGuardCharacter.h"

AStealthSurvivalCharacter::AStealthSurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	MovementComp->JumpZVelocity = 500.f;
	MovementComp->AirControl = 0.35f;
	MovementComp->MaxWalkSpeed = WalkSpeed;
	MovementComp->MinAnalogWalkSpeed = 20.f;
	MovementComp->BrakingDecelerationWalking = 2000.f;
	MovementComp->BrakingDecelerationFalling = 1500.0f;
	MovementComp->NavAgentProps.bCanCrouch = true;
	MovementComp->MaxWalkSpeedCrouched = CrouchSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AStealthSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthSurvivalCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AStealthSurvivalCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealthSurvivalCharacter::Look);
		
		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AStealthSurvivalCharacter::StopSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AStealthSurvivalCharacter::StopSprint);
		
		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::ToggleCrouch);
		
		// Takedown
		EnhancedInputComponent->BindAction(TakeDownAction, ETriggerEvent::Started,this, &AStealthSurvivalCharacter::ExecuteTakeDown);
	}
	else
	{
		UE_LOG(LogStealthSurvival, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AStealthSurvivalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AStealthSurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AStealthSurvivalCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AStealthSurvivalCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AStealthSurvivalCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AStealthSurvivalCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AStealthSurvivalCharacter::StartSprint()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	
	bIsSprinting = true;
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::StopSprint()
{
	bIsSprinting = false;
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		bIsSprinting = false;
		Crouch();
	}
	
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp == nullptr) return;
	
	if (bIsSprinting)
		MovementComp->MaxWalkSpeed = RunSpeed;
	else
		MovementComp->MaxWalkSpeed = WalkSpeed;
	
	MovementComp->MaxWalkSpeedCrouched = CrouchSpeed;
}

float AStealthSurvivalCharacter::GetCurrentNoiseRange() const
{
	if (bIsCrouched) return CrouchNoiseRange;
	if (bIsSprinting) return RunNoiseRange;
	return WalkNoiseRange;
}

void AStealthSurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	NoiseEmissionTimer += DeltaSeconds;
	if (NoiseEmissionTimer < NoiseEmissionInterval)
	{
		return;
	}
	NoiseEmissionTimer = 0.f;
	
	const float CurrentSpeed = GetVelocity().Size2D();
	if (CurrentSpeed < 5.f)
	{
		return;
	}
	
	const float NoiseRange = GetCurrentNoiseRange();
	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		GetActorLocation(),
		1.0f,
		this,
		NoiseRange
	);
}

void AStealthSurvivalCharacter::ExecuteTakeDown()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + GetActorForwardVector() * TakeDownTraceDistance;
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	const bool bHit = World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, Params);
	if (!bHit)
	{
		return;
	}
	
	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(Hit.GetActor());
	if (Guard == nullptr)
	{
		return;
	}
	
	FVector GuardToPlayer = GetActorLocation() - Guard->GetActorLocation();
	GuardToPlayer.Z = 0.f;
	GuardToPlayer.Normalize();
	
	FVector GuardForward = Guard->GetActorForwardVector();
	GuardForward.Z = 0.f;
	GuardForward.Normalize();
	
	const float DotProduct = FVector::DotProduct(GuardForward, GuardToPlayer);
	
	if (DotProduct > TakeDownRearDotThreshold)
	{
		return;
	}
	
	Guard->Die();
}
