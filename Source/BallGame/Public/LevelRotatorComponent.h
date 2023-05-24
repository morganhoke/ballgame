// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "BallGame/Message/BallGameWorldRotationMessage.h"

#include "LevelRotatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLGAME_API ULevelRotatorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelRotatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type) override;

	void OnWorldRotationMessage(FGameplayTag Channel, const FBallGameWorldRotationMessage& Message);

	FGameplayMessageListenerHandle ListenerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// If set, this rotates around its own axis
	UPROPERTY(EditAnywhere)
	bool IsRootRotator = false;

	// If not root rotator, rotate about this
	UPROPERTY(EditAnywhere)
	AActor* RootRotator = nullptr;
};
