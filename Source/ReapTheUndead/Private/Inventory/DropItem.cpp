#include "Inventory/DropItem.h"

#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "Components/WrapBox.h"
#include "Inventory/InventorySystem.h"
#include "Inventory/SlotButtonInventory.h"
#include "Inventory/DataAsset/InventoryDataItems.h"
#include "TimerManager.h"
#include "Components/Border.h"
#include "Notification/GameNotificationManager.h"
#include "Player/DetectionDrop.h"
#include "ReapTheUndead/ReapTheUndeadCharacter.h"

ADropItem::ADropItem()
{
	Objectmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bag Mesh"));
	RootComponent = Objectmesh;

	BoxComponent->SetupAttachment(Objectmesh);
	TargetPoint->SetupAttachment(Objectmesh);

	const FVector NewSizeBox(100.f, 100.f, 50.f);
	BoxComponent->SetBoxExtent(NewSizeBox);
}

void ADropItem::BeginPlay()
{
	Super::BeginPlay();
}

void ADropItem::InteractObject()
{
	Super::InteractObject();
	
	InteractDropInventory();
}

void ADropItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(this);
	}
}

void ADropItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, AnyOtherComponent, OtherBodyIndex);
	
	if (AReapTheUndeadCharacter* Player = Cast<AReapTheUndeadCharacter>(OtherActor))
	{
		Player->SetInteractableObject(nullptr);
	}
}

void ADropItem::InteractDropInventory()
{
	APlayerController* PlayerControllerVar = GetWorld()->GetFirstPlayerController();
	if (!WidgetStashDrop && !PlayerControllerVar && !HoveredWidget) return;

	if (IsOpen)
	{
		CloseDropInventory(PlayerControllerVar);
	}
	else
	{
		PlayerControllerVar->SetInputMode(FInputModeGameAndUI());
		PlayerControllerVar->SetIgnoreMoveInput(true);
		WidgetStashDrop->AddToViewport();
		HoveredWidget->AddToViewport();

		if (UFunction* GetActualBPDropFunction = WidgetStashDrop->FindFunction(TEXT("GetActualBPDrop")))
		{
			WidgetStashDrop->ProcessEvent(GetActualBPDropFunction, nullptr);
		}
		LoadDropInventory();

		PlayerControllerVar->SetShowMouseCursor(!IsOpen);
		IsOpen = !IsOpen;
	}
}

void ADropItem::CloseDropInventory(APlayerController* PlayerControllerVar, const bool Close)
{
	PlayerControllerVar->SetInputMode(FInputModeGameOnly());
	PlayerControllerVar->SetIgnoreMoveInput(false);
	WidgetStashDrop->RemoveFromParent();
	HoveredWidget->RemoveFromParent();
	
	PlayerControllerVar->SetShowMouseCursor(Close);
	IsOpen = !IsOpen;
}

void ADropItem::OnButtonDoubleClicked(int ButtonIndex)
{
	static float LastClickTimeDrop = -1.0f;
	const float CurrentTimeDrop = GetWorld()->GetTimeSeconds();

	if (LastClickTimeDrop >= 0.0f && (CurrentTimeDrop - LastClickTimeDrop < 0.4f))
	{
		if (DropListItem.Find(ButtonIndex))
		{
			UInventoryDataItems* SaveActualAssetData = nullptr;
			for (UInventoryDataItems* DataAsset : InventorySystem->GetDataAssets())
			{
				if (DataAsset->ItemClass == DropListItem[ButtonIndex]->ItemClass)
				{
					SaveActualAssetData = DataAsset;
					break;
				}
			}
			
			if (DropQuantityInstance.Contains(SaveActualAssetData))
			{
				DropQuantityInstance[SaveActualAssetData]--;
			}
			
			if (SaveActualAssetData)
			{
				if (DropQuantityInstance[SaveActualAssetData] < 1)
				{
					TotalUsedDropSlots--;
					DropListItem.Remove(ButtonIndex);
				}
				SaveActualAssetData->Quantity++;
			}
			else
			{
				InventorySystem->AddItem(DropListItem[ButtonIndex], 1);

				if (DropQuantityInstance.Contains(SaveActualAssetData))
				{
					if (DropQuantityInstance[SaveActualAssetData] <= 1)
					{
						DropListItem.Remove(ButtonIndex);
					}
				}
			}

			TMap<int, UInventoryDataItems*> NewDropList;
			int NewIndex = 0;
			for (auto& Elem : DropListItem)
			{
				NewDropList.Add(NewIndex++, Elem.Value);
			}
			DropListItem = NewDropList;
			InventorySystem->SaveInventoryToFile();
			InventorySystem->LoadInventoryFromFile();
			LoadDropInventory();
			CheckDestroyingDrop();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("drop list item not found"));
		}
	}
	LastClickTimeDrop = CurrentTimeDrop;
}

