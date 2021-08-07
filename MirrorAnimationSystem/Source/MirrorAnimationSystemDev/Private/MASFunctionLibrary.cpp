// Copyright 2017-2021 Rexocrates. All Rights Reserved.


#include "MASFunctionLibrary.h"

#include "MirrorTable.h"

#include "Animation/AnimSequence.h"

#include "Misc/PackageName.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "Toolkits/AssetEditorManager.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#endif

#include "MASUtils.h"
#include "AnimationRuntime.h"

#define LOCTEXT_NAMESPACE "MASLibrary"

UMASFunctionLibrary::UMASFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMASFunctionLibrary::BulkMirrorEditorOnly(const TArray <UAnimSequence*> SourceAnims, const UMirrorTable* MirrorTable, TArray <UAnimSequence*>& OutNewAnims)
{
	if (SourceAnims.Num() == 0) return;
	if (MirrorTable == NULL) return;

#if WITH_EDITOR
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	for (int32 i = 0; i < SourceAnims.Num(); i++)
	{
		// Create the asset
		FString Name;
		FString PackageName;


		FString Suffix = TEXT("_Mirrored");

		AssetToolsModule.Get().CreateUniqueAssetName(SourceAnims[i]->GetOutermost()->GetName(), Suffix, /*out*/ PackageName, /*out*/ Name);
		const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);


		UObject* NewAsset = AssetToolsModule.Get().DuplicateAsset(Name, PackagePath, SourceAnims[i]);

		if (NewAsset != NULL)
		{
			UAnimSequence* MirrorAnimSequence = Cast<UAnimSequence>(NewAsset);
			CreateMirrorSequenceFromAnimSequence(MirrorAnimSequence, MirrorTable);

			OutNewAnims.Add(MirrorAnimSequence);

			// Notify asset registry of new asset
			FAssetRegistryModule::AssetCreated(MirrorAnimSequence);

			// Display notification so users can quickly access
			if (GIsEditor)
			{
				FNotificationInfo Info(FText::Format(LOCTEXT("AnimationMirrored", "Successfully Mirrored Animation"), FText::FromString(MirrorAnimSequence->GetName())));
				Info.ExpireDuration = 8.0f;
				Info.bUseLargeFont = false;
				//Info.Hyperlink = FSimpleDelegate::CreateLambda([=]() { FAssetEditorManager::Get().OpenEditorForAssets(TArray<UObject*>({ MirrorAnimSequence })); });
				Info.Hyperlink = FSimpleDelegate::CreateLambda([=]() { GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(TArray<UObject*>({ MirrorAnimSequence })); });

				Info.HyperlinkText = FText::Format(LOCTEXT("OpenNewAnimationHyperlink", "Open {0}"), FText::FromString(MirrorAnimSequence->GetName()));
				TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
				if (Notification.IsValid())
				{
					Notification->SetCompletionState(SNotificationItem::CS_Success);
				}
			}
		}
	}
#endif
}

