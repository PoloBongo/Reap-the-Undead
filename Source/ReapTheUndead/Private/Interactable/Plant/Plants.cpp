#include "Interactable/Plant/Plants.h"

#include "Blueprint/UserWidget.h"
#include "Planting/PlantingSystem.h"
#include "Engine/TargetPoint.h"

void APlants::InteractObject()
{
	if (IsAlreadyPlanted) return;
	
	Super::InteractObject();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("print plant01"));
	if (PlantingSystemClass && LocationSpawnObject)
	{
		FVector SpawnLocation = LocationSpawnObject->GetActorLocation();
		FRotator SpawnRotation = LocationSpawnObject->GetActorRotation();

		PlantingSystem = GetWorld()->SpawnActor<APlantingSystem>(PlantingSystemClass, SpawnLocation, SpawnRotation);
	}

	IsAlreadyPlanted = true;
	if(UIUserInteract && IsAlreadyPlanted) UIUserInteract->SetVisibility(ESlateVisibility::Hidden);
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
