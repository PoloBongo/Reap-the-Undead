#pragma once

#include "CoreMinimal.h"
#include "Planting/PlantManager.h"
#include "Plants.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API APlants : public APlantManager
{
	GENERATED_BODY()
	
protected:
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interact", meta = (AllowPrivateAccess = "true"))
	bool IsAlreadyPlanted = false;
private:
	APlantingSystem* PlantingSystem;
	UStaticMeshComponent* ActualEvolveMesh;
};
