// Copyright 2017-2021 Rexocrates. All Rights Reserved.


#include "MASUtils.h"

bool FMASUtils::TwinSubstring(const FName BoneName, const FString Substring_A, const FString Substring_B, FName& OutTwinBoneName)
{
	FString CurrentBoneString = BoneName.ToString();

	int ResultA = CurrentBoneString.Find(Substring_A, ESearchCase::IgnoreCase, ESearchDir::FromEnd, -1);
	int ResultB = CurrentBoneString.Find(Substring_B, ESearchCase::IgnoreCase, ESearchDir::FromEnd, -1);

	if ((ResultA != -1) || (ResultB != -1))
	{
		FString TwinBoneString;
		bool ValidSymmetry = true;
		if (ResultB != -1)
		{

			TwinBoneString = CurrentBoneString.Mid(0, ResultB);
			TwinBoneString += Substring_A;
			TwinBoneString += CurrentBoneString.Mid(ResultB + Substring_B.Len());

			if ((!(abs(TwinBoneString.Len() - Substring_A.Len()) == abs(CurrentBoneString.Len() - Substring_B.Len()))) && ValidSymmetry)
			{
				ValidSymmetry = false;
			}
		}
		else
		{

			TwinBoneString = CurrentBoneString.Mid(0, ResultA);
			TwinBoneString += Substring_B;
			TwinBoneString += CurrentBoneString.Mid(ResultA + Substring_A.Len());

			if ((!(abs(TwinBoneString.Len() - Substring_B.Len()) == abs(CurrentBoneString.Len() - Substring_A.Len()))) && ValidSymmetry)
			{
				ValidSymmetry = false;
			}
		}

		if (ValidSymmetry)
		{
			OutTwinBoneName = FName(*TwinBoneString);
			return true;
		}
	}

	return false;
}

void FMASUtils::CSMirrorSettings(
	const FReferenceSkeleton& RefSkeleton,
	const EAxis::Type MirrorAxis, const FString Substring_A, const FString Substring_B,
	TArray<FIntPoint>& TwinPairs, TArray<int32>& NonTwinIDs, TArray<EAxis::Type>& NonTwinFlipAxis)
{
	TwinPairs.Empty();
	NonTwinIDs.Empty();
	NonTwinFlipAxis.Empty();

	const int32 NumBones = RefSkeleton.GetNum();
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
