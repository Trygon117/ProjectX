// Fill out your copyright notice in the Description page of Project Settings.


#include "XPlayerCameraManager.h"

#include "DefaultCharacter.h"
#include "XCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AXPlayerCameraManager::AXPlayerCameraManager()
{
}

void AXPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ADefaultCharacter* XCharacter = Cast<ADefaultCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UXCharacterMovementComponent* XMC = XCharacter->GetXCharacterMovement();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			XMC->GetCrouchedHalfHeight() - XCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (XMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}