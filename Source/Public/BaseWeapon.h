#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <../Basic/CustomStruct.h>
#include <../Interfaces/WeaponInterface.h>

#include <AssassinationComponent.h>
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class UE4_PORTPOLIO_API ABaseWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FName GetWeaponSocketName() { return WeaponSocket; }
	void BaseAttack();

	UFUNCTION(BlueprintCallable)
		void DoAssassinate();

private:
	void SetWidgetVisibility(ESlateVisibility const Visibility) {}

	void SetWeaponIdle(UAnimMontage* const Montage, float const PlayRate, FName const Section) {} //TODO
	void PlayWeaponAssetMontage();

private: // Weapon Interface
#pragma region WeaponStateInterface
	virtual void EnableAttack() override;
	virtual void DisableAttack() override;
#pragma endregion
#pragma region WeaponActionInterface
	virtual void ActivateAttack() override;
	virtual void DeactivateAttack() override;
#pragma endregion

	UFUNCTION()
		void AttackContinued(UAnimMontage* const Montage, bool const bInterrupted);

	UFUNCTION()
		void AttackEnded(UAnimMontage* const Montage, bool const bInterrupted);

protected:
#pragma region WeaponComponents
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
		class UAssassinationComponent* Assassination;
#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponInformation", meta = (AllowPrivateAccess))
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAssets", meta = (AllowPrivateAccess))
		class UMontageDataAsset* ActingData;

private:
	int32 AttackIndex = 0;
	bool bAttackTransisted = false;
	bool bAttackEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess))
		FName WeaponSocket;

	/*UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		class UUserWidget* PickUpWidget;*/

private:
	class ACharacter* OwnerCharacter;
	bool bCanAssassinate = false;
};
