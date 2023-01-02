#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0), Category = "Ammunition")
	int32 Ammunition = 5;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Ammunition <= 0", EditConditionHides), Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditCondition = "Ammunition <= 0", EditConditionHides), Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;


public:
	AAmmoBox();

protected:

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
