#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "AmmoBox.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ParentFirePoint.generated.h"

class UStaticMeshComponent;
class ACannon;
class UHealthComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class TANKOGEDDON_API AParentFirePoint : public APawn, public IDamageTaker
{
	GENERATED_BODY()
	
public:	

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BodyMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    UParticleSystem* DestuctionParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* DestructionSound;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UArrowComponent* CannonSetupPoint;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* HitCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    TSubclassOf<ACannon> CannonClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    TSubclassOf<AAmmoBox> AmmoboxClass;
    UPROPERTY()
    ACannon* Cannon;

public:	
    AParentFirePoint();

    UFUNCTION()
    virtual void Die();

    UFUNCTION()
    virtual void DamageTaken(float InDamage);

    UFUNCTION()
    void TakeDamage(FDamageData DamageData);

    UFUNCTION()
    void Fire();
protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;
};
