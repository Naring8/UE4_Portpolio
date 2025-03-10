#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <../Basic/CustomStruct.h>
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
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;
#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponInformation", meta = (AllowPrivateAccess))
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAssets", meta = (AllowPrivateAccess))
		class UMontageDataAsset* ActingData;	// replace to map(container)
};
