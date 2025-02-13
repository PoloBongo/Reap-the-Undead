#include "Interactable/InteractableObjects.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractableObjects::AInteractableObjects()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Objectmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Objectmesh;

	TargetPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Plant"));
	TargetPoint->SetupAttachment(Objectmesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxComponent->SetupAttachment(Objectmesh);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObjects::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableObjects::OnEndOverlap);
}

void AInteractableObjects::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
}

void AInteractableObjects::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AInteractableObjects::InteractObject() {}

void AInteractableObjects::InteractFunction()
{
	if (isPlayerTrigger)
	{
		InteractObject();
	}
}

void AInteractableObjects::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (PlayerController != OtherActor) return;
	isPlayerTrigger = true;

	Objectmesh->SetOverlayMaterial(MaterialOverlay);
}

void AInteractableObjects::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex)
{
	isPlayerTrigger = false;

	Objectmesh->SetOverlayMaterial(nullptr);
}

