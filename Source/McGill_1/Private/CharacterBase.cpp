// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "SkillUsage.h"
#include "InteractableBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // move at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->Skill.Timer += DeltaTime;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	PlayerInputComponent->BindAxis("TurnX", this, &ACharacterBase::TurnRight);
	PlayerInputComponent->BindAxis("TurnY", this, &ACharacterBase::TurnForward);

	//Bind input for interact object
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::InteractObject);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::Attack);
	PlayerInputComponent->BindAction("Special", IE_Pressed, this, &ACharacterBase::ActivatePowerUp);
}

void ACharacterBase::CollectPowerUpElement(EPowerUpType elem)
{
	PowerUpQueue.Enqueue(elem);
	PowerUpElemCount++;
	if(PowerUpElemCount > 5)
	{
		EPowerUpType t;
		PowerUpQueue.Dequeue(t);
		PowerUpElemCount--;
	}
	UE_LOG(LogTemp, Warning, TEXT("power up count %d"), PowerUpElemCount);
}

bool ACharacterBase::DetermineTriggerablePowerUp(ACharacterBase* aimingPlayer)
{
	if(PowerUpElemCount < 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("not enough power up"));
		NotEnoughPowerUpElementNotify();
		return false;
	}
	else
	{
		// get a list of all the elements in the queue
		// add to map
		TArray<EPowerUpType> list;
		list = GetAllPowerUpElements();
		for(EPowerUpType& type : list)
		{
			if(!Portion.Contains(type))
			{
				Portion.Add(type, 1);
			}
			else
			{
				Portion[type] += 1;
			}
		}
	}
	// clean up structure for next use
	// we can reuse map to record value for us
	// at last, we find out the biggest portion
	int max = -1;
	EPowerUpType triggeredPowerUp;
	
	for(auto& e : Portion)
	{
		if(e.Value > max)
		{
			triggeredPowerUp = e.Key;
			max = e.Value;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d, %d"), (int) e.Key, e.Value);
		e.Value = 0;
	}

	PowerUpQueue.Empty();
	PowerUpElemCount = 0;

	USkillUsage::ActivatePowerUp(aimingPlayer, triggeredPowerUp);
	UE_LOG(LogTemp, Warning, TEXT("Trigger %d"), (int)triggeredPowerUp);
	return true;
}

TArray<EPowerUpType> ACharacterBase::GetAllPowerUpElements()
{
	TArray<EPowerUpType> list;
    for (int i=0; i<PowerUpElemCount; ++i) 
	{
		EPowerUpType t;
		PowerUpQueue.Dequeue(t);
		list.Add(t);
		PowerUpQueue.Enqueue(t);
	}
	return list;
}

void ACharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::TurnRight(float Value)
{
	if((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value*0.001);
	}
}

void ACharacterBase::TurnForward(float Value)
{
	if((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value*0.001);
	}
}

void ACharacterBase::InteractObject()
{
	if (ToInteract != nullptr) {
		ToInteract->Trigger();
	}
}

void ACharacterBase::RegisterInteractableObject(AInteractableBase * ObjectToInteract)
{
	if (ToInteract == nullptr) {
		ToInteract = ObjectToInteract;
	}
}

void ACharacterBase::UnRegisterInteractableObject(AInteractableBase * ObjectToInteract)
{
	if (ToInteract != nullptr) {
		ToInteract = nullptr;
	}
}
