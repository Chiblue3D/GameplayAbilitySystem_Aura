// Copyright Stuart Hancock


#include "Player/AuraPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();

	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * line trace from cursor... Scenarios
	 * A. LastActor = null and ThisActor = null
	 *		- Do nothing
	 * B. LastActor = null and ThisActor is valid
	 *		- Highlight ThisActor
	 * C. LastActor is valid and ThisActor is null
	 *      - Unhighlight LastActor
	 * D. Both are valid, but LastActor != ThisActor
	 *      - Unhiglight LastActor and Highlight ThisActor
	 * E. Both are valid, but LastActor == ThisActor
	 *		- Do nothing
	*/
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			// Case B
			ThisActor->HightlightActor();
		}
		else
		{
			// Case A
		}
	}
	else
	{
		if(ThisActor == nullptr)
		{
			// case C
			LastActor->UnHightlightActor();
		}
		else
		{
			// case D
			if(LastActor != ThisActor)
			{
				LastActor->UnHightlightActor();
				ThisActor->HightlightActor();
			}
			else
			{
				// case E
			}
		}
	}
}
