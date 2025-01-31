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

void AInteractableObjects::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}


void AInteractableObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObjects::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("1111111111"));
	if (PlayerController != OtherActor) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("278575785"));
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
