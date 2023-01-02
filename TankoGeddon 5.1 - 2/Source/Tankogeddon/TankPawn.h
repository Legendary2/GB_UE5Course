#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "Components/BoxComponent.h"
#include "ParentFirePoint.h"
#include "HealthComponent.h"

#include "TankPawn.generated.h"



class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;
class UHealthComponent;
class ATargetPoint;

UCLASS()
class TANKOGEDDON_API ATankPawn : public AParentFirePoint
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void RotateRight(float AxisValue);


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
		float TurretRotationInterpolationKey = 0.5f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> CannonClassSecond;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
		TArray<ATargetPoint*> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")

		float MovementAccurency = 50;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void FireSpecial();

	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
		void ChangeCannon();

	UFUNCTION()
		void SetNewCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
		void IncreaseAmmunition(int Ammunition);

	UFUNCTION()
		TArray<FVector> GetPatrollingPoints();

	UFUNCTION()
		void SetPatrollingPoints(const TArray<ATargetPoint*>& NewPatrollingPoints);
	UFUNCTION()
		float GetMovementAccurency()
	{
		return MovementAccurency;
	};
	UFUNCTION()
		FVector GetTurretForwardVector();

	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);

	UFUNCTION()
		FVector GetEyesPosition();

private:
	float TargetForwardAxisValue;
	float TargetRightAxisValue;
	float CurrentRightAxisValue;

	TSubclassOf<ACannon> CurrentCannon;
	UPROPERTY()
		ATankPlayerController* TankController;
};
