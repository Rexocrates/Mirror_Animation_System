// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"

#include "MirrorTable.h"

#include "AnimNode_MirrorCS.generated.h"


// Component Space version of the Mirror Pose node.
USTRUCT(BlueprintInternalUseOnly)
struct MIRRORANIMATIONSYSTEM_API FAnimNode_MirrorCS : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MirrorSettings)
		TEnumAsByte<EAxis::Type> MirrorAxis = EAxis::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MirrorSettings)
		bool CompletlySymmetrical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MirrorBones)
	FString Substring_A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MirrorBones)
	FString Substring_B;

	~FAnimNode_MirrorCS();
	FAnimNode_MirrorCS();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	// End of FAnimNode_SkeletalControlBase interface

private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface


	void SetMirrorDataIfDirty(const FReferenceSkeleton& RefSkeleton);
	TArray<FIntPoint> TwinPairs;
	TArray<int32> NonTwinIDs;
	TArray<EAxis::Type> NonTwinFlipAxis;
	int32 LastBoneNum = 0;
};

