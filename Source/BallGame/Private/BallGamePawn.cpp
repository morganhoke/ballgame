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
	WorldRotation = FQuat(1, 0, 0, 0);
	//PlayerMesh->OnComponentHit.AddDynamic(this, &ABallGamePawn::OnBallPawnHit);
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

void ABallGamePawn::OnBallPawnHit(UPrimitiveComponent* component, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& result)
{
	DrawDebugDirectionalArrow(GetWorld(), result.ImpactPoint, (result.ImpactPoint + result.ImpactNormal.GetSafeNormal() * 5), 3, FColor::Red, true);
	DrawDebugDirectionalArrow(GetWorld(), result.ImpactPoint, (result.ImpactPoint + result.ImpactNormal.GetSafeNormal() * -420) , 3, FColor::Green, true);
	PlayerMesh->AddImpulse(result.ImpactNormal.GetSafeNormal() * -420);
}

void ABallGamePawn::RecieveMoveAxisAction(const FInputActionInstance& Instance)
{
	FVector2D motionInputVector = Instance.GetValue().Get<FVector2D>();
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FBallGameWorldRotationMessage Message;

	// Get camera forward vector on XY

	FVector CameraRelativeActual = RootComponent->GetComponentLocation() - Camera->GetComponentLocation();

	FVector CameraFacing(CameraRelativeActual.XYZ[0], CameraRelativeActual.XYZ[1], 0);
	CameraFacing.Normalize();
	FVector CameraRight = FVector::CrossProduct(FVector::UpVector, CameraFacing);
	CameraRight.Normalize();

	Message.TargetChannel = TAG_BallGame_WorldRotation_Message;
	Message.CameraForward = CameraFacing;
	Message.CameraRight = CameraRight;
	Message.CameraRelativePitch = motionInputVector.Y * GroundRotationSpeed;
	Message.CameraRelativeRoll = motionInputVector.X * GroundRotationSpeed;

	FQuat rollQuat(Message.CameraForward, -1 * FMath::DegreesToRadians(Message.CameraRelativeRoll));
	FQuat pitchQuat(Message.CameraRight, FMath::DegreesToRadians(Message.CameraRelativePitch));

	WorldRotation = rollQuat * pitchQuat * WorldRotation;

	UE_LOG(LogCore, Display, TEXT("Rotation %f, %f, %f"), WorldRotation.Rotator().Pitch, WorldRotation.Rotator().Roll, WorldRotation.Rotator().Yaw);

	DrawDebugDirectionalArrow(GetWorld(), PlayerMesh->GetComponentLocation(), PlayerMesh->GetComponentLocation() + (WorldRotation.RotateVector(FVector::UpVector) * 100 * -1 ) , 5, FColor::Blue, true);

	MessageSubsystem.BroadcastMessage<FBallGameWorldRotationMessage>(TAG_BallGame_WorldRotation_Message, Message);
}

void ABallGamePawn::RecieveCameraAxisAction(const FInputActionInstance& Instance)
{
	FVector2D cameraInputVector = Instance.GetValue().Get<FVector2D>();
	
	FRotator SpringArmRot = SpringArmComponent->GetRelativeRotation();
	
	float proposedPitch = SpringArmRot.Pitch + ( - 1 * (cameraInputVector.Y * CameraRotationSpeed));
	float proposedYaw = SpringArmRot.Yaw + (-1 * (cameraInputVector.X * CameraRotationSpeed));

	if (proposedPitch >= HighestAngle && proposedPitch <= LowestAngle)
	{
		SpringArmRot.Pitch = proposedPitch;
	}

	SpringArmRot.Yaw = proposedYaw;

	SpringArmRot.Normalize();
	SpringArmRot.Roll = 0;

	SpringArmComponent->SetRelativeRotation(SpringArmRot);
	
}
