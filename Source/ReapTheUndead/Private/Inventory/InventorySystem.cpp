#include "Inventory/InventorySystem.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Inventory/Item.h"
#include "Inventory/DataAsset/InventoryDataItems.h"

AInventorySystem::AInventorySystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryWidget) return;
	InventoryWidget->AddToViewport();

	LoadInventory();
}

void AInventorySystem::LoadInventory()
{
	if (!InventoryWrapBox) return;
	for (UClass* Classe : Classes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vérification de la classe: %s"), *Classe->GetName());

		for (UInventoryDataItems* Data : DataAssets)
		{
			UE_LOG(LogTemp, Warning, TEXT("Vérification de l'item dans Data: %s"), *Data->ItemClass->GetName());

			if (Classe->GetFName() == Data->ItemClass->GetFName())
			{
				UImage* ItemImage = NewObject<UImage>(this);

				if (UTexture* BaseTexture = Data->Image)
				{
					UTexture2D* ItemTexture2D = Cast<UTexture2D>(BaseTexture);
					if (ItemTexture2D)
					{
						FSlateBrush ImageBrush;
						ImageBrush.SetResourceObject(ItemTexture2D);
						ItemImage->SetBrush(ImageBrush);
						FVector2D ImageSize(100.f, 400.f);
						InventoryWrapBox->AddChildToWrapBox(ItemImage);
						ItemImage->SetBrushSize(ImageSize);

						ItemImage->OnMouseButtonDownEvent.BindUFunction(this, FName("OnItemClicked"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Le cast vers UTexture2D a échoué pour: %s"), *Data->ItemClass->GetName());
					}
				}
			}
		}
	}
}


void AInventorySystem::OnItemClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("image cliqué! %s"), *this->GetName());
}

void AInventorySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInventorySystem::InteractInventory()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!InventoryWidget && !InventoryBorder && !PlayerController && !ImageBtnCloseInventory) return;

	if (IsOpen)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		InventoryBorder->SetVisibility(ESlateVisibility::Visible);
		ImageBtnCloseInventory->SetVisibility(ESlateVisibility::Visible);
	}
	PlayerController->SetShowMouseCursor(!IsOpen);
	IsOpen = !IsOpen;
}

void AInventorySystem::CloseInventory()
{
	InteractInventory();
}

EItemType AInventorySystem::GetItemType(const EItemType ItemType)
{
	return ItemType;
}

void AInventorySystem::AddObjectInInventory(UClass* Object)
{
	Classes.Add(Object);
}

void AInventorySystem::UpdateInventorySlotImage()
{
	UButton* Button = ButtonsSlots[0];
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
				AItem* SpawnedItem;

				if (InstanciatedItems.Contains(ItemClass))
				{
					SpawnedItem = InstanciatedItems[ItemClass];
				}
				else
				{
					FVector SpawnLocation(0.f, 0.f, 0.f);
					FRotator SpawnRotation(0.f, 0.f, 0.f);
                    
					SpawnedItem = GetWorld()->SpawnActor<AItem>(ItemClass, SpawnLocation, SpawnRotation);
					if (SpawnedItem)
					{
						InstanciatedItems.Add(ItemClass, SpawnedItem);
					}
				}

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
			UE_LOG(LogTemp, Warning, TEXT("La référence n'est pas un UClass."));
		}
	}
}