MIRRORANIMATIONSYSTEMDEV_API void UMASFunctionLibrary::BulkMirror_CS_EditorOnly(
	const TArray<UAnimSequence*> SourceAnims, 
	const TEnumAsByte<EAxis::Type> MirrorAxis, const FString Substring_A, const FString Substring_B, const bool Symmetrical, TArray<UAnimSequence*>& OutNewAnims)
{
	if (SourceAnims.Num() == 0) return;
	if (MirrorAxis == EAxis::None) return;

#if WITH_EDITOR
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	for (int32 i = 0; i < SourceAnims.Num(); i++)
	{
		// Create the asset
		FString Name;
		FString PackageName;


		FString Suffix = TEXT("_Mirrored");

		AssetToolsModule.Get().CreateUniqueAssetName(SourceAnims[i]->GetOutermost()->GetName(), Suffix, /*out*/ PackageName, /*out*/ Name);
		const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);


		UObject* NewAsset = AssetToolsModule.Get().DuplicateAsset(Name, PackagePath, SourceAnims[i]);

		if (NewAsset != NULL)
		{
			UAnimSequence* MirrorAnimSequence = Cast<UAnimSequence>(NewAsset);
			CreateMirrorSequenceFromAnimSequence_CS(MirrorAnimSequence, MirrorAxis, Substring_A, Substring_B, Symmetrical);

			OutNewAnims.Add(MirrorAnimSequence);

			// Notify asset registry of new asset
			FAssetRegistryModule::AssetCreated(MirrorAnimSequence);

			// Display notification so users can quickly access
			if (GIsEditor)
			{
				FNotificationInfo Info(FText::Format(LOCTEXT("AnimationMirrored", "Successfully Mirrored Animation"), FText::FromString(MirrorAnimSequence->GetName())));
				Info.ExpireDuration = 8.0f;
				Info.bUseLargeFont = false;
				//Info.Hyperlink = FSimpleDelegate::CreateLambda([=]() { FAssetEditorManager::Get().OpenEditorForAssets(TArray<UObject*>({ MirrorAnimSequence })); });
				Info.Hyperlink = FSimpleDelegate::CreateLambda([=]() { GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(TArray<UObject*>({ MirrorAnimSequence })); });

				Info.HyperlinkText = FText::Format(LOCTEXT("OpenNewAnimationHyperlink", "Open {0}"), FText::FromString(MirrorAnimSequence->GetName()));
				TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
				if (Notification.IsValid())
				{
					Notification->SetCompletionState(SNotificationItem::CS_Success);
				}
			}
		}
	}
#endif
}

#if WITH_EDITOR

