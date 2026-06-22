// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBoxNotifyState.h"

#include "Character/CustomCharacterBase.h"
#include "Weapon/WeaponBase.h"

void UWeaponBoxNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACustomCharacterBase* Character = Cast<ACustomCharacterBase>(MeshComp->GetOwner());
	if (Character && Character->GetCurrentWeapon())
	{
		Character->GetCurrentWeapon()->SetWeaponCollisionEnabled(true);
	}
}

void UWeaponBoxNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UWeaponBoxNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ACustomCharacterBase* Character = Cast<ACustomCharacterBase>(MeshComp->GetOwner());
	if (Character && Character->GetCurrentWeapon())
	{
		Character->GetCurrentWeapon()->SetWeaponCollisionEnabled(false);
	}
}

