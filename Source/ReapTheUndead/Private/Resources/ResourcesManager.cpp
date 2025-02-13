#include "Resources/ResourcesManager.h"

#include "Chaos/SpatialAccelerationCollection.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
		Destroy();
	}
}

void AResourcesManager::Harvest()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, std::to_string(Yield).data());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ResourceType.ToString());
}

void AResourcesManager::InteractObject()
{
	TakeDamage(10);
}

void AResourcesManager::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
	}
}

void AResourcesManager::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
	}
}