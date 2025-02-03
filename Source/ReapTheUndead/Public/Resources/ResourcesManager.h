// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObjects.h"
#include "ResourcesManager.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AResourcesManager : public AInteractableObjects
{
	GENERATED_BODY()

public:
	AResourcesManager();


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

private:
	
};
