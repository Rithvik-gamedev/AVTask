// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Character/CustomAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "Components/BoxComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(WeaponMesh);
	BoxCollision->SetHiddenInGame(false);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::SetWeaponCollisionEnabled(bool bEnabled)
{
	if (!BoxCollision) return;
	
	if (bEnabled)
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::AWeaponBase::OnBoxOverlap);
	}
}

void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponBase::AWeaponBase::OnBoxOverlap);
	}
}

void AWeaponBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator() || OtherActor == GetOwner()) return;
	
	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	
	if (TargetASC && InstigatorASC)
	{
		FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Player.Character");
		FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag("Enemy.Character");
		
		bool bIsInstigatorPlayer = InstigatorASC->HasMatchingGameplayTag(PlayerTag);
		bool bIsTargetPlayer = TargetASC->HasMatchingGameplayTag(PlayerTag);
		
		// If both the instigator and target are players or both are enemies, do not apply damage
		if (bIsInstigatorPlayer == bIsTargetPlayer)
		{
			return;
		}
		
		// Apply Damage
		if (DamageEffectClass)
		{
			FGameplayEffectContextHandle EffectContextHandle = InstigatorASC->MakeEffectContext();
			EffectContextHandle.AddInstigator(GetInstigator(), this);
			
			if (bFromSweep)
			{
				EffectContextHandle.AddHitResult(SweepResult);
			}
			
			FGameplayEffectSpecHandle EffectSpecHandle = InstigatorASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				
				InstigatorASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
			}
		}
	}
}

