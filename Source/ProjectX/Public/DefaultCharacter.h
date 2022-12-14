// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XCharacterMovementComponent.h"


#include "DefaultCharacter.generated.h"

UCLASS()
class PROJECTX_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement) 
		class UXCharacterMovementComponent* XCharacterMovementComponent;

public:
	// Sets default values for this character's properties
	ADefaultCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual bool CanJumpInternal_Implementation() const override;

private:

	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);

	void Sprint();
	void StopSprinting();

	void Crouch();
	void StopCrouching();

//	virtual void Jump() override;
//	virtual void StopJumping() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
		bool isCrouched = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
		bool isSprinting = false;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE UXCharacterMovementComponent* GetXCharacterMovement() const { return XCharacterMovementComponent; }

};
