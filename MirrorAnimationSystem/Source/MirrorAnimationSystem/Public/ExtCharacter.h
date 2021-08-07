// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "ExtCharacterMovementComponent.h"
#include "ExtCharacter.generated.h"
/*Character class that merely implements ExtCharacterMovementComponent and some blueprint functions to set the Mirror Root Motion Parameters*/
UCLASS()
class MIRRORANIMATIONSYSTEM_API AExtCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AExtCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable, Category = "Mirror Animation")
		virtual void SetMirrorRootMotion(bool Mirror);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mirror Animation")
		virtual bool GetMirrorRootMotion();

	UFUNCTION(BlueprintCallable, Category = "Mirror Animation")
		virtual void SetRootMotionMirrorAndFlipAxis(TEnumAsByte<EAxis::Type> MirrorAxis, TEnumAsByte<EAxis::Type> FlipAxis);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mirror Animation")
		virtual void GetRootMotionMirrorAndFlipAxis(TEnumAsByte<EAxis::Type> & MirrorAxis, TEnumAsByte<EAxis::Type> & FlipAxis);
};