void ADropItem::LoadDropInventory()
{
	if (!InventoryDropWrapBox) return;

	InventoryDropWrapBox->ClearChildren();
	ImagesButtonsDropInventory.Reset();

	int Index = 0;
	TotalUsedDropSlots = 0;

	for (const TPair<int, UInventoryDataItems*>& Elem : DropListItem)
	{
		UInventoryDataItems* Value = Elem.Value;

		if (DropQuantityInstance[Value] > 0)
		{
			USlotButtonInventory* NewButton = NewObject<USlotButtonInventory>(this, ImagesButtonsDropInventoryClass);

			if (NewButton)
			{
				FButtonStyle ButtonStyle = NewButton->GetStyle();

				if (UTexture* BaseTexture = Value->Image)
				{
					UTexture2D* ItemTexture2D = Cast<UTexture2D>(BaseTexture);
					if (ItemTexture2D)
					{
						ButtonStyle.Normal.SetResourceObject(ItemTexture2D);
						ButtonStyle.Hovered.SetResourceObject(ItemTexture2D);
						ButtonStyle.Pressed.SetResourceObject(ItemTexture2D);

						FVector2D ImageSize(100.f, 400.f);
						ButtonStyle.Normal.SetImageSize(ImageSize);
						ButtonStyle.Hovered.SetImageSize(ImageSize);
						ButtonStyle.Pressed.SetImageSize(ImageSize);

						NewButton->SetStyle(ButtonStyle);
						InventoryDropWrapBox->AddChildToWrapBox(NewButton);
						ImagesButtonsDropInventory.Add(NewButton);
					}
				}
			}
			Index++;
		}
	}

	if (UFunction* BindingAllItemsFunction = WidgetStashDrop->FindFunction(TEXT("BindingAllItems")))
	{
		WidgetStashDrop->ProcessEvent(BindingAllItemsFunction, nullptr);
	}

	AttachHoveredEvent();
	AttachUnHoveredEvent();
}

void ADropItem::TriggerButtonClickedDelegate(int ButtonIDVar)
{
	OnButtonDoubleClicked(ButtonIDVar);
}

void ADropItem::AddItemToDrop(UInventoryDataItems* InventoryDataItemsVar)
{
	if (TotalUsedDropSlots < MaxSlots)
	{
		bool FoundDoubleItem = false;
		if (DropListItem.Num() > 0)
		{
			for (const TPair<int, UInventoryDataItems*>& Elem : DropListItem)
			{
				UInventoryDataItems* Value = Elem.Value;
			
				if (Value->ID == InventoryDataItemsVar->ID)
				{
					FoundDoubleItem = true;
					break;
				}
			}
		}
		if (!FoundDoubleItem)
		{
			DropListItem.Add(TotalUsedDropSlots, InventoryDataItemsVar);
			TotalUsedDropSlots++;
			CountDropSlot = FString::Printf(TEXT("%d/%d"), TotalUsedDropSlots, MaxSlots);
		}
		
		if (DropQuantityInstance.Contains(InventoryDataItemsVar))
		{
			DropQuantityInstance[InventoryDataItemsVar]++;
		}
		else
		{
			DropQuantityInstance.Add(InventoryDataItemsVar, 1);
		}
		
		InventorySystem->SaveInventoryToFile();
		UE_LOG(LogTemp, Warning, TEXT("instance drop item name : %s"), *InventoryDataItemsVar->GetName());
	}
	else
	{
		if (!GameNotificationManager) return;
		GameNotificationManager->SetTextNotification(TEXT("Vous ne pouvez plus déposer d'item dans ce sac!"), FColor::Red);
	}
}

void ADropItem::CheckDestroyingDrop()
{
	if (DropListItem.Num() <= 0)
	{
		Destroyed();
	}
}

void ADropItem::Destroyed()
{
	Super::Destroyed();

	if (APlayerController* PlayerControllerVar = GetWorld()->GetFirstPlayerController()) CloseDropInventory(PlayerControllerVar);
	
	if (Objectmesh) Objectmesh->DestroyComponent();
	if (BoxComponent) BoxComponent->DestroyComponent();
	if (TargetPoint) TargetPoint->DestroyComponent();
	if (InventorySystem) InventorySystem->RemoveDropInstance(this);

	if (ReapTheUndeadCharacter)
	{
		UDetectionDrop* DetectionDrop = ReapTheUndeadCharacter->GetComponentByClass<UDetectionDrop>();

		if (DetectionDrop)
		{
			DetectionDrop->SetDropItem(nullptr);
		}
	}
	
	Destroy();
}

void ADropItem::StopAnimation()
{
	IsDrop = false;
}

UInventoryDataItems* ADropItem::GetItemDropAt(UInventoryDataItems* InventoryDataItems)
{
	for (const TPair<int, UInventoryDataItems*>& Elem : DropListItem)
	{
		UInventoryDataItems* Value = Elem.Value;

		if (Value == InventoryDataItems)
		{
				return Elem.Value;
		}
	}
	return nullptr;
}

void ADropItem::TriggerButtonHoveredDelegate(int ButtonID)
{
	if (!HoveredWidget) return;
	HoveredWidget->SetVisibility(ESlateVisibility::Visible);
	
	for (UInventoryDataItems* Item : InventorySystem->GetAllDataAssets())
	{
		if (Item->ItemClass == DropListItem[ButtonID]->ItemClass)
		{
			QuantityText = FString::Printf(TEXT("Nom : %s\nQuantity : %d"), *DropListItem[ButtonID]->Name, DropQuantityInstance[Item]);
			break;
		}
	}
}

void ADropItem::TriggerButtonUnHoveredDelegate()
{
	if (!HoveredWidget) return;

	HoveredWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ADropItem::AttachHoveredEvent() const
{
	if (UFunction* BindingAllItemsHoveredFunction = WidgetStashDrop->FindFunction(TEXT("BindingAllItemsHovered")))
	{
		WidgetStashDrop->ProcessEvent(BindingAllItemsHoveredFunction, nullptr);
	}
}

void ADropItem::AttachUnHoveredEvent() const
{
	if (UFunction* BindingAllItemsUnHoveredFunction = WidgetStashDrop->FindFunction(TEXT("BindingAllItemsUnHovered")))
	{
		WidgetStashDrop->ProcessEvent(BindingAllItemsUnHoveredFunction, nullptr);
	}
}
