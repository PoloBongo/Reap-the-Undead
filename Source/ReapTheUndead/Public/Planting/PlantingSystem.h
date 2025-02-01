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
	FTimerHandle FirstPlantingRateTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Plant")
	float EvolutionRate = 3.f;

	UPROPERTY(EditAnywhere, Category="Plant")
	float FirstEvolutionRate = 5.f;

	UPROPERTY(EditAnywhere, Category="Plant")
	int IndexActualMesh = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara Effect", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* EvolutionEffect;

	UFUNCTION()
	void OnNiagaraFinished(UNiagaraComponent* NiagaraComponent);
	
	UFUNCTION()
	void InitFirstMeshEvolve();

	bool AntiSpawnNiagaraForFirstUsage = false;
};
