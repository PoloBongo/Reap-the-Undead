#include "Interactable/NPC/InteractableNPC.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

void AInteractableNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableNPC::InteractObject()
{
	Super::InteractObject();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("sa dit quoi"));
	
	UIInteractableNPC->AddToViewport();
	UIInteractableNPC->SetVisibility(ESlateVisibility::Visible);

	if (UIInteractableNPC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("plein"));
	}
	

	// if(WrapBox) WrapBox->ClearChildren();
	//
	// for (int i = 0; i < DataAssets.Num(); i++)
	// {
	//   	UButton* NewButton = NewObject<UButton>(WrapBox);
	//   	WrapBox->AddChildToWrapBox(NewButton);
	// }
	
	IsOpen = !IsOpen;
}

void AInteractableNPC::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerController != OtherActor) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OUAIS MANUELO"));

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
	}
}

void AInteractableNPC::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DEGAGE BATARD"));

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
	}
}