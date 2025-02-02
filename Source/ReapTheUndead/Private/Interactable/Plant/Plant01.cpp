#include "Interactable/Plant/Plant01.h"

#include "Planting/PlantingSystem.h"
#include "Engine/TargetPoint.h"

void APlant01::InteractObject()
{
	if (isAlreadyPlanted) return;
	
	Super::InteractObject();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("print plant01"));
	if (PlantingSystemClass && LocationSpawnObject)
	{
		FVector SpawnLocation = LocationSpawnObject->GetActorLocation();
		FRotator SpawnRotation = LocationSpawnObject->GetActorRotation();

		PlantingSystem = GetWorld()->SpawnActor<APlantingSystem>(PlantingSystemClass, SpawnLocation, SpawnRotation);
	}

	isAlreadyPlanted = true;
}

void APlant01::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("trigger plant01"));
	
	if (isAlreadyPlanted)
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

void APlant01::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);
	
	if (ActualEvolveMesh)
	{
		ActualEvolveMesh->SetOverlayMaterial(nullptr);
	}
}
