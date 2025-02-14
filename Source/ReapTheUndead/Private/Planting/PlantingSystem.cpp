#include "Planting/PlantingSystem.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Interactable/Plant/Plants.h"
#include "Inventory/InventorySystem.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Kismet/GameplayStatics.h"
#include "Notification/GameNotificationManager.h"

APlantingSystem::APlantingSystem()
{
	PrimaryActorTick.bCanEverTick = false;

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant Mesh"));
	RootComponent = PlantMesh;
}

void APlantingSystem::BeginPlay()
{
	Super::BeginPlay();
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
			AssociatePlant->ChangeText(FString::Printf(TEXT("Evolution : %d/%d"), IndexActualMesh + 1, PlantsMesh.Num()));
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
	AssociatePlant->SetCanHarvest(true);
	AssociatePlant->ChangeText("Appuyez sur [E] pour recolter la plante");
}

void APlantingSystem::HarvestPlant()
{
	if (IndexActualMesh < PlantsMesh.Num()) return;
	if (AInventorySystem* InventorySystem = Cast<AInventorySystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AInventorySystem::StaticClass())))
	{
		InventorySystem->AddItem(DataItems, 1);
		GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Vous venez de récolter : %s"), *DataItems->Image->GetName()), FColor::Green);
		InventorySystem->SaveInventoryToFile();
		InventorySystem->LoadInventoryFromFile();
		if (this)
		{
			Destroy();
		}
		if (AssociatePlant) {
			AssociatePlant->Destroyed();
		}
	}
}

void APlantingSystem::SetDataAsset(UInventoryDataItems* DataAsset)
{
	DataItems = DataAsset;
}

void APlantingSystem::SetAssociatePlant(APlants* Plant)
{
	AssociatePlant = Plant;
}
