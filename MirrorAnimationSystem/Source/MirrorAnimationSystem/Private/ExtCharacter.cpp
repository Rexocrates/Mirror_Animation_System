// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "ExtCharacter.h"
#include "MirrorAnimationSystem.h"



// Sets default values
AExtCharacter::AExtCharacter(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	:
	Super(ObjectInitializer.SetDefaultSubobjectClass<UExtCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExtCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AExtCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AExtCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AExtCharacter::SetMirrorRootMotion(bool Mirror)
{
	UExtCharacterMovementComponent * ChMovComp = Cast<UExtCharacterMovementComponent>(AExtCharacter::GetCharacterMovement());
	if (ChMovComp != NULL)
	{
		ChMovComp->MirrorRootMotion = Mirror;
	}
}

bool AExtCharacter::GetMirrorRootMotion()
{
	UExtCharacterMovementComponent * ChMovComp = Cast<UExtCharacterMovementComponent>(AExtCharacter::GetCharacterMovement());
	if (ChMovComp != NULL)
	{
		return ChMovComp->MirrorRootMotion;
	}
	return false;
}

void AExtCharacter::SetRootMotionMirrorAndFlipAxis(TEnumAsByte<EAxis::Type> MirrorAxis, TEnumAsByte<EAxis::Type> FlipAxis)
{
	UExtCharacterMovementComponent * ChMovComp = Cast<UExtCharacterMovementComponent>(AExtCharacter::GetCharacterMovement());
	if (ChMovComp != NULL)
	{
		ChMovComp->MirrorAxis = MirrorAxis;
		ChMovComp->FlipAxis = FlipAxis;
		return;
	}
}

void AExtCharacter::GetRootMotionMirrorAndFlipAxis(TEnumAsByte<EAxis::Type>& MirrorAxis, TEnumAsByte<EAxis::Type>& FlipAxis)
{
	UExtCharacterMovementComponent * ChMovComp = Cast<UExtCharacterMovementComponent>(AExtCharacter::GetCharacterMovement());
	if (ChMovComp != NULL)
	{
		MirrorAxis = ChMovComp->MirrorAxis;
		FlipAxis = ChMovComp->FlipAxis;
		return;
	}
}

