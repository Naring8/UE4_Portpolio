// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../Interfaces/CharacterMovementInterface.h>
#include "BaseCharacter.generated.h"

UCLASS()
class UE4_PORTPOLIO_API ABaseCharacter : public ACharacter, public ICharacterMovementInterface
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

private:
	UFUNCTION(BlueprintCallable)
		void CustomCrouch();
	UFUNCTION(BlueprintCallable)
		void CustomUnCrouch();

private:
	virtual void Walk() override;
	virtual void Run() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	TSubclassOf<class ABaseWeapon> curWeapon;

private:
	float walkSpeed = 200.0f;
	float runSpeed = 600.0f;

	float DefaultCapsuleHalfHeight = 95.0f;
	float DefaultCapsuleRadius = 35.0f;

	float CrouchCapsuleHalfHeight = 65.0f;

};
