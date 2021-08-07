// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/CharacterMovementComponent.h"
#include "ExtCharacterMovementComponent.generated.h"
/**
 * 
 */

struct FRootMotionMovementParams;
/*CharacterMovementComponent Class just carries the Mirror Root Motion parameters and applies them when root motion is received from an animation*/
UCLASS()
class MIRRORANIMATIONSYSTEM_API UExtCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
protected:
	virtual void PerformMovement(float DeltaSeconds) override;

public:
	UPROPERTY(Category = "Character Movement: Root Motion", EditAnywhere, BlueprintReadWrite)
		bool MirrorRootMotion = false;

	UPROPERTY(Category = "Character Movement: Root Motion", EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAxis::Type> MirrorAxis = EAxis::None;

	UPROPERTY(Category = "Character Movement: Root Motion", EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAxis::Type> FlipAxis = EAxis::None;
};
