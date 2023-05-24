// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelRotatorComponent.h"

// Sets default values for this component's properties
ULevelRotatorComponent::ULevelRotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULevelRotatorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsRootRotator)
	{
		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
		ListenerHandle = MessageSubsystem.RegisterListener(TAG_BallGame_WorldRotation_Message, this, &ULevelRotatorComponent::OnWorldRotationMessage);
	}
	else
	{
		if (RootRotator)
		{
			UE_LOG(LogGameplayTags, Display, TEXT("Begin Play on plane"));
			this->GetAttachParentActor()->AttachToActor(RootRotator, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		}
	}
}

void ULevelRotatorComponent::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	if (IsRootRotator)
	{
		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
		MessageSubsystem.UnregisterListener(ListenerHandle);
	}
	else
	{
		this->GetAttachParentActor()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));
	}
}

void ULevelRotatorComponent::OnWorldRotationMessage(FGameplayTag Channel, const FBallGameWorldRotationMessage& Message)
{
	//UE_LOG(LogGameplayTags, Display, TEXT("In On World Rotation Message"));
	if (IsRootRotator)
	{
		//UE_LOG(LogGameplayTags, Display, TEXT("I'm a root rotator, rotation %f, %f, %f"), Message.Rotation.Pitch, Message.Rotation.Yaw, Message.Rotation.Roll);
		FRotator rotation = this->GetAttachParentActor()->GetActorRotation();
		FQuat rollQuat(Message.CameraForward, -1 * FMath::DegreesToRadians(Message.CameraRelativeRoll));
		FQuat pitchQuat(Message.CameraRight,  FMath::DegreesToRadians(Message.CameraRelativePitch));
		this->GetAttachParentActor()->AddActorWorldRotation(rollQuat * pitchQuat);
	}
}


// Called every frame
void ULevelRotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

