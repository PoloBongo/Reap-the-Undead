#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "PlantManager.generated.h"

class ATargetPoint;
class APlantingSystem;
/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API APlantManager : public AInteractableObjects
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Planting System")
	TSubclassOf<APlantingSystem> PlantingSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Point", meta=(AllowPrivateAccess="true"))
	ATargetPoint* LocationSpawnObject;
	
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;
};
