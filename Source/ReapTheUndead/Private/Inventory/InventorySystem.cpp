#include "Inventory/InventorySystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Inventory/Item.h"
#include "Inventory/DataAsset/InventoryDataItems.h"

AInventorySystem::AInventorySystem(): ID(0), Quantity(0), Image(nullptr), InventoryWidget(nullptr),
                                      InventoryWrapBox(nullptr),
                                      ImageBtnCloseInventory(nullptr),
                                      DefaultSlotImage(nullptr),
                                      InventoryBorder(nullptr)
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

void AInventorySystem::OnButtonDoubleClicked(int32 ButtonIndex)
{
	UInventoryDataItems* SaveActualAssetData = nullptr;
	static float LastClickTime = 0.0f;
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	UButton* Button = ButtonsSlots[ButtonIndex];
	FButtonStyle ButtonStyle = Button->GetStyle();

	if (CurrentTime - LastClickTime < 0.3f)
	{
		if (IsFirstDoubleClick)
		{
			IsFirstDoubleClick = false;
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Double clic"));
		if (InventorySlots.Find(ButtonIndex))
		{
			if (UClass* Found = FoundClassInSlot(ButtonIndex))
			{
				for (UInventoryDataItems* DataAsset : DataAssets)
				{
					SaveActualAssetData = DataAsset;
					if (DataAsset->ItemClass->GetFName() == *Found->GetFName().ToString())
					{
						break;
					}
				}
				
				if (DefaultSlotImage && Button)
				{
					ButtonStyle.Normal.SetResourceObject(DefaultSlotImage);
					ButtonStyle.Hovered.SetResourceObject(DefaultSlotImage);
				}

				Button->SetStyle(ButtonStyle);
				InventorySlots.Remove(ButtonIndex);
				SaveActualAssetData->Quantity++;

				LoadInventory();
			}
		}
	}

	LastClickTime = CurrentTime;
}

void AInventorySystem::LoadInventory()
{
	if (!InventoryWrapBox) return;

	InventoryWrapBox->ClearChildren();
	
	for (UInventoryDataItems* Data : DataAssets)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vérification de l'item dans Data: %s"), *Data->ItemClass->GetName());

		if (Data->Quantity > 0)
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
					ItemImage->SetDesiredSizeOverride(ImageSize);
					ItemImage->OnMouseButtonDownEvent.BindUFunction(this, FName("OnItemClicked"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Le cast UTexture failed pour: %s"), *Data->ItemClass->GetName());
				}
			}
		}
	}
}

void AInventorySystem::AddItem(UInventoryDataItems* ItemData, int Amount)
{
	if (DataAssets.Find(ItemData))
	{
		ItemData->Quantity += Amount;

		LoadInventory();
	}
}

void AInventorySystem::RemoveItem(UInventoryDataItems* ItemData, int Amount)
{
	if (DataAssets.Find(ItemData))
	{
		ItemData->Quantity -= Amount;

		LoadInventory();
	}
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
		IsFirstDoubleClick = true;
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

void AInventorySystem::UseSlots(int Index)
{
	if (!InventoryWidget) return;
	
	if (UClass* Found = FoundClassInSlot(Index))
	{
		AItem* SpawnedItem;

		if (InstanciatedItems.Contains(Found->GetClass()))
		{
			SpawnedItem = InstanciatedItems[Found->GetClass()];
		}
		else
		{
			FVector SpawnLocation(0.f, 0.f, 0.f);
			FRotator SpawnRotation(0.f, 0.f, 0.f);
                    
			SpawnedItem = GetWorld()->SpawnActor<AItem>(Found->GetClass(), SpawnLocation, SpawnRotation);
			if (SpawnedItem)
			{
				InstanciatedItems.Add(Found->GetClass(), SpawnedItem);
			}
		}

		if (SpawnedItem)
		{
			SpawnedItem->UseItem();
		}
	}
}

UClass* AInventorySystem::FoundClassInSlot(int32 Index)
{
	if (UClass** ItemClass = InventorySlots.Find(Index))
	{
		return *ItemClass;
	}

	return nullptr;
}
