#include "Inventory/DropItem.h"

#include "Components/WidgetComponent.h"

ADropItem::ADropItem(): BagMesh(nullptr)
{
	BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bag Mesh"));
	BagMesh->SetupAttachment(RootComponent);

	WidgetStashDropComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Stash"));
	WidgetStashDropComponent->SetupAttachment(BagMesh);
}

void ADropItem::InteractObject()
{
	Super::InteractObject();

	if (WidgetStashDropClass)
	{
		WidgetStashDrop = CreateWidget<UUserWidget>(GetWorld(), WidgetStashDropClass);

		if (WidgetStashDrop)
		{
			WidgetStashDrop->AddToViewport();
		}
	}
}
