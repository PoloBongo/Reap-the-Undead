#include "Planting/PlantingSystem.h"
#include "TimerManager.h"

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
	GetWorldTimerManager().SetTimer(PlantingRateTimerHandle, this, &APlantingSystem::EvolutionPlanting, EvolutionRate, true);
}

void APlantingSystem::EvolutionPlanting()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("next evolution"));
	SwitchMesh(IndexActualMesh);

	IndexActualMesh++;
}

void APlantingSystem::SwitchMesh(int _indexMesh)
{
	if (!PlantsMesh.IsValidIndex(_indexMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("index %d invalid"), _indexMesh);
		return;
	}

	if (PlantMesh)
	{
		PlantMesh->SetStaticMesh(PlantsMesh[_indexMesh]);
		UE_LOG(LogTemp, Log, TEXT("mesh switch pour l'index %d"), _indexMesh);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("le plantMesh est null"));
	}
	if (_indexMesh == PlantsMesh.Max() - 2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("end of evolution"));
		GetWorld()->GetTimerManager().ClearTimer(PlantingRateTimerHandle);
	}
}