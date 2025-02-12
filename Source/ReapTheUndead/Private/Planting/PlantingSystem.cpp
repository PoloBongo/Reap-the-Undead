#include "Planting/PlantingSystem.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Inventory/InventorySystem.h"
#include "Kismet/GameplayStatics.h"

APlantingSystem::APlantingSystem()
{
	PrimaryActorTick.bCanEverTick = false;

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant Mesh"));
	RootComponent = PlantMesh;
}

void APlantingSystem::BeginPlay()
{
	Super::BeginPlay();
	if (!HarvestWidget) return;
	FVector SpawnLocation(0, 0, AddingZOnWidgetHarvest);
	HarvestWidget->AddRelativeLocation(SpawnLocation);
	HarvestWidget->SetVisibility(false);
	StartPlanting();
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
		else
		{
			ShowHarvestPlant();
		}
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

void APlantingSystem::ShowHarvestPlant()
{
	if (!HarvestWidget) return;
	HarvestWidget->SetVisibility(true);
	CanHarvest = true;
}

void APlantingSystem::HarvestPlant()
{
	if (!CanHarvest) return;
	if (!HarvestWidget) return;
	HarvestWidget->SetVisibility(false);

	if (AInventorySystem* InventorySystem = Cast<AInventorySystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AInventorySystem::StaticClass())))
	{
		InventorySystem->AddItem(DataItems, 1);
		if (this)
		{
			Destroy();
		}
	}
}

void APlantingSystem::SetDataAsset(UInventoryDataItems* DataAsset)
{
	DataItems = DataAsset;
}

bool APlantingSystem::GetCanHarvest() const
{
	return CanHarvest;
}

UWidgetComponent* APlantingSystem::GetWidgetComponent() const
{
	return HarvestWidget;
}
