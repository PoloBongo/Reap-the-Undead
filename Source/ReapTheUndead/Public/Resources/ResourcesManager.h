#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "ResourcesManager.generated.h"

class UInventoryDataItems;
class AGameNotificationManager;
class AInventorySystem;
class ALumberJack;
class UNiagaraSystem;

UCLASS()
class REAPTHEUNDEAD_API AResourcesManager : public AInteractableObjects
{
	GENERATED_BODY()

public:
	AResourcesManager();
	
	UFUNCTION(BlueprintCallable, category = "Events")
	void OnPlayAnimationTimber();

	UFUNCTION(BlueprintCallable, category = "Events")
	void OnPlayAnimationMiner();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Yield;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FName ResourceType;

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void TakeDamage(int32 DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void Harvest();
	
	virtual void InteractObject() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	UNiagaraSystem* DebrisEffect;

	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	ALumberJack* AxeActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	AInventorySystem* InventorySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	AGameNotificationManager* GameNotificationManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	UInventoryDataItems* InventoryDataItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	bool CanPlayAnimationTimber = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
	bool CanPlayAnimationMiner = false;
};
