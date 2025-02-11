#include "Notification/GameNotificationManager.h"

#include "Blueprint/UserWidget.h"

AGameNotificationManager::AGameNotificationManager(): NotificationWidget(nullptr), NotificationDuration(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameNotificationManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGameNotificationManager::SetTextNotification(FString NewNotification, FColor Color)
{
	TextNotification = NewNotification;
	if (!NotificationWidget) return;
	NotificationWidget->SetColorAndOpacity(Color);
	NotificationWidget->AddToViewport();
	if (!GetWorld()->GetTimerManager().IsTimerActive(NotificationHandle))
	{
		GetWorldTimerManager().SetTimer(NotificationHandle, this, &AGameNotificationManager::HiddeNotification, NotificationDuration, false);
	}
}

FString AGameNotificationManager::GetTextNotification()
{
	return TextNotification;
}

void AGameNotificationManager::HiddeNotification() const
{
	NotificationWidget->RemoveFromParent();
}
