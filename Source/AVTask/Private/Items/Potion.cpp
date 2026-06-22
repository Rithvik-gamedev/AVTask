#include "Items/Potion.h"

#include "AbilitySystemComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "Character/HeroCharacter.h"
#include "Items/PotionWidget.h"


APotion::APotion()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	PotionMesh->SetupAttachment(RootComp);
	PotionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComp);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetSphereRadius(50.f);
	SphereComponent->SetHiddenInGame(false);

	WidgetZone = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetZone"));
	WidgetZone->SetupAttachment(RootComp);
	WidgetZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WidgetZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	WidgetZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WidgetZone->SetSphereRadius(250.f);
	WidgetZone->SetHiddenInGame(false);

	PotionLabelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PotionLabelWidget"));
	PotionLabelWidget->SetupAttachment(RootComp);
	PotionLabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PotionLabelWidget->SetDrawAtDesiredSize(true);
	PotionLabelWidget->SetVisibility(false);
	PotionLabelWidget->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
}

void APotion::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (PotionLabelWidget)
	{
#if WITH_EDITOR
		
		if (!PotionLabelWidget->GetUserWidgetObject())
		{
			PotionLabelWidget->InitWidget();
		}
#endif
		
		UPotionWidget* PotionWidget = Cast<UPotionWidget>(PotionLabelWidget->GetUserWidgetObject());
		if (PotionWidget)
		{
			PotionWidget->UpdateText(PotionText);
		}
	}
}

void APotion::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlapBegin);
	}
	
	UPotionWidget* PotionWidget = Cast<UPotionWidget>(PotionLabelWidget->GetUserWidgetObject());
	if (PotionWidget)
	{
		PotionWidget->UpdateText(PotionText);
	}

	StartMeshLocation = PotionMesh->GetRelativeLocation();
	
	GetWorld()->GetTimerManager().SetTimer(HoverTimerHandle, this, &APotion::UpdateHover, 0.016f, true);
	
	WidgetZone->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnWidgetZoneOverlap);
	WidgetZone->OnComponentEndOverlap.AddDynamic(this, &APotion::OnWidgetZoneEndOverlap);
}

void APotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor || OtherActor == this || !PotionEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);;
	FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Player.Character");

	if (ASC && ASC->HasMatchingGameplayTag(PlayerTag))
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PotionEffectClass, 1.f, ContextHandle);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			Destroy();
		}
	}
}

void APotion::UpdateHover()
{
	CurrentHoverTime += 0.016f;
	FVector NewLocation = StartMeshLocation;
	NewLocation.Z += FMath::Sin(CurrentHoverTime * HoverSpeed) * HoverAmplitude;
	PotionMesh->SetRelativeLocation(NewLocation);
}

void APotion::OnWidgetZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AHeroCharacter* Char = Cast<AHeroCharacter>(OtherActor);
		if (Char && Char->IsLocallyControlled())
		{
			PotionLabelWidget->SetVisibility(true);
		}
	}
}

void APotion::OnWidgetZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		AHeroCharacter* Char = Cast<AHeroCharacter>(OtherActor);
		if (Char && Char->IsLocallyControlled())
		{
			PotionLabelWidget->SetVisibility(false);
		}
	}
}

