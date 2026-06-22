// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyInterface.h"
#include "Character/CustomCharacterBase.h"
#include "EnemyCharacter.generated.h"

struct FGameplayTag;
class UWidgetComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFinishedSignature);

class UEnemyDataAsset;
struct FOnAttributeChangeData;
class AEnemyPatrolRoute;
class UBehaviorTree;

UCLASS(HideCategories = ("Default Gameplay Effects"))
class AVTASK_API AEnemyCharacter : public ACustomCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	
	AEnemyCharacter();
	
	virtual AEnemyPatrolRoute* GetPatrolRoute_Implementation() override;
	virtual float GetCharacterMovementSpeed_Implementation() override;
	virtual void SetEnemyMoveSpeed_Implementation(float Value) override; 
	
	UPROPERTY(BlueprintAssignable)
	FOnAttackFinishedSignature OnAttackFinishedDelegate;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	void InitializeDefaultAttributes(TSubclassOf<UGameplayEffect> DefaultEffect);
	virtual void OnChasingTagChanged(const FGameplayTag Tag, int32 NewCount);
	virtual void PostNetInit() override;
	
	virtual void InitAbilityActorInfo() override;
	
	void OnMoveSpeedChange(const FOnAttributeChangeData& Data);
	
	UFUNCTION()
	void HandleDeath(AController* InstigatorController);
	
	UFUNCTION(BlueprintCallable)
	void Attack();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	TObjectPtr<AEnemyPatrolRoute> PatrolRoute;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemyDataAsset> EnemyConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> EnemyWidgetComponent;
	
	FTransform InitialSpawnTransform;
	
public:
	
	FORCEINLINE UBehaviorTree* GetBehaviourTree() const { return BehaviourTree; }
	FORCEINLINE void SetPatrolRoute(AEnemyPatrolRoute* Route) { PatrolRoute = Route; }
};
