#pragma once

#include "CoreMinimal.h"
#include "Planting/PlantManager.h"
#include "Plant01.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API APlant01 : public APlantManager
{
	GENERATED_BODY()

protected:
	virtual void InteractObject() override;
};
