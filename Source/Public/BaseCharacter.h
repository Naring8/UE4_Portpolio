// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include <../Interfaces/AnimationInterface.h>
#include <../Interfaces/CharacterStateInterface.h>
#include <../Interfaces/CharacterMovementInterface.h>

#include "BaseCharacter.generated.h"

UCLASS(ABSTRACT)
class UE4_PORTPOLIO_API ABaseCharacter : public ACharacter, 
										 public IAnimationInterface,
										 public ICharacterStateInterface,
										 public ICharacterMovementInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void OnConstruction(FTransform const& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PlayAnimation(class UAnimMontage* const Montage, float const PlayRate, FName const Section) override;

	UFUNCTION()
		void ResetToIdle(UAnimMontage* const Montage, bool const bInterrupted);
private:
	UFUNCTION(BlueprintCallable)
		void CustomCrouch();
	UFUNCTION(BlueprintCallable)
		void CustomUncrouch();

private:
#pragma region CharacterStateInterface
	virtual ECharacterState GetCharacterState() override { return CharacterState; }
	virtual void SetCharacterState(ECharacterState const StateType) override { CharacterState = StateType; }

	virtual void CharacterDead() override;
	virtual void CharacterDetected(bool const IsDetected) override { bIsDetected = IsDetected; }
	
#pragma endregion

#pragma region CharacterMovementInterface
	virtual void Walk() override { GetCharacterMovement()->MaxWalkSpeed = walkSpeed; }
	virtual void Run() override { GetCharacterMovement()->MaxWalkSpeed = runSpeed; }

	virtual void ChangeWeapon() override;
	virtual void BaseAttack() override;
	virtual void Stealth() override;
#pragma endregion

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess))
		TArray<TSubclassOf<class ABaseWeapon>> WeaponClasses;

	int32 WeaponIdx = 0;
	TArray<class ABaseWeapon*> Weapons;

private:
	float walkSpeed = 200.0f;
	float runSpeed = 600.0f;

	float DefaultCapsuleHalfHeight = 95.0f;
	float DefaultCapsuleRadius = 35.0f;

	float CrouchCapsuleHalfHeight = 65.0f;

	ECharacterState CharacterState = ECharacterState::IDLE;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
		bool bIsDetected = false;
};
