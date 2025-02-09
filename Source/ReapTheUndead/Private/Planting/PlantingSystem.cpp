#include "Planting/PlantingSystem.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"

APlantingSystem::APlantingSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant Mesh"));
	RootComponent = PlantMesh;
}

void APlantingSystem::BeginPlay()
{
	Super::BeginPlay();

	StartPlanting();
}

void APlantingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlantingSystem::StartPlanting()
{
	GetWorldTimerManager().SetTimer(PlantingRateTimerHandle, this, &APlantingSystem::EvolutionPlanting, EvolutionRate[0], true);
}

void APlantingSystem::EvolutionPlanting()
{
	if (IndexActualMesh < PlantsMesh.Num())
	{
		GetWorldTimerManager().SetTimer(ShowPlantingRateTimerHandle, this, &APlantingSystem::OnNiagaraFinished, DelayShowPlant, false);

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			EvolutionEffect,
			PlantMesh,
			NAME_None,
			FVector(0.f),
			FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset,
			true);
	}
}

void APlantingSystem::SwitchMesh(int _indexMesh)
{
	if (!PlantsMesh.IsValidIndex(_indexMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("Index %d is invalid"), _indexMesh);
		GetWorld()->GetTimerManager().ClearTimer(PlantingRateTimerHandle);
		return;
	}

	if (PlantMesh)
	{
		PlantMesh->SetStaticMesh(PlantsMesh[_indexMesh]);

		GetWorld()->GetTimerManager().ClearTimer(PlantingRateTimerHandle);
		if (_indexMesh + 1 < PlantsMesh.Num())
		{
			GetWorldTimerManager().SetTimer(PlantingRateTimerHandle, this, &APlantingSystem::EvolutionPlanting, EvolutionRate[_indexMesh + 1], true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlantMesh is null"));
	}
}

void APlantingSystem::OnNiagaraFinished()
{
	if (IndexActualMesh < PlantsMesh.Num())
	{
		SwitchMesh(IndexActualMesh);
		IndexActualMesh++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PlantingRateTimerHandle);
	}
}
