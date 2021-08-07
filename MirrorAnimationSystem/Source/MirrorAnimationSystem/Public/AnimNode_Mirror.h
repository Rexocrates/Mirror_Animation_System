// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstanceProxy.h"
#include "MirrorTable.h"
#include "AnimNode_Mirror.generated.h"
/**
*
*/
/*Runtime code for the AnimGraph Node Mirror Pose, 
takes in a pose in local space and Mirrors each bone according to a Mirror Table*/
USTRUCT(BlueprintInternalUseOnly)
struct MIRRORANIMATIONSYSTEM_API FAnimNode_Mirror : public FAnimNode_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
		FPoseLink BasePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MirrorTable, meta = (PinShownByDefault))
		UMirrorTable* MirrorTable;
public:

	FAnimNode_Mirror();

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext & Output) override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface
};