void UMASFunctionLibrary::CreateMirrorSequenceFromAnimSequence(UAnimSequence* MirrorSequence, const UMirrorTable* MirrorTable)
{
	//Check if it's valid
	if ((MirrorSequence != NULL) && (MirrorTable != NULL) && (MirrorSequence->GetSkeleton() != NULL))
	{
		//Make the duplicate that I will edit
		//UAnimSequence* MirrorSequence = FromAnimSequence;
		const auto& Skel = MirrorSequence->GetSkeleton()->GetReferenceSkeleton();

		int NumMirrorBones = MirrorTable->MirrorBones.Num();

		int NumFrames = MirrorSequence->GetNumberOfFrames();
		TArray<FRawAnimSequenceTrack> SourceRawAnimDatas = MirrorSequence->GetRawAnimationData();
		const auto& TrackNames = MirrorSequence->GetAnimationTrackNames();

		for (int i = 0; i < NumMirrorBones; i++)
		{
			FMirrorBone CurrentBone = MirrorTable->MirrorBones[i];

			if (Skel.FindBoneIndex(CurrentBone.BoneName) == INDEX_NONE)
			{
				continue;
			}

			if (CurrentBone.IsTwinBone)
			{
				if (Skel.FindBoneIndex(CurrentBone.TwinBoneName) == INDEX_NONE)
				{
					continue;
				}

				int32 TrackIndex = TrackNames.IndexOfByKey(CurrentBone.BoneName);
				int32 TwinTrackIndex = TrackNames.IndexOfByKey(CurrentBone.TwinBoneName);

				if (TrackIndex == INDEX_NONE && TwinTrackIndex == INDEX_NONE)
				{
					continue;
				}

				TArray <FVector> MirrorPosKeys;
				TArray <FQuat> MirrorRotKeys;
				TArray <FVector> MirrorScaleKeys;

				TArray <FVector> TwinMirrorPosKeys;
				TArray <FQuat> TwinMirrorRotKeys;
				TArray <FVector> TwinMirrorScaleKeys;

				// Original Bone
				if (TrackIndex != INDEX_NONE)
				{
					auto& MirroredRawTrack = SourceRawAnimDatas[TrackIndex];

					for (int u = 0; u < NumFrames; u++)
					{
						FTransform MirrorTM;

						bool bSetPos = false;
						bool bSetRot = false;
						bool bSetScale = false;

						if (MirroredRawTrack.PosKeys.IsValidIndex(u))
						{
							MirrorTM.SetTranslation(MirroredRawTrack.PosKeys[u]);
							bSetPos = true;
						}
						if (MirroredRawTrack.RotKeys.IsValidIndex(u))
						{
							MirrorTM.SetRotation(MirroredRawTrack.RotKeys[u]);
							bSetRot = true;
						}
						if (MirroredRawTrack.ScaleKeys.IsValidIndex(u))
						{
							MirrorTM.SetScale3D(MirroredRawTrack.ScaleKeys[u]);
							bSetScale = true;
						}

						MirrorTM.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

						FRotator BoneNewRotation = MirrorTM.Rotator();

						BoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
						BoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
						BoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

						MirrorTM.SetRotation(FQuat(BoneNewRotation));
						MirrorTM.SetScale3D(MirrorTM.GetScale3D().GetAbs());
						MirrorTM.NormalizeRotation();

						if (bSetPos)
						{
							MirrorPosKeys.Add(MirrorTM.GetTranslation());
						}
						if (bSetRot)
						{
							MirrorRotKeys.Add(MirrorTM.GetRotation());
						}
						if (bSetScale)
						{
							MirrorScaleKeys.Add(MirrorTM.GetScale3D());
						}
					}
				}
				else
				{
					auto RefTM = Skel.GetRefBonePose()[Skel.FindBoneIndex(CurrentBone.BoneName)];

					RefTM.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

					FRotator BoneNewRotation = RefTM.Rotator();

					BoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
					BoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
					BoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

					RefTM.SetRotation(FQuat(BoneNewRotation));
					RefTM.SetScale3D(RefTM.GetScale3D().GetAbs());
					RefTM.NormalizeRotation();

					MirrorPosKeys.Add(RefTM.GetTranslation());
					MirrorRotKeys.Add(RefTM.GetRotation());
				}

				// Twin Bone
				if (TwinTrackIndex != INDEX_NONE)
				{
					auto& TwinMirroredRawTrack = SourceRawAnimDatas[TwinTrackIndex];

					for (int u = 0; u < NumFrames; u++)
					{
						FTransform TwinMirrorTM;

						bool TwinbSetPos = false;
						bool TwinbSetRot = false;
						bool TwinbSetScale = false;

						if (TwinMirroredRawTrack.PosKeys.IsValidIndex(u))
						{
							TwinMirrorTM.SetTranslation(TwinMirroredRawTrack.PosKeys[u]);
							TwinbSetPos = true;
						}
						if (TwinMirroredRawTrack.RotKeys.IsValidIndex(u))
						{
							TwinMirrorTM.SetRotation(TwinMirroredRawTrack.RotKeys[u]);
							TwinbSetRot = true;
						}
						if (TwinMirroredRawTrack.ScaleKeys.IsValidIndex(u))
						{
							TwinMirrorTM.SetScale3D(TwinMirroredRawTrack.ScaleKeys[u]);
							TwinbSetScale = true;
						}

						TwinMirrorTM.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

						FRotator TwinBoneNewRotation = TwinMirrorTM.Rotator();

						TwinBoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
						TwinBoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
						TwinBoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

						TwinMirrorTM.SetRotation(FQuat(TwinBoneNewRotation));
						TwinMirrorTM.SetScale3D(TwinMirrorTM.GetScale3D().GetAbs());
						TwinMirrorTM.NormalizeRotation();

						if (TwinbSetPos)
						{
							TwinMirrorPosKeys.Add(TwinMirrorTM.GetTranslation());
						}
						if (TwinbSetRot)
						{
							TwinMirrorRotKeys.Add(TwinMirrorTM.GetRotation());
						}
						if (TwinbSetScale)
						{
							TwinMirrorScaleKeys.Add(TwinMirrorTM.GetScale3D());
						}
					}
				}
				else
				{
					auto RefTM = Skel.GetRefBonePose()[Skel.FindBoneIndex(CurrentBone.TwinBoneName)];

					RefTM.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

					FRotator TwinBoneNewRotation = RefTM.Rotator();

					TwinBoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
					TwinBoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
					TwinBoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

					RefTM.SetRotation(FQuat(TwinBoneNewRotation));
					RefTM.SetScale3D(RefTM.GetScale3D().GetAbs());
					RefTM.NormalizeRotation();

					TwinMirrorPosKeys.Add(RefTM.GetTranslation());
					TwinMirrorRotKeys.Add(RefTM.GetRotation());
				}

				// Original Bone -> Twin Bone
				{
					FRawAnimSequenceTrack NewTrack;

					NewTrack.PosKeys = CurrentBone.MirrorTranslation ? MirrorPosKeys : TwinMirrorPosKeys;
					NewTrack.RotKeys = MirrorRotKeys;
					NewTrack.ScaleKeys = MirrorScaleKeys;

					MirrorSequence->AddNewRawTrack(CurrentBone.TwinBoneName, &NewTrack);
				}

				// Twin Bone -> Original Bone
				{
					FRawAnimSequenceTrack NewTrack;

					NewTrack.PosKeys = CurrentBone.MirrorTranslation ? TwinMirrorPosKeys : MirrorPosKeys;
					NewTrack.RotKeys = TwinMirrorRotKeys;
					NewTrack.ScaleKeys = TwinMirrorScaleKeys;

					MirrorSequence->AddNewRawTrack(CurrentBone.BoneName, &NewTrack);
				}
			}
			else
			{
				int32 TrackIndex = TrackNames.IndexOfByKey(CurrentBone.BoneName);

				if (TrackIndex == INDEX_NONE)
				{
					continue;
				}

				FRawAnimSequenceTrack MirroredRawTrack = SourceRawAnimDatas[TrackIndex];

				//MirrorAllFrames
				TArray <FVector> MirrorPosKeys;
				TArray <FQuat> MirrorRotKeys;
				TArray <FVector> MirrorScaleKeys;

				for (int u = 0; u < NumFrames; u++)
				{
					//Mirror Transform
					FTransform MirrorTM;

					bool bSetPos = false;
					bool bSetRot = false;
					bool bSetScale = false;

					if (MirroredRawTrack.PosKeys.IsValidIndex(u))
					{
						MirrorTM.SetTranslation(MirroredRawTrack.PosKeys[u]);
						bSetPos = true;
					}
					if (MirroredRawTrack.RotKeys.IsValidIndex(u))
					{
						MirrorTM.SetRotation(MirroredRawTrack.RotKeys[u]);
						bSetRot = true;
					}
					if (MirroredRawTrack.ScaleKeys.IsValidIndex(u))
					{
						MirrorTM.SetScale3D(MirroredRawTrack.ScaleKeys[u]);
						bSetScale = true;
					}

					MirrorTM.Mirror(CurrentBone.MirrorAxis, CurrentBone.FlipAxis);

					FRotator BoneNewRotation = MirrorTM.Rotator();

					BoneNewRotation.Yaw += CurrentBone.RotationOffset.Yaw;
					BoneNewRotation.Roll += CurrentBone.RotationOffset.Roll;
					BoneNewRotation.Pitch += CurrentBone.RotationOffset.Pitch;

					MirrorTM.SetRotation(FQuat(BoneNewRotation));
					//MirrorTM.NormalizeRotation();
					MirrorTM.SetScale3D(MirrorTM.GetScale3D().GetAbs());

					MirrorTM.NormalizeRotation();

					//Setting it up Main
					if (bSetPos)
					{
						MirrorPosKeys.Add(MirrorTM.GetTranslation());
					}
					if (bSetRot)
					{
						MirrorRotKeys.Add(MirrorTM.GetRotation());
					}
					if (bSetScale)
					{
						MirrorScaleKeys.Add(MirrorTM.GetScale3D());
					}

					/////////////////////////////////
				}

				MirroredRawTrack.PosKeys = MirrorPosKeys;
				MirroredRawTrack.RotKeys = MirrorRotKeys;
				MirroredRawTrack.ScaleKeys = MirrorScaleKeys;

				//Finally Setting it in the AnimSequence

				MirrorSequence->AddNewRawTrack(CurrentBone.BoneName, &MirroredRawTrack);
			}
		}
		MirrorSequence->ClearBakedTransformData();
		MirrorSequence->RawCurveData.TransformCurves.Empty();
		MirrorSequence->bNeedsRebake = false;
		MirrorSequence->MarkRawDataAsModified();
		MirrorSequence->OnRawDataChanged();
		MirrorSequence->MarkPackageDirty();
	}
}


