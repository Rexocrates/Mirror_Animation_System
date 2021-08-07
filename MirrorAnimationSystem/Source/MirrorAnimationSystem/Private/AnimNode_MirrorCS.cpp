// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "AnimNode_MirrorCS.h"
#include "MirrorAnimationSystem.h"

#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"

#include "MASUtils.h"

/////////////////////////////////////////////////////
// FAnimNode_MirrorCS

FAnimNode_MirrorCS::~FAnimNode_MirrorCS()
{
	TwinPairs.Empty();
	NonTwinIDs.Empty();
	NonTwinFlipAxis.Empty();
}

FAnimNode_MirrorCS::FAnimNode_MirrorCS() :
CompletlySymmetrical(false)
{
	TwinPairs.Empty();
	NonTwinIDs.Empty();
	NonTwinFlipAxis.Empty();
}

void FAnimNode_MirrorCS::GatherDebugData(FNodeDebugData& DebugData)
{
	Super::GatherDebugData(DebugData);
}

void FAnimNode_MirrorCS::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	check(OutBoneTransforms.Num() == 0);

	if(NonTwinIDs.Num())
	{
		const bool DeltaStep = !CompletlySymmetrical;

		TArray <FTransform> NewCSTMs; NewCSTMs.SetNum(Output.Pose.GetPose().GetNumBones());

		const USkeletalMeshComponent* SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();
		const auto& RefSkeleton = SkelComp->SkeletalMesh->RefSkeleton;
		

		FVector TwinMirrorScale = FVector(1.f);
		FVector TargetAxis = FVector::ZeroVector;
		if (MirrorAxis != EAxis::None)
		{
			TwinMirrorScale[MirrorAxis - 1] = -1.f;
			TargetAxis[MirrorAxis - 1] = 1.f;
		}

		FTransform TwinMirrorModTM(FQuat::Identity, FVector::ZeroVector, TwinMirrorScale);

		for (int32 i = 0; i < NonTwinIDs.Num(); i++)
		{
			FCompactPoseBoneIndex CmptBoneIndex(NonTwinIDs[i]);

			FTransform TM = Output.Pose.GetComponentSpaceTransform(CmptBoneIndex);
			TM.Mirror(MirrorAxis, NonTwinFlipAxis[i]);

			OutBoneTransforms.Add(FBoneTransform(CmptBoneIndex, TM));
			NewCSTMs[NonTwinIDs[i]] = TM;
		}

		for (int32 i = 0; i < TwinPairs.Num(); i++)
		{
			int32 BoneIndex = TwinPairs[i].X;
			FCompactPoseBoneIndex CmptBoneIndex(BoneIndex);

			const int32 TwinBoneIndex = TwinPairs[i].Y;

			FCompactPoseBoneIndex TwinCmptBoneIndex(TwinBoneIndex);

			const FTransform RefTM = FAnimationRuntime::GetComponentSpaceTransformRefPose(RefSkeleton, BoneIndex);
			const FTransform TwinRefTM = FAnimationRuntime::GetComponentSpaceTransformRefPose(RefSkeleton, TwinBoneIndex);

			const FTransform TM = Output.Pose.GetComponentSpaceTransform(CmptBoneIndex);
			const FTransform TwinTM = Output.Pose.GetComponentSpaceTransform(TwinCmptBoneIndex);

			const int32 ParentIndex = RefSkeleton.GetParentIndex(BoneIndex);
			const int32 TwinParentIndex = RefSkeleton.GetParentIndex(TwinBoneIndex);

			const bool SameParent = ParentIndex == TwinParentIndex;

			
		

			// twin 1º
			{
				const FTransform MirrRef = RefTM * TwinMirrorModTM;
				const FTransform Delta = TwinRefTM.GetRelativeTransform(MirrRef);
				const FQuat DeltaQuat = Delta.GetRotation();
				
				FTransform MirrTM = TM * TwinMirrorModTM;

				MirrTM.SetRotation(MirrTM.GetRotation() * DeltaQuat);
				MirrTM.SetScale3D(TwinTM.GetScale3D());

				if(DeltaStep)
				{
					if (SameParent)
					{
						FTransform RefBS = RefTM;
						RefBS = RefBS * TwinMirrorModTM;
						const FVector PosDelta = MirrTM.GetLocation() - RefBS.GetLocation();
						MirrTM.SetLocation(TwinRefTM.GetLocation() + PosDelta);
					}
					else
					{
						const FTransform& ParentTwinTM = NewCSTMs[RefSkeleton.GetParentIndex(TwinBoneIndex)];
						const FTransform& IParentTM = Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(ParentIndex));
						FTransform RefBS = RefSkeleton.GetRefBonePose()[BoneIndex] * IParentTM;
						RefBS = RefBS * TwinMirrorModTM;
						RefBS.SetRotation(RefBS.GetRotation() * DeltaQuat);
						RefBS.SetScale3D(TwinTM.GetScale3D());

						MirrTM = (MirrTM.GetRelativeTransform(RefBS) * RefSkeleton.GetRefBonePose()[TwinBoneIndex]) * ParentTwinTM;
					}
				}


				OutBoneTransforms.Add(FBoneTransform(TwinCmptBoneIndex, MirrTM));
				NewCSTMs[TwinBoneIndex] = MirrTM;
			}

			// twin 2º
			{
				FTransform TwinMirrRef = TwinRefTM * TwinMirrorModTM;
				const FQuat TwinDeltaQuat = TwinMirrRef.GetRotation().Inverse() * RefTM.GetRotation();
				
				FTransform TwinMirrTM = TwinTM * TwinMirrorModTM;

				TwinMirrTM.SetRotation(TwinMirrTM.GetRotation() * TwinDeltaQuat);
				TwinMirrTM.SetScale3D(TM.GetScale3D());
				
				if (DeltaStep)
				{
					if (SameParent)
					{
						FTransform TwinRefBS = TwinRefTM;
						TwinRefBS = TwinRefBS * TwinMirrorModTM;
						const FVector PosDelta = TwinMirrTM.GetLocation() - TwinRefBS.GetLocation();
						TwinMirrTM.SetLocation(RefTM.GetLocation() + PosDelta);
					}
					else
					{
						const FTransform& ParentTM = NewCSTMs[RefSkeleton.GetParentIndex(BoneIndex)];
						const FTransform& IParentTwinTM = Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(TwinParentIndex));
						FTransform TwinRefBS = RefSkeleton.GetRefBonePose()[TwinBoneIndex] * IParentTwinTM;
						TwinRefBS = TwinRefBS * TwinMirrorModTM;
						TwinRefBS.SetRotation(TwinRefBS.GetRotation() * TwinDeltaQuat);
						TwinRefBS.SetScale3D(TM.GetScale3D());

						TwinMirrTM = (TwinMirrTM.GetRelativeTransform(TwinRefBS) * RefSkeleton.GetRefBonePose()[BoneIndex]) * ParentTM;
					}
				}

				OutBoneTransforms.Add(FBoneTransform(CmptBoneIndex, TwinMirrTM));
				NewCSTMs[BoneIndex] = TwinMirrTM;
			}
		}
		
	}

	if (OutBoneTransforms.Num())
	{
		OutBoneTransforms.Sort(FCompareBoneTransformIndex());
	}
}

