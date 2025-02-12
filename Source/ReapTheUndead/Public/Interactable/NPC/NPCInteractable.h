// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCInteractable.generated.h"

class UBoxComponent;

UCLASS()
class REAPTHEUNDEAD_API ANPCInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	ANPCInteractable();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* AnyOtherComponent, int32 OtherBodyIndex);

};
