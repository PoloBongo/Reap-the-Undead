#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "PlantManager.generated.h"

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
	TSubclassOf<APlantingSystem> PlantingSystem;

	virtual void InteractObject() override;
};
