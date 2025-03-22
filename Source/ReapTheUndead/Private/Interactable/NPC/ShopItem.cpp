#include "Interactable/NPC/ShopItem.h"

#include "Blueprint/UserWidget.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

void AShopItem::BeginPlay()
{
	Super::BeginPlay();
}

void AShopItem::InteractObject()
{
	Super::InteractObject();
	APlayerController* GetPlayerController = GetWorld()->GetFirstPlayerController();
	
	if (!UIInteractableNPC || !PlayerController) return;
	
	IsOpen = !IsOpen;

	if (IsOpen)
	{
		GetPlayerController->SetInputMode(FInputModeGameAndUI());
		GetPlayerController->SetIgnoreMoveInput(true);
		UIInteractableNPC->AddToViewport();
		GetPlayerController->SetShowMouseCursor(true);
	}
	else
	{
		GetPlayerController->SetInputMode(FInputModeGameOnly());
		GetPlayerController->SetIgnoreMoveInput(false);
		UIInteractableNPC->RemoveFromParent();
		GetPlayerController->SetShowMouseCursor(false);
	}
}

void AShopItem::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerController != OtherActor) return;

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
	}
}

void AShopItem::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
	}
}