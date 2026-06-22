// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterBase.h"
#include "HeroCharacter.generated.h"

struct FGameplayTag;
struct FGameplayTagContainer;
struct FOnAttributeChangeData;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class AVTASK_API AHeroCharacter : public ACustomCharacterBase
{
	GENERATED_BODY()

public:

	AHeroCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PawnClientRestart() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void InitAbilityActorInfo() override;
	
	virtual void HandleDeath(AController* InstigatorController);
	
	void OnMoveSpeedChange(const FOnAttributeChangeData& Data);
	FDelegateHandle MoveSpeedChangeDelegateHandle;
	
	void InitializeDefaultAttributes();
	
	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> CharacterInputContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dash;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void DoJump(const FInputActionValue& Value);
	void DoDash(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_BufferAttack();

	// Stamina Regeneration
	void StopStaminaRegen();
	void StartStaminaRegenDelay();
	void StartStaminaRegen();
	void TickStaminaRegen();

	virtual void OnStaminaActionTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	FTimerHandle StaminaRegenDelayTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;

	// Components
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Abilities")
	TSubclassOf<UGameplayAbility> DeathAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Abilities")
	TSubclassOf<UGameplayAbility> DashAbility;
	
};