static FTransform GetAnimBoneTM(UAnimSequence* AnimSeq, const int32 BoneTreeIndex, const float AnimTime)
{
	USkeleton* Skeleton = AnimSeq->GetSkeleton();
	//int32 BoneTreeIndex = Skeleton->GetSkeletonBoneIndexFromMeshBoneIndex(SkelMesh, BoneTreeIndex);
	int32 BoneTrackIndex = Skeleton->GetRawAnimationTrackIndex(BoneTreeIndex, AnimSeq);
	if (BoneTrackIndex == INDEX_NONE)
	{
		return Skeleton->GetReferenceSkeleton().GetRefBonePose()[BoneTreeIndex];
	}
	FTransform BoneTM = FTransform::Identity;
	AnimSeq->GetBoneTransform(BoneTM, BoneTrackIndex, AnimTime, true);
	return BoneTM;
}

static FTransform GetAnimBoneCSTM(UAnimSequence* AnimSeq, const int32 BoneTreeIndex, const float AnimTime)
{
	USkeleton* Skeleton = AnimSeq->GetSkeleton();
	const auto& RefSkeleton = Skeleton->GetReferenceSkeleton();
	FTransform BoneTMWS = GetAnimBoneTM(AnimSeq, BoneTreeIndex, AnimTime);
	int32 CurrBone = BoneTreeIndex;
	while (true)
	{
		const int32 Parent(RefSkeleton.GetParentIndex(CurrBone));
		if (Parent < 0) break;
		else
		{
			
			BoneTMWS = BoneTMWS * GetAnimBoneTM(AnimSeq, Parent, AnimTime);

			CurrBone = Parent;
		}
	}
	return BoneTMWS;
}

