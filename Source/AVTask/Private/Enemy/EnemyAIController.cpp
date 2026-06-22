// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Enemy Behavior Tree");
	check(BehaviorTreeComponent);
	
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception Component");
	check(AIPerceptionComponent);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);
	if (Enemy && Enemy->GetBehaviourTree())
	{
		RunBehaviorTree(Enemy->GetBehaviourTree());
	}
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
    
	if (!TargetASC) return;
	
	FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Player.Character");
	if (!TargetASC->HasMatchingGameplayTag(PlayerTag))
	{
		return;
	}
	
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->SetValueAsObject(TargetKeyName, Actor);
		}
		else
		{
			BlackboardComp->ClearValue(TargetKeyName);
		}
	}
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}
