#pragma once


#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "ParentFirePoint.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class UHealthComponent;
class ATankPawn;

UCLASS()
class TANKOGEDDON_API ATurret : public AParentFirePoint
{
    GENERATED_BODY()
protected:
    
    UPROPERTY()
    APawn* PlayerPawn;

    //UPROPERTY()
    //ATankPawn* TankPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange = 1000;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed = 0.1f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRate = 0.005f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accurency = 10;

    const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
    const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

public:
    ATurret();

    virtual void PostInitializeComponents() override;
protected:
    virtual void BeginPlay() override;
    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    bool CanFire();
    bool DetectPlayerVisibility();
    FVector GetEyesPosition();
};