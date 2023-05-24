// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Math/UnrealMathUtility.h"
#include "BallGameWorldRotationMessage.generated.h"


BALLGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_BallGame_WorldRotation_Message);

USTRUCT(BlueprintType)
struct BALLGAME_API FBallGameWorldRotationMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = Notification)
    FGameplayTag TargetChannel;

    UPROPERTY(BlueprintReadWrite, Category = Notification)
    FVector CameraForward;

    // Rotation around forward vector
    UPROPERTY(BlueprintReadWrite, Category = Notiication)
    double CameraRelativeRoll;

    UPROPERTY(BlueprintReadWrite, Category = Notification)
    FVector CameraRight;

    // Rotation around right vector
    UPROPERTY(BlueprintReadWrite, Category = Notification)
    double CameraRelativePitch;


};