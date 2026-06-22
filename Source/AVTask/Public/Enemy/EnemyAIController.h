// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;
class UBehaviorTreeComponent;

UCLASS()
class AVTASK_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();
	
	virtual void OnPossess(APawn* InPawn) override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetKeyName = "TargetActor";
	
	
	public:
	
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; }
};
