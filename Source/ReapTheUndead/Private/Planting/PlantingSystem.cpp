#include "Planting/PlantingSystem.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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
	GetWorldTimerManager().SetTimer(FirstPlantingRateTimerHandle, this, &APlantingSystem::InitFirstMeshEvolve, FirstEvolutionRate, false);
	GetWorldTimerManager().SetTimer(PlantingRateTimerHandle, this, &APlantingSystem::EvolutionPlanting, EvolutionRate, true);
}

void APlantingSystem::EvolutionPlanting()
{
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
	EvolutionEffect,
	PlantMesh,
	NAME_None,
	FVector(0.f),
	FRotator(0.f),
	EAttachLocation::Type::KeepRelativeOffset,
	true);

	if (NiagaraComponent)
	{
		NiagaraComponent->OnSystemFinished.AddDynamic(this, &APlantingSystem::OnNiagaraFinished);
	}
}

void APlantingSystem::SwitchMesh(int _indexMesh)
{
	if (!PlantsMesh.IsValidIndex(_indexMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("index %d invalid"), _indexMesh);
		GetWorld()->GetTimerManager().ClearTimer(PlantingRateTimerHandle);
		return;
	}

	if (PlantMesh)
	{
		PlantMesh->SetStaticMesh(PlantsMesh[_indexMesh]);
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

void APlantingSystem::OnNiagaraFinished(UNiagaraComponent* NiagaraComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("next evolution"));
	SwitchMesh(IndexActualMesh);
	IndexActualMesh++;
}

void APlantingSystem::InitFirstMeshEvolve()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("first evolve"));
	SwitchMesh(IndexActualMesh);
	IndexActualMesh++;
}
