#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantingSystem.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class REAPTHEUNDEAD_API APlantingSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	APlantingSystem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void StartPlanting();
	void EvolutionPlanting();
	void SwitchMesh(int _indexMesh);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* PlantMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plants Mesh", meta=(AllowPrivateAccess="true"))
	TArray<UStaticMesh*> PlantsMesh;
	
	FTimerHandle PlantingRateTimerHandle;
	FTimerHandle ShowPlantingRateTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Plant")
	TArray<float> EvolutionRate;

	UPROPERTY(EditAnywhere, Category="Plant")
	float DelayShowPlant = 1.f;

	UPROPERTY(EditAnywhere, Category="Plant")
	int IndexActualMesh = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara Effect", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* EvolutionEffect;

	void OnNiagaraFinished();
};
