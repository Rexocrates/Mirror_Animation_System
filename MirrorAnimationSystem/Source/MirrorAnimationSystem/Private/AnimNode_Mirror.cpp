// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "AnimNode_Mirror.h"
#include "MirrorAnimationSystem.h"

//#include "AnimationRuntime.h"

FAnimNode_Mirror::FAnimNode_Mirror()
	: 
	MirrorTable(NULL)
{

}

void FAnimNode_Mirror::Initialize_AnyThread(const FAnimationInitializeContext & Context)
{
	
	FAnimNode_Base::Initialize_AnyThread(Context);
	
	BasePose.Initialize(Context);
}

void FAnimNode_Mirror::CacheBones_AnyThread(const FAnimationCacheBonesContext & Context)
{
	
	BasePose.CacheBones(Context);
}

void FAnimNode_Mirror::Update_AnyThread(const FAnimationUpdateContext & Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);
	BasePose.Update(Context);
}


void FAnimNode_Mirror::Evaluate_AnyThread(FPoseContext & Output)
{
	// Evaluate the input

	BasePose.Evaluate(Output);
	
	if ((MirrorTable != NULL) || (MirrorTable != nullptr))
	{
		int Number = MirrorTable->MirrorBones.Num();
		if (Number > 0)
		{
			USkeletalMeshComponent* SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();

			if (SkelComp != NULL)
			{

				for (int i = 0; i < Number; i++)
				{

					FMirrorBone CurrentBone = MirrorTable->MirrorBones[i];
					int BoneIndex = SkelComp->GetBoneIndex(CurrentBone.BoneName);
					FCompactPoseBoneIndex CmpctBoneIndex(BoneIndex);
					if (Output.Pose.IsValidIndex(CmpctBoneIndex))
					{
						
						FTransform BoneTransform = Output.Pose[CmpctBoneIndex];

						BoneTransform.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

						if (CurrentBone.RotationOffset != FRotator::ZeroRotator)
						{
							FRotator BoneNewRotation = BoneTransform.Rotator();

							BoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
							BoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
							BoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

							BoneTransform.SetRotation(FQuat(BoneNewRotation));
						}

						if (CurrentBone.IsTwinBone)
						{

							int TwinBoneIndex = SkelComp->GetBoneIndex(CurrentBone.TwinBoneName);
							FCompactPoseBoneIndex CmpctTwinBoneIndex(TwinBoneIndex);

							if (Output.Pose.IsValidIndex(CmpctTwinBoneIndex))
							{

								
								FTransform TwinBoneTransform = Output.Pose[CmpctTwinBoneIndex];
								TwinBoneTransform.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

								if (CurrentBone.RotationOffset != FRotator::ZeroRotator)
								{
									FRotator TwinBoneNewRotation = TwinBoneTransform.Rotator();
									TwinBoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
									TwinBoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
									TwinBoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;
									TwinBoneTransform.SetRotation(FQuat(TwinBoneNewRotation));
								}

								TwinBoneTransform.SetScale3D(TwinBoneTransform.GetScale3D().GetAbs());
								BoneTransform.SetScale3D(BoneTransform.GetScale3D().GetAbs());

								if (CurrentBone.MirrorTranslation)
								{
									//TwinBoneTransform.SetRotation(TwinBoneTransform.GetRotation());
									//BoneTransform.SetRotation(FQuat(BoneNewRotation));
									
									
									Output.Pose[CmpctBoneIndex] = TwinBoneTransform;
									Output.Pose[CmpctBoneIndex].NormalizeRotation();

									Output.Pose[CmpctTwinBoneIndex] = BoneTransform;
									Output.Pose[CmpctTwinBoneIndex].NormalizeRotation();
								}
								else
								{
									FVector TwinPos = TwinBoneTransform.GetLocation();
									FVector Pos = BoneTransform.GetLocation();

									TwinBoneTransform.SetLocation(Pos);
									BoneTransform.SetLocation(TwinPos);

									Output.Pose[CmpctBoneIndex] = TwinBoneTransform;
									Output.Pose[CmpctBoneIndex].NormalizeRotation();

									Output.Pose[CmpctTwinBoneIndex] = BoneTransform;
									Output.Pose[CmpctTwinBoneIndex].NormalizeRotation();
								}
							}

						}

						else
						{

							BoneTransform.SetScale3D(BoneTransform.GetScale3D().GetAbs());

							Output.Pose[CmpctBoneIndex] = BoneTransform;
							//Output.Pose[CmpctBoneIndex].NormalizeRotation();
						}
					}
				}
			}
		}
	}
}


void FAnimNode_Mirror::GatherDebugData(FNodeDebugData & DebugData)
{
	FString DebugLine = DebugData.GetNodeName(this);

	DebugData.AddDebugItem(DebugLine);

	BasePose.GatherDebugData(DebugData);
}