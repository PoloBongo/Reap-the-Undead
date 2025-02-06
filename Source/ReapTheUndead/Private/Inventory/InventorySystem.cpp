#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Inventory/Item.h"

AInventorySystem::AInventorySystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryWidget) return;
	InventoryWidget->AddToViewport();
}	

void AInventorySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInventorySystem::InteractInventory()
{
	if (!InventoryWidget && !InventoryBorder) return;
	IsOpen ? InventoryBorder->SetVisibility(ESlateVisibility::Hidden) : InventoryBorder->SetVisibility(ESlateVisibility::Visible);
	IsOpen = !IsOpen;
}

EItemType AInventorySystem::GetItemType(const EItemType ItemType)
{
	return ItemType;
}

void AInventorySystem::AddObjectInInventory(UObject* Object)
{
	Objects.Add(Object);
}

void AInventorySystem::UpdateInventorySlotImage()
{
	// UButton* Button = ButtonsSlots[0];
	// if (!Button) return;
	//
	// FButtonStyle ButtonStyle = Button->WidgetStyle;
	//
	// ButtonStyle.Normal.SetResourceObject();
	//
	// Button->SetStyle(ButtonStyle);
}

void AInventorySystem::UseSlots(int Index)
{
	if (!InventoryWidget) return;

	if (UClass** ItemPtr = InventorySlots.Find(Index))
	{
	    UClass* Item = *ItemPtr;

	    if (UClass* ItemClass = Cast<UClass>(Item))
	    {
	        if (ItemClass->IsChildOf(AItem::StaticClass()))
	        {
	            FVector SpawnLocation(0.f, 0.f, 0.f); 
	            FRotator SpawnRotation(0.f, 0.f, 0.f);

	            AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(ItemClass, SpawnLocation, SpawnRotation);
	            if (SpawnedItem)
	            {
	                SpawnedItem->UseItem();
	            }
	        }
	        else
	        {
	            UE_LOG(LogTemp, Warning, TEXT("L'objet du slot n'est pas valide"));
	        }
	    }
	    else
	    {
	        UE_LOG(LogTemp, Warning, TEXT("la ref n'est pas un UClass."));
	    }
	}

}
