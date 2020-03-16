// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkillInfo.h"
#include "Engine.h"
#include "PowerUpElement.h"
#include "Containers/Queue.h"
#include "CharacterBase.generated.h"

class AInteractableBase;

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	VE_Father	UMETA(DisplayName = "Father"),
	VE_Girl		UMETA(DisplayName = "Kid")
};

UCLASS()
class MCGILL_1_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	ACharacterBase();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// function to manipulate power up queue
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	virtual void CollectPowerUpElement(EPowerUpType elem);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PowerUp")
	void ActivatePowerUp();

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	bool DetermineTriggerablePowerUp(ACharacterBase* aimingPlayer);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PowerUp")
	void NotEnoughPowerUpElementNotify();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PowerUp")
	void ActivateSpeedPowerUp();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PowerUp")
	void ActivateAttackPowerUp();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PowerUp")
	void ActivateInvincibilityPowerUp();

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	virtual TArray<EPowerUpType> GetAllPowerUpElements();

public:

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TurnRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TurnForward(float Value);

	// character attacking function, implemented in blueprint and overriden by child character
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, category = "Skill")
	void Attack();

	UFUNCTION(BlueprintCallable, category = "Custom")
	virtual void InteractObject();

	void RegisterInteractableObject(AInteractableBase* ObjectToInteract);
	void UnRegisterInteractableObject(AInteractableBase* ObjectToInteract);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerRole CharacterRole;

	UPROPERTY(EditAnyWhere, category=setup)
	FSkillInfo Skill;

protected:
    int PowerUpElemCount = 0;
	TMap<EPowerUpType, int> Portion;
    TQueue<EPowerUpType> PowerUpQueue;
	AInteractableBase* ToInteract = nullptr;
};
