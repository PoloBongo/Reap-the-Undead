#include "Interactable/Plant/Plants.h"

#include "Blueprint/UserWidget.h"
#include "Planting/PlantingSystem.h"

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
		CanDeclenchAnim = true;
		GetWorldTimerManager().SetTimer(PlantingAnimTimerHandle, this, &APlants::ResetAnimation, DelayResetAnimation, false, DelayResetAnimation);

		if (PlantingSystemClass && TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetComponentLocation();
			FRotator SpawnRotation = TargetPoint->GetComponentRotation();

			PlantingSystem = GetWorld()->SpawnActor<APlantingSystem>(PlantingSystemClass, SpawnLocation, SpawnRotation);

			PlantingSystem->SetAssociatePlant(this);
		}

		IsAlreadyPlanted = true;
		if(UIUserInteract && IsAlreadyPlanted) UIUserInteract->SetVisibility(ESlateVisibility::Hidden);	
	}
}

void APlants::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	
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

void APlants::ResetAnimation()
{
	CanDeclenchAnim = false;
}

void APlants::ChangeText(const FString& NewText)
{
	UIUserInteract->SetVisibility(ESlateVisibility::Visible);
	OnTextChanged.Broadcast(NewText);
}

void APlants::SetCanHarvest(bool Harvest)
{
	CanHarvest = Harvest;
}


void APlants::Destroyed()
{
	if (ActualEvolveMesh)
	{
		ActualEvolveMesh->DestroyComponent();
		ActualEvolveMesh = nullptr;
	}
	Destroy();
}
