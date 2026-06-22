// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Character/CustomAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Core/CustomGameMode.h"
#include "Enemy/EnemyDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/EnemyHealthWidget.h"
#include "Weapon/WeaponBase.h"

class UEnemyHealthWidget;

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("Ability System");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>("Attribute Set");
	
	EnemyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	EnemyWidgetComponent->SetupAttachment(RootComponent);
	EnemyWidgetComponent->SetDrawSize(FVector2D(100.f, 30.f));
	EnemyWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	EnemyWidgetComponent->SetVisibility(false);
	
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

AEnemyPatrolRoute* AEnemyCharacter::GetPatrolRoute_Implementation()
{
	return PatrolRoute;
}

float AEnemyCharacter::GetCharacterMovementSpeed_Implementation()
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->MaxWalkSpeed;
	}
	return 0.f;
}

void AEnemyCharacter::SetEnemyMoveSpeed_Implementation(float Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Value;
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitialSpawnTransform = GetActorTransform();
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (EnemyWidgetComponent)
	{
		EnemyWidgetComponent->SetWidget(nullptr);
	}
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		FGameplayTag EnemyCharacterTag = FGameplayTag::RequestGameplayTag(FName("Enemy.Character"));
		if (EnemyCharacterTag.IsValid())
		{
			AbilitySystemComponent->AddLooseGameplayTag(EnemyCharacterTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
		}
		
		// Give Jump Ability
		if (JumpAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(JumpAbility, 1));
		}
		
		if (EnemyConfig)
		{
			if (EnemyConfig->DefaultAttributesGE)
			{
				InitializeDefaultAttributes(EnemyConfig->DefaultAttributesGE);
			}
			
			for (auto Ability : EnemyConfig->GrantedAbilities)
			{
				if (!HasAbilityClass(Ability))
				{
					FGameplayAbilitySpec NewSpec(Ability, 1, INDEX_NONE, this);
					FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(NewSpec);
					AddToGrantedAbilities(AbilitySpecHandle);
				}
			}
			
			if (EnemyConfig->WeaponToEquip)
			{
				EquipWeapon(EnemyConfig->WeaponToEquip);
			}
		}
	}
	
	if (AttributeSet)
	{
		AttributeSet->OnDeath.AddUObject(this, &AEnemyCharacter::HandleDeath);
	}
	
	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		if (EnemyWidgetComponent && EnemyConfig->OverHeadHealthWidget)
		{
			UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), EnemyConfig->OverHeadHealthWidget);
			EnemyWidgetComponent->SetWidget(Widget);
			
			if (UEnemyHealthWidget* HealthWidget = Cast<UEnemyHealthWidget>(Widget))
			{
				HealthWidget->InitializeWithASC(AbilitySystemComponent, AttributeSet);
			}
		}
		
	}
	
	FGameplayTag ChasingTag = FGameplayTag::RequestGameplayTag(FName("Enemy.State.Chasing"));
    
	if (ChasingTag.IsValid())
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(ChasingTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::OnChasingTagChanged);
        
		if (AbilitySystemComponent->HasMatchingGameplayTag(ChasingTag))
		{
			EnemyWidgetComponent->SetVisibility(false);
		}
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &AEnemyCharacter::OnMoveSpeedChange);
	
	
}

void AEnemyCharacter::OnMoveSpeedChange(const FOnAttributeChangeData& Data)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	}
}

void AEnemyCharacter::HandleDeath(AController* InstigatorController)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
    
	GetCharacterMovement()->DisableMovement();
    
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Enemy.Death")));
	
	if (CurrentWeapon)
	{
		CurrentWeapon->SetLifeSpan(5.f);
	}
    
	if (HasAuthority())
	{
		if (ACustomGameMode* GM = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->RequestEnemyRespawn(GetClass(), InitialSpawnTransform, 5.0f, PatrolRoute); 
		}
       
		SetLifeSpan(5.f);
	}
}

void AEnemyCharacter::Attack()
{
	FGameplayTag AttackAbilityTag = FGameplayTag::RequestGameplayTag(FName("Enemy.Attack"));
	
	if (AttackAbilityTag.IsValid())
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackAbilityTag));
	}
}

void AEnemyCharacter::InitializeDefaultAttributes(TSubclassOf<UGameplayEffect> DefaultEffect)
{
	if (!HasAuthority() || !AbilitySystemComponent || !DefaultAttributes) return;
	
	ApplyEffectToSelf(DefaultEffect, 1.f);
}

void AEnemyCharacter::OnChasingTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (EnemyWidgetComponent)
	{
	
		const bool bIsPatrolling = NewCount > 0;
		EnemyWidgetComponent->SetVisibility(bIsPatrolling, true);
	}
}

void AEnemyCharacter::PostNetInit()
{
	Super::PostNetInit();
	
	if (!HasAuthority())
	{
		InitAbilityActorInfo();
	}
}
