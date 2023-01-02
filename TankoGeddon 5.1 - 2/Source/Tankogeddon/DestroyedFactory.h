#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyedFactory.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TANKOGEDDON_API ADestroyedFactory : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
public:	
	ADestroyedFactory();

protected:
	virtual void BeginPlay() override;

};
