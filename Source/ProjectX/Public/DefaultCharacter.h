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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanJumpInternal_Implementation() const override;

	// the weapon classes spawned by default
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
		TArray <TSubclassOf<class AWeapon>> DefaultWeapons;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
		TArray<class AWeapon*> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
		class AWeapon* CurrentWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
		int32 CurrentWeaponIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual void EquipWeapon(const int32 Index);

protected:
	UFUNCTION()
		virtual void OnRep_CurrentWeapon(const class AWeapon* OldWeapon);

	UFUNCTION(Server, Reliable)
		void Server_SetCurrentWeapon(class AWeapon* Weapon);
	virtual void Server_SetCurrentWeapon_Implementation(class AWeapon* Weapon);

private:
	virtual void NextWeapon();
	virtual void PreviousWeapon();

	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);

	void Sprint();
	void StopSprinting();

	void thisCrouch();
	void thisStopCrouching();

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
