// Copyright mrgyarmati


#include "Characters/AGXCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "ALSCamera/Public/AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AGXAttributeSet.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Player/AGXPlayerController.h"
#include "Player/AGXPlayerState.h"
#include "UI/HUD/AGXHUD.h"


AAGXCharacterPlayer::AAGXCharacterPlayer()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});
}

void AAGXCharacterPlayer::NotifyControllerChanged()
{
	const auto* PreviousPlayer{Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			InputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	auto* NewPlayer{Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		// Use the Enhanced Input plugin Scalar Modifier instead.
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;

		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			FModifyContextOptions Options;
			Options.bNotifyUserSettings = true;

			InputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
		}
	}

	Super::NotifyControllerChanged();
}

void AAGXCharacterPlayer::InitAbilityActorInfo()
{
	if (AAGXPlayerState* AGXPlayerState = GetPlayerState<AAGXPlayerState>())
	{
		AGXPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AGXPlayerState, this);
		AbilitySystemComponent = AGXPlayerState->GetAbilitySystemComponent();
		AttributeSet = AGXPlayerState->GetAttributeSet();

		if (AAGXPlayerController* AGXPlayerController = Cast<AAGXPlayerController>(GetController()))
		{
			if (AAGXHUD* AGXHUD = Cast<AAGXHUD>(AGXPlayerController->GetHUD()))
				AGXHUD->InitOverlay(AGXPlayerController, AGXPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAGXCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAGXCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAGXCharacterPlayer::CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AAGXCharacterPlayer::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnAim);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchShoulder);
	}
}

void AAGXCharacterPlayer::Input_OnLookMouse(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void AAGXCharacterPlayer::Input_OnLook(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate);
	AddControllerYawInput(Value.X * LookRightRate);
}

void AAGXCharacterPlayer::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsMath::ClampMagnitude012D(ActionValue.Get<FVector2D>())};

	const auto ForwardDirection{UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw))};
	const auto RightDirection{UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection)};

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}

void AAGXCharacterPlayer::Input_OnSprint(const FInputActionValue& ActionValue)
{
	//SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
	const bool bWantsToSprint = ActionValue.Get<bool>();
	// Block sprinting if not enough stamina
	if (bWantsToSprint && AttributeSet && Cast<UAGXAttributeSet>(AttributeSet)->GetStamina() <= 10.0f)
	{
		// Optional: play a denied sound or HUD feedback
		return;
	}
	SetDesiredGait(bWantsToSprint ? AlsGaitTags::Sprinting : AlsGaitTags::Running);

	if (AbilitySystemComponent)
	{
		if (bWantsToSprint)
		{
			// Apply Stamina Drain
			if (StaminaDrainEffect && !ActiveStaminaDrainHandle.IsValid())
			{
				FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(StaminaDrainEffect, 1.0f, Context);
				if (Spec.IsValid())
				{
					ActiveStaminaDrainHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				}
			}

			// Remove Regen if sprinting
			if (ActiveStaminaRegenHandle.IsValid())
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveStaminaRegenHandle);
				ActiveStaminaRegenHandle.Invalidate();
			}
		}
		else
		{
			// Remove Stamina Drain
			if (ActiveStaminaDrainHandle.IsValid())
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveStaminaDrainHandle);
				ActiveStaminaDrainHandle.Invalidate();
			}

			// Delay regen (optional)
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				if (StaminaRegenEffect && !ActiveStaminaRegenHandle.IsValid())
				{
					FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
					FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(StaminaRegenEffect, 1.0f, Context);
					if (Spec.IsValid())
					{
						ActiveStaminaRegenHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
					}
				}
			});
		}
	}
}

void AAGXCharacterPlayer::Input_OnWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		SetDesiredGait(AlsGaitTags::Running);
	}
	else if (GetDesiredGait() == AlsGaitTags::Running)
	{
		SetDesiredGait(AlsGaitTags::Walking);
	}
}

void AAGXCharacterPlayer::Input_OnCrouch()
{
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		SetDesiredStance(AlsStanceTags::Crouching);
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
	}
}

void AAGXCharacterPlayer::Input_OnJump(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (StopRagdolling())
		{
			return;
		}

		if (StartMantlingGrounded())
		{
			return;
		}

		if (GetStance() == AlsStanceTags::Crouching)
		{
			SetDesiredStance(AlsStanceTags::Standing);
			return;
		}

		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AAGXCharacterPlayer::Input_OnAim(const FInputActionValue& ActionValue)
{
	SetDesiredAiming(ActionValue.Get<bool>());
}

void AAGXCharacterPlayer::Input_OnRagdoll()
{
	if (!StopRagdolling())
	{
		StartRagdolling();
	}
}

void AAGXCharacterPlayer::Input_OnRoll()
{
	static constexpr auto PlayRate{1.3f};

	StartRolling(PlayRate);
}

void AAGXCharacterPlayer::Input_OnRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
							   ? AlsRotationModeTags::ViewDirection
							   : AlsRotationModeTags::VelocityDirection);
}

void AAGXCharacterPlayer::Input_OnViewMode()
{
	SetViewMode(GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
}

void AAGXCharacterPlayer::Input_OnSwitchShoulder()
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AAGXCharacterPlayer::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused,
	float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}
