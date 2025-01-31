#include "Interactable/InteractableObjects.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractableObjects::AInteractableObjects()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Objectmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Objectmesh;

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

void AInteractableObjects::InteractObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("print"));
}


void AInteractableObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObjects::InteractFunction()
{
	InteractObject();
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
	
}

