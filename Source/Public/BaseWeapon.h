#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class UE4_PORTPOLIO_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
#pragma region WeaponComponents
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;
#pragma endregion

};