bool FAnimNode_MirrorCS::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return true;
}

void FAnimNode_MirrorCS::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	Super::InitializeBoneReferences(RequiredBones);

	SetMirrorDataIfDirty(RequiredBones.GetReferenceSkeleton());
}

void FAnimNode_MirrorCS::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);
}


void FAnimNode_MirrorCS::SetMirrorDataIfDirty(const FReferenceSkeleton& RefSkeleton)
{
	if (RefSkeleton.GetNum() == LastBoneNum) return;

	const int32 NumBones = RefSkeleton.GetNum();
	LastBoneNum = NumBones;
	TArray <bool> Already; Already.SetNumZeroed(NumBones);

	FVector TargetAxis = FVector::ZeroVector;
	if (MirrorAxis != EAxis::None)
	{
		TargetAxis[MirrorAxis - 1] = 1.f;
	}

	for (int32 BoneIndex = 0; BoneIndex < NumBones; BoneIndex++)
	{
		if (Already[BoneIndex] == true) continue;

		FName BoneName = RefSkeleton.GetBoneName(BoneIndex);
		FName TwinBoneName;
		if (FMASUtils::TwinSubstring(BoneName, Substring_A, Substring_B, TwinBoneName))
		{
			const int32 TwinBoneIndex = RefSkeleton.FindBoneIndex(TwinBoneName);
			if (TwinBoneIndex != INDEX_NONE)
			{
				Already[BoneIndex] = true;
				Already[TwinBoneIndex] = true;
				TwinPairs.Add(FIntPoint(BoneIndex, TwinBoneIndex));
			}
		}
		else
		{
			const FTransform RefTM = FAnimationRuntime::GetComponentSpaceTransformRefPose(RefSkeleton, BoneIndex);

			EAxis::Type WinnerAxis = EAxis::None;
			float Max = -FLT_MAX;

			for (int32 Axis = 0; Axis < 3; Axis++)
			{
				if (Axis == 0)
				{
					const float Val = FMath::Abs(RefTM.GetRotation().GetAxisX() | TargetAxis);
					if (Val > Max)
					{
						Max = Val;
						WinnerAxis = EAxis::X;
					}
				}
				else if (Axis == 1)
				{
					const float Val = FMath::Abs(RefTM.GetRotation().GetAxisY() | TargetAxis);
					if (Val > Max)
					{
						Max = Val;
						WinnerAxis = EAxis::Y;
					}
				}
				else if (Axis == 2)
				{
					const float Val = FMath::Abs(RefTM.GetRotation().GetAxisZ() | TargetAxis);
					if (Val > Max)
					{
						Max = Val;
						WinnerAxis = EAxis::Z;
					}
				}
			}

			NonTwinIDs.Add(BoneIndex);
			NonTwinFlipAxis.Add(WinnerAxis);
			
		}
	}
}
