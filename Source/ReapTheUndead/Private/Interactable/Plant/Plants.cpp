#include "Interactable/Plant/Plants.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Planting/PlantingSystem.h"
#include "Engine/TargetPoint.h"

void APlants::InteractObject()
{
	
	Super::InteractObject();
	if (PlantingSystem)
	{
		PlantingSystem->SetDataAsset(DataItems);
		PlantingSystem->HarvestPlant();
	}
	else
	{
		if (IsAlreadyPlanted) return;

		if (PlantingSystemClass && LocationSpawnObject)
		{
			FVector SpawnLocation = LocationSpawnObject->GetActorLocation();
			FRotator SpawnRotation = LocationSpawnObject->GetActorRotation();

			PlantingSystem = GetWorld()->SpawnActor<APlantingSystem>(PlantingSystemClass, SpawnLocation, SpawnRotation);
		}

		IsAlreadyPlanted = true;
		if(UIUserInteract && IsAlreadyPlanted) UIUserInteract->SetVisibility(ESlateVisibility::Hidden);	
	}
}

void APlants::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("trigger plant01"));
	
	if (IsAlreadyPlanted)
	{
		if (!ActualEvolveMesh)
		{
			ActualEvolveMesh = Cast<UStaticMeshComponent>(PlantingSystem->GetRootComponent());
		}
		if (!ActualEvolveMesh->GetStaticMesh()) return;
		if (!MaterialOverlay) return;
		
		ActualEvolveMesh->SetOverlayMaterial(MaterialOverlay);
	}
}

void APlants::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);
	
	if (ActualEvolveMesh)
	{
		ActualEvolveMesh->SetOverlayMaterial(nullptr);
	}
}

bool APlants::GetCanHarvestFromPlantingSystem() const
{
	if (PlantingSystem)
	{
		return PlantingSystem->GetCanHarvest();
	}
	return false;
}

UWidgetComponent* APlants::GetUWidgetComponentFromPlantingSystem() const
{
	if (PlantingSystem)
	{
		return PlantingSystem->GetWidgetComponent();
	}
	return nullptr;
}
