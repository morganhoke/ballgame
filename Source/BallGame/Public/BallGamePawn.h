// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "BallGame/Message/BallGameWorldRotationMessage.h"
#include "BallGamePawn.generated.h"


UCLASS()
class BALLGAME_API ABallGamePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallGamePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RecieveMoveAxisAction(const FInputActionInstance& Instance);
	void RecieveCameraAxisAction(const FInputActionInstance& Instance);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Collision")
	void OnBallPawnHit(UPrimitiveComponent* component, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PlayerMesh;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputAction> MoveAxisAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputAction> CameraAxisAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float GroundRotationSpeed = 1;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float HighestAngle = -75;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float LowestAngle = 5;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationSpeed = 1;

	FQuat WorldRotation;
};
