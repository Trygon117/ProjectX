#include "XCharacterMovementComponent.h"

#include "DefaultCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UXCharacterMovementComponent::UXCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

bool UXCharacterMovementComponent::FSavedMove_X::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_X* NewXMove = static_cast<FSavedMove_X*>(NewMove.Get());
		
	if (Saved_bWantsToSprint != NewXMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UXCharacterMovementComponent::FSavedMove_X::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UXCharacterMovementComponent::FSavedMove_X::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

void UXCharacterMovementComponent::FSavedMove_X::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UXCharacterMovementComponent* CharacterMovement = Cast<UXCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UXCharacterMovementComponent::FSavedMove_X::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UXCharacterMovementComponent* CharacterMovement = Cast<UXCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UXCharacterMovementComponent::FNetworkPredictionData_Client_X::FNetworkPredictionData_Client_X(const UXCharacterMovementComponent& ClientMovement) 
	: Super(ClientMovement)
{
}

FSavedMovePtr UXCharacterMovementComponent::FNetworkPredictionData_Client_X::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_X);
}

FNetworkPredictionData_Client* UXCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UXCharacterMovementComponent* MutableThis = const_cast<UXCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_X(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UXCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_X::FLAG_Custom_0) != 0;
}

void UXCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("Update"));

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Walking"));

		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else {
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

UFUNCTION(BlueprintCallable) void UXCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

UFUNCTION(BlueprintCallable) void UXCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

UFUNCTION(BlueprintCallable) void UXCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = true;
}

UFUNCTION(BlueprintCallable) void UXCharacterMovementComponent::CrouchReleased()
{
	bWantsToCrouch = false;
}