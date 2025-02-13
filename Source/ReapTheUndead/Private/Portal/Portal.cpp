#include "Portal/Portal.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;
	
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(BoxComponent);
	
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnBeginOverlap);
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void APortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerController != OtherActor) return;

	SwitchLevel(TargetLevelName);
}

void APortal::SwitchLevel(FName LevelName) const
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true);
}