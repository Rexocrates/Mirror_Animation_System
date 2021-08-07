// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "MirrorTable.generated.h"

/**
 * 
 */
/*Struct that contains the setup data for Mirroring a single bone or a pair of bones*/
USTRUCT(BlueprintType)
struct MIRRORANIMATIONSYSTEM_API FMirrorBone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		TEnumAsByte<EAxis::Type> MirrorAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		TEnumAsByte<EAxis::Type> FlipAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		bool IsTwinBone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		FName TwinBoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorAnimation")
		bool MirrorTranslation;
};

/*Data asset class that holds the MirrorBone's parameters for an entire skeleton, 
this class is used for both the Mirror Pose Animgraph Node and when selecting a Mirror Table inside the Mirror AnimAsset dialog*/
UCLASS(BlueprintType)
class MIRRORANIMATIONSYSTEM_API UMirrorTable : public UDataAsset
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MirrorBoneSettings")
		TArray <FMirrorBone> MirrorBones;
	
};