MIRRORANIMATIONSYSTEMDEV_API void UMASFunctionLibrary::CreateMirrorSequenceFromAnimSequence_CS(
	UAnimSequence* MirrorSequence,
	const TEnumAsByte<EAxis::Type> MirrorAxis,
	const FString Substring_A, 
	const FString Substring_B, 
	const bool Symmetrical)
{
	const int32 NumFrames = MirrorSequence->GetRawNumberOfFrames();
	const float DT = MirrorSequence->SequenceLength / NumFrames;

	USkeleton* Skeleton = MirrorSequence->GetSkeleton();
	const auto& RefSkeleton = Skeleton->GetReferenceSkeleton();

	

	TArray <bool> Already; Already.SetNumZeroed(Skeleton->GetBoneTree().Num());

	TArray<FIntPoint> TwinPairs;
	TArray<int32> NonTwinIDs;
	TArray<EAxis::Type> NonTwinFlipAxis;
	FMASUtils::CSMirrorSettings(RefSkeleton, MirrorAxis, Substring_A, Substring_B, TwinPairs, NonTwinIDs, NonTwinFlipAxis);

	const bool DeltaStep = !Symmetrical;

	FVector TwinMirrorScale = FVector(1.f);
	FVector TargetAxis = FVector::ZeroVector;

	check(MirrorAxis != EAxis::None);
	{
		TwinMirrorScale[MirrorAxis - 1] = -1.f;
		TargetAxis[MirrorAxis - 1] = 1.f;
	}
	FTransform TwinMirrorModTM(FQuat::Identity, FVector::ZeroVector, TwinMirrorScale);


	TMap<int32, FRawAnimSequenceTrack> BoneTracks;
	
	for (int32 i = 0; i < RefSkeleton.GetNum(); i++)
	{
		BoneTracks.Add(i, FRawAnimSequenceTrack());
	}

	for (int32 j = 0; j < NumFrames; j++)
	{
		TArray <FTransform> NewCSTMs; NewCSTMs.SetNum(RefSkeleton.GetNum());

		for (int32 i = 0; i < NonTwinIDs.Num(); i++)
		{
			int32 BoneTreeIndex = NonTwinIDs[i];
			int32 BoneTrackIndex = Skeleton->GetRawAnimationTrackIndex(BoneTreeIndex, MirrorSequence);

			if (BoneTrackIndex == INDEX_NONE)
			{
				const int32 ParentIndex = RefSkeleton.GetParentIndex(BoneTreeIndex);
				if (ParentIndex != INDEX_NONE)
				{
					NewCSTMs[BoneTreeIndex] = RefSkeleton.GetRefBonePose()[BoneTreeIndex] * NewCSTMs[ParentIndex];
				}
				else
				{
					NewCSTMs[BoneTreeIndex] = RefSkeleton.GetRefBonePose()[BoneTreeIndex];
				}

				continue;
			}

			FTransform CSTM = GetAnimBoneCSTM(MirrorSequence, BoneTreeIndex, DT * j);
			CSTM.Mirror(MirrorAxis, NonTwinFlipAxis[i]);

			NewCSTMs[BoneTreeIndex] = CSTM;
		}


		for (int32 i = 0; i < TwinPairs.Num(); i++)
		{
			const int32 BoneIndex = TwinPairs[i].X;
			const FCompactPoseBoneIndex CmptBoneIndex(BoneIndex);

			const int32 TwinBoneIndex = TwinPairs[i].Y;

			const FCompactPoseBoneIndex TwinCmptBoneIndex(TwinBoneIndex);

			const FTransform RefTM = FAnimationRuntime::GetComponentSpaceTransformRefPose(RefSkeleton, BoneIndex);
			const FTransform TwinRefTM = FAnimationRuntime::GetComponentSpaceTransformRefPose(RefSkeleton, TwinBoneIndex);

			const FTransform TM = GetAnimBoneCSTM(MirrorSequence, BoneIndex, DT * j); 
			//Output.Pose.GetComponentSpaceTransform(CmptBoneIndex);
			const FTransform TwinTM = GetAnimBoneCSTM(MirrorSequence, TwinBoneIndex, DT * j); 
			//Output.Pose.GetComponentSpaceTransform(TwinCmptBoneIndex);

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

				if (DeltaStep)
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
						const FTransform& IParentTM =// Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(ParentIndex));
							GetAnimBoneCSTM(MirrorSequence, ParentIndex, DT * j);
						FTransform RefBS = RefSkeleton.GetRefBonePose()[BoneIndex] * IParentTM;
						RefBS = RefBS * TwinMirrorModTM;
						RefBS.SetRotation(RefBS.GetRotation() * DeltaQuat);
						RefBS.SetScale3D(TwinTM.GetScale3D());

						MirrTM = (MirrTM.GetRelativeTransform(RefBS) * RefSkeleton.GetRefBonePose()[TwinBoneIndex]) * ParentTwinTM;
					}
				}

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
						const FTransform& IParentTwinTM = //Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(TwinParentIndex));
							GetAnimBoneCSTM(MirrorSequence, TwinParentIndex, DT * j);
						FTransform TwinRefBS = RefSkeleton.GetRefBonePose()[TwinBoneIndex] * IParentTwinTM;
						TwinRefBS = TwinRefBS * TwinMirrorModTM;
						TwinRefBS.SetRotation(TwinRefBS.GetRotation() * TwinDeltaQuat);
						TwinRefBS.SetScale3D(TM.GetScale3D());

						TwinMirrTM = (TwinMirrTM.GetRelativeTransform(TwinRefBS) * RefSkeleton.GetRefBonePose()[BoneIndex]) * ParentTM;
					}
				}

				NewCSTMs[BoneIndex] = TwinMirrTM;
			}
		}


		for (int32 i = 0; i < NewCSTMs.Num(); i++)
		{
			const int32 ParentIndex = RefSkeleton.GetParentIndex(i);
			FTransform BSTM;
			if (ParentIndex != INDEX_NONE) BSTM = NewCSTMs[i].GetRelativeTransform(NewCSTMs[ParentIndex]);
			else BSTM = NewCSTMs[i];

			auto& BoneTrack = BoneTracks[i];
			BoneTrack.PosKeys.Add(BSTM.GetLocation());
			BoneTrack.RotKeys.Add(BSTM.GetRotation());
			BoneTrack.ScaleKeys.Add(BSTM.GetScale3D());
		}
	}

	for (auto Pair : BoneTracks)
	{
		const FName TrackName = Skeleton->GetReferenceSkeleton().GetBoneName(Pair.Key);
		MirrorSequence->AddNewRawTrack(TrackName, &Pair.Value);
	}
	// Have to also apply to pelvis and spine_01
	MirrorSequence->MarkRawDataAsModified();
	MirrorSequence->OnRawDataChanged();
	MirrorSequence->MarkPackageDirty();
}

#endif

#undef LOCTEXT_NAMESPACE