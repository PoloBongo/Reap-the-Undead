// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReapTheUndeadCharacter.h"
#include "Interactable/InteractableObjects.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Inventory/InventorySystem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AReapTheUndeadCharacter

AReapTheUndeadCharacter::AReapTheUndeadCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AReapTheUndeadCharacter::SetInteractableObject(AInteractableObjects* _AInteractableObjects)
{
	InteractableObject = _AInteractableObjects;
}

void AReapTheUndeadCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	InstanceInteractableObject(SpawnParams);
	InstanceInventorySystem(SpawnParams);
}

void AReapTheUndeadCharacter::InstanceInteractableObject(FActorSpawnParameters SpawnParams)
{
	if (AInteractableObjects* InteractableObj = GetWorld()->SpawnActor<AInteractableObjects>(InteractableObjectClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, SpawnParams))
	{
		InteractableObject = InteractableObj;
	}
}

void AReapTheUndeadCharacter::InstanceInventorySystem(FActorSpawnParameters SpawnParams)
{
	if (AInventorySystem* Inv = GetWorld()->SpawnActor<AInventorySystem>(InventorySystemClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, SpawnParams))
	{
		InventorySystem = Inv;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AReapTheUndeadCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AReapTheUndeadCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AReapTheUndeadCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Interact);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::OpenInventory);
		EnhancedInputComponent->BindAction(SlotAction1, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Slot1);
		EnhancedInputComponent->BindAction(SlotAction2, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Slot2);
		EnhancedInputComponent->BindAction(SlotAction3, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Slot3);
		EnhancedInputComponent->BindAction(SlotAction4, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Slot4);
		EnhancedInputComponent->BindAction(SlotAction5, ETriggerEvent::Started, this, &AReapTheUndeadCharacter::Slot5);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AReapTheUndeadCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractableObject)
	{
		InteractableObject->InteractFunction();
	}
}

void AReapTheUndeadCharacter::OpenInventory(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->InteractInventory();
	}
}

void AReapTheUndeadCharacter::Slot1(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->UseSlots(0);
	}
}

void AReapTheUndeadCharacter::Slot2(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->UseSlots(1);
	}
}

void AReapTheUndeadCharacter::Slot3(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->UseSlots(2);
	}
}

void AReapTheUndeadCharacter::Slot4(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->UseSlots(3);
	}
}

void AReapTheUndeadCharacter::Slot5(const FInputActionValue& Value)
{
	if (InventorySystem)
	{
		InventorySystem->UseSlots(4);
	}
}

void AReapTheUndeadCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AReapTheUndeadCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}