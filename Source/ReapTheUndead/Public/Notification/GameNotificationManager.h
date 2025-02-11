#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameNotificationManager.generated.h"

UCLASS()
class REAPTHEUNDEAD_API AGameNotificationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameNotificationManager();

	void SetTextNotification(FString NewNotification, FColor Color);
	FString GetTextNotification();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	FString TextNotification;
	
	FTimerHandle NotificationHandle;
	FTimerManager NotificationManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* NotificationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	float NotificationDuration;

	void HiddeNotification() const;
};
