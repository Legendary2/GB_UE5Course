#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "DestroyedFactory.h"
#include "TankFactory.generated.h"


class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UHealthComponent;
class ATankPawn;
class ATargetPoint;
class AMapLoader;
class UParticleSystem;
class USoundBase;
class ADestroyedFactory;

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestuctionParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* SpawnParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* SpawnSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DestroyedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	AMapLoader* LinkedMapLoader;
public:
	ATankFactory();
	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;


protected:
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

};

