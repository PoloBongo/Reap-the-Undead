#include "Resources/ResourcesManager.h"

#include "Chaos/SpatialAccelerationCollection.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interactable/Resources/LumberJack.h"
#include "Inventory/InventorySystem.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "Kismet/GameplayStatics.h"
#include "Notification/GameNotificationManager.h"

AResourcesManager::AResourcesManager()
{
	Health = 100;
	Yield = 5;
	ResourceType = "Default";
}

void AResourcesManager::TakeDamage(int32 DamageAmount)
{
	Health -= DamageAmount;
	if (DebrisEffect)
	{
		FVector location(Objectmesh->GetComponentLocation().X,Objectmesh->GetComponentLocation().Y, Objectmesh->GetComponentLocation().Z + 150 );
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			DebrisEffect,
			Objectmesh,
			NAME_None,
			FVector(location),
			FRotator(0.f),
			EAttachLocation::Type::KeepWorldPosition,
			true);
	}
	if (Health <= 0)
	{
		Harvest();
		InventorySystem->AddItem(InventoryDataItems);
		InventorySystem->SaveInventoryToFile();
		InventorySystem->LoadInventoryFromFile();
		GameNotificationManager->SetTextNotification(FString::Printf(TEXT("Vous venez de récolter un %s"), *InventoryDataItems->Image->GetName()), FColor::Green);
		Destroy();
	}
}

void AResourcesManager::Harvest() {}

void AResourcesManager::InteractObject()
{
	if (AxeActor->ResourcesManager->ResourceType == "Wood")
	{
		OnPlayAnimationTimber();
	}
	if (AxeActor->ResourcesManager->ResourceType == "Rock")
	{
		OnPlayAnimationMiner();
	}
	TakeDamage(10);
}

void AResourcesManager::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
		if (AxeActor)
		{
			AxeActor->ResourcesManager = this;
		}
		if (AxeActor->ResourcesManager->ResourceType == "Wood")
		{
			CanPlayAnimationTimber = true;
		}
		if (AxeActor->ResourcesManager->ResourceType == "Rock")
		{
			CanPlayAnimationMiner = true;
		}
	}
}

void AResourcesManager::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
		if (AxeActor)
		{
			AxeActor->ResourcesManager = nullptr;
		}
		CanPlayAnimationTimber = false;
		CanPlayAnimationMiner = false;
	}
}

void AResourcesManager::OnPlayAnimationTimber()
{
}

void AResourcesManager::OnPlayAnimationMiner()
{
}

