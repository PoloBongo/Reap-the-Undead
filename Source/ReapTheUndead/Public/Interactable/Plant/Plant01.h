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

public:
	virtual void Tick(float DeltaTime) override;
	
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
