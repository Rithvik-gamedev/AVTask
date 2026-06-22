// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Character/HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CustomAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Core/CustomGameMode.h"
#include "Core/CustomPlayerController.h"
#include "Core/CustomPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/WeaponBase.h"


AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
}


void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (MoveSpeedChangeDelegateHandle.IsValid())
	{
		MoveSpeedChangeDelegateHandle.Reset();
	}
}

void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeroCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem && CharacterInputContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(CharacterInputContext, 0);
		}
	}
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}

void AHeroCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	if (ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>())
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		AttributeSet = PS->GetAttributeSet();
		
		ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetController());
		if (PC)
		{
			PC->InitializeHUD(AbilitySystemComponent, AttributeSet, PS);
		}
		
		if (AbilitySystemComponent)
		{
			FGameplayTag PlayerCharacterTag = FGameplayTag::RequestGameplayTag(FName("Player.Character"));
			if (PlayerCharacterTag.IsValid())
			{
				AbilitySystemComponent->AddLooseGameplayTag(PlayerCharacterTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
			}
			
			// Set MoveSpeed from Attribute
			MoveSpeedChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &AHeroCharacter::OnMoveSpeedChange);
			
			bool bFound = false;
			float InitialSpeed = AbilitySystemComponent->GetGameplayAttributeValue(AttributeSet->GetMoveSpeedAttribute(), bFound);
			if (bFound && GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
			}
			
			if (HasAuthority())
			{
				if (PS->RetainedWeapon)
				{
					CurrentWeapon = PS->RetainedWeapon;
					CurrentWeapon->SetOwner(this);
					CurrentWeapon->SetActorEnableCollision(true);
					OnRep_CurrentWeapon();
				}
				
				// Give Jump Ability
				if (JumpAbility)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(JumpAbility, 1));
				}
				
				if (DeathAbility)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DeathAbility, 1));
				}
				
				if (DashAbility)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DashAbility, 1));
				}
			}
			
			// Stamina action on tag added/removed 
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Player.State.Attacking"), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHeroCharacter::OnStaminaActionTagChanged);
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Player.State.Jumping"), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHeroCharacter::OnStaminaActionTagChanged);
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Player.State.Dashing"), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHeroCharacter::OnStaminaActionTagChanged);
			
		}
		
		// On Death
		if (AttributeSet)
		{
			AttributeSet->OnDeath.AddUObject(this, &AHeroCharacter::HandleDeath);
		}
	}
	
	InitializeDefaultAttributes();
}

void AHeroCharacter::HandleDeath(AController* InstigatorController)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Death")));
	
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(true);
	}
	
	if (HasAuthority())
	{
		if (CurrentWeapon)
		{
			if (ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>())
			{
				PS->RetainedWeapon = CurrentWeapon;
				CurrentWeapon->SetOwner(PS);
			}
		}
		
		AController* PC = GetController();
		DetachFromControllerPendingDestroy();
		
		if (ACustomGameMode* GM = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->PlayerDead(PC);
		}
		SetLifeSpan(3.f);
		
	}
}

void AHeroCharacter::OnMoveSpeedChange(const FOnAttributeChangeData& Data)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	}
}

void AHeroCharacter::InitializeDefaultAttributes()
{
	if (!HasAuthority() || !AbilitySystemComponent || !DefaultAttributes) return;
	
	ApplyEffectToSelf(DefaultAttributes, 1.f);
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHeroCharacter::Move);
		EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AHeroCharacter::Look);
		EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &AHeroCharacter::Attack);
		EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AHeroCharacter::DoJump);
		EnhancedInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &AHeroCharacter::DoDash);
	}
}

void AHeroCharacter::Move(const FInputActionValue& Value)
{
	/*if (AbilitySystemComponent)
	{
		FGameplayTag BlockMovementTag = FGameplayTag::RequestGameplayTag(FName("Player.State.Attacking"));
        
		if (AbilitySystemComponent->HasMatchingGameplayTag(BlockMovementTag))
		{
			return;
		}
	}*/
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		const FRotator Rotation = PC->GetControlRotation();
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
		
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AHeroCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHeroCharacter::Attack(const FInputActionValue& Value)
{
	const bool bVal = Value.Get<bool>();
	if (!bVal) return;
	if (!AbilitySystemComponent) return;
	
	FGameplayTag AttackingTag = FGameplayTag::RequestGameplayTag(FName("Player.State.Attacking"));
	
	if (AbilitySystemComponent->HasMatchingGameplayTag(AttackingTag))
	{
		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.Target = this;
        
		AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Input.Attack"), &Payload);
		
		if (!HasAuthority())
		{
			Server_BufferAttack();
		}
	}
	else
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Attack")));
	}
}

void AHeroCharacter::DoJump(const FInputActionValue& Value)
{
	const bool bVal = Value.Get<bool>();
	if (!bVal) return;
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Jump")));
}

void AHeroCharacter::DoDash(const FInputActionValue& Value)
{
	const bool bVal = Value.Get<bool>();
	if (!bVal) return;
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Dash")));
}

void AHeroCharacter::Server_BufferAttack_Implementation()
{
	if (AbilitySystemComponent)
	{
		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.Target = this;
	        
		AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Input.Attack"), &Payload);
	}
}

void AHeroCharacter::StopStaminaRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void AHeroCharacter::StartStaminaRegenDelay()
{
	if (!AttributeSet) return;
	
	StopStaminaRegen();
	
	float Delay = AttributeSet->GetStaminaRegenDelay();
	if (Delay <= 0.0f) Delay = 1.5f;

	GetWorld()->GetTimerManager().SetTimer(StaminaRegenDelayTimerHandle, this, &AHeroCharacter::StartStaminaRegen, Delay, false);
}

void AHeroCharacter::StartStaminaRegen()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &AHeroCharacter::TickStaminaRegen, 0.1f, true);
}

void AHeroCharacter::TickStaminaRegen()
{
	if (!AttributeSet) return;

	float CurrentStamina = AttributeSet->GetStamina();
	float MaxStamina = AttributeSet->GetMaxStamina();
	float RegenRate = AttributeSet->GetStaminaRegenRate();

	if (CurrentStamina >= MaxStamina)
	{
		StopStaminaRegen();
		return;
	}


	float NewStamina = FMath::Clamp(CurrentStamina + (RegenRate * 0.1f), 0.0f, MaxStamina);
	AttributeSet->SetStamina(NewStamina);
}

void AHeroCharacter::OnStaminaActionTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		StopStaminaRegen();
	}
	else
	{
		// Check for other actions before starting stamina regen 
		if (AbilitySystemComponent && 
			!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player.State.Attacking")) && 
			!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player.State.Jumping")) &&
			!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player.State.Dashing")))
		{
			StartStaminaRegenDelay();
		}
	}
}
