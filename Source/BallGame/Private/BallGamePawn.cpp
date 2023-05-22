// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGamePawn.h"

// Sets default values
ABallGamePawn::ABallGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	RootComponent = PlayerMesh;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(PlayerMesh);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABallGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABallGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	ULocalPlayer* Player = Cast<ULocalPlayer>(GetController()->GetNetOwningPlayer());
	if (Player)
	{
		UEnhancedInputLocalPlayerSubsystem* inputSubsystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (inputSubsystem && !InputMapping.IsNull())
		{
			inputSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}

	Input->BindAction(MoveAxisAction.Get(), ETriggerEvent::Triggered, this, &ABallGamePawn::RecieveMoveAxisAction);
	Input->BindAction(CameraAxisAction.Get(), ETriggerEvent::Triggered, this, &ABallGamePawn::RecieveCameraAxisAction);


}

void ABallGamePawn::RecieveMoveAxisAction(const FInputActionInstance& Instance)
{
	
}

void ABallGamePawn::RecieveCameraAxisAction(const FInputActionInstance& Instance)
{
	FVector2D cameraInputVector = Instance.GetValue().Get<FVector2D>();
	
	FRotator SpringArmRot = SpringArmComponent->GetRelativeRotation();
	
	float proposedPitch = SpringArmRot.Pitch + ( - 1 * (cameraInputVector.Y * RotationSpeed));
	float proposedYaw = SpringArmRot.Yaw + (-1 * (cameraInputVector.X * RotationSpeed));

	if (proposedPitch >= HighestAngle && proposedPitch <= LowestAngle)
	{
		SpringArmRot.Pitch = proposedPitch;
	}

	SpringArmRot.Yaw = proposedYaw;

	SpringArmRot.Normalize();
	SpringArmRot.Roll = 0;

	SpringArmComponent->SetRelativeRotation(SpringArmRot);
	
}
