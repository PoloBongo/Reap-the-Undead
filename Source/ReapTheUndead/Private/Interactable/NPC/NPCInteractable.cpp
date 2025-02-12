#include "Interactable/NPC/NPCInteractable.h"

#include "Components/BoxComponent.h"

ANPCInteractable::ANPCInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxComponent->SetupAttachment(RootComponent);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANPCInteractable::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ANPCInteractable::OnEndOverlap);
}

void ANPCInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPCInteractable::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TU VEUX HEIN"));

}

void ANPCInteractable::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DEGAGE BATARD"));

}

