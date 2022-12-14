// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"

#include "XCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADefaultCharacter::ADefaultCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UXCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	XCharacterMovementComponent = Cast<UXCharacterMovementComponent>(GetCharacterMovement());

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), FName("head"));
}

// Called when the game starts or when spawned
void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// check if this is the server
	//if (HasAuthority()) {
	//	for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons)
	//	{
	//		if (!WeaponClass) continue;
	//		FActorSpawnParameters Params;
	//		Params.Owner = this;
	//		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params);
	//		const int32 Index = Weapons.Add(SpawnedWeapon);
	//		if (Index == CurrentIndex)
	//			CurrentWeapon = SpawnedWeapon;
	//		{
	//			OnRep_CurrentWeapon(nullptr);
	//		}
	//	}
	//}
}

// Called every frame
void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction(FName("NextWeapon"), EInputEvent::IE_Pressed, this, &ADefaultCharacter::NextWeapon);
	//PlayerInputComponent->BindAction(FName("LastWeapon"), EInputEvent::IE_Pressed, this, &ADefaultCharacter::LastWeapon);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ADefaultCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &ADefaultCharacter::LookRight);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ADefaultCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Released,this, &ADefaultCharacter::StopJumping);

	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Pressed, this, &ADefaultCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Released, this, &ADefaultCharacter::StopSprinting);


	PlayerInputComponent->BindAction(FName("Crouch"), EInputEvent::IE_Pressed, this, &ADefaultCharacter::Crouch);
	PlayerInputComponent->BindAction(FName("Crouch"), EInputEvent::IE_Released, this, &ADefaultCharacter::StopCrouching);
}

// Player Movement
void ADefaultCharacter::MoveForward(const float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Forwards"));

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::MoveRight(const float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Right"));

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::LookUp(const float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Look Up"));

	AddControllerPitchInput(Value);
}

void ADefaultCharacter::LookRight(const float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Look Right"));

	AddControllerYawInput(Value);
}

 void ADefaultCharacter::Sprint()
 {
 	//UE_LOG(LogTemp, Warning, TEXT("Look Right"));
 	XCharacterMovementComponent->SprintPressed();
	isSprinting = true;
 }
 
 void ADefaultCharacter::StopSprinting()
 {
 	//UE_LOG(LogTemp, Warning, TEXT("Look Right"));
 	XCharacterMovementComponent->SprintReleased();
	isSprinting = false;

 }

 void ADefaultCharacter::Crouch()
 {
 	//UE_LOG(LogTemp, Warning, TEXT("Look Right"));
 	XCharacterMovementComponent->CrouchPressed();
	XCharacterMovementComponent->SetJumpAllowed(true);
	isCrouched = true;
 }
 
 void ADefaultCharacter::StopCrouching()
 {
 	//UE_LOG(LogTemp, Warning, TEXT("Look Right"));
 	XCharacterMovementComponent->CrouchReleased();
	isCrouched = false;
 }

 // // override jump functions
 // void ADefaultCharacter::Jump()
 // {
 //	  Super();
 // }
 // 
 // void ADefaultCharacter::StopJumping()
 // {
 //	  Super();
 // }

 // if true the character can jump, if false they can't (if in a certain movement mode for example)
 bool ADefaultCharacter::CanJumpInternal_Implementation() const
 {
	 bool canJump = false;

	 // checks if the movement mode allows us to jump, and if the character is on the ground
	 if (XCharacterMovementComponent->IsJumpAllowed() &&
		 (XCharacterMovementComponent->IsMovingOnGround())) 
	 {
		 canJump = true;
	 }
	 else
	 {
		 canJump = false;
	 }

	return canJump;
 }