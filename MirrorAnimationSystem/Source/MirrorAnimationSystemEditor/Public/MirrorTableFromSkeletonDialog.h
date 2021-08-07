// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Engine/Texture2D.h"
#include "IDetailsView.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"
#include "MirrorTable.h"

#include "MirrorTableFromSkeletonSettings.h"
/**
 * 
 */
class USkeleton;
/*SCompound widget class containing the functionality for creating a new Mirror Table asset based on a Skeleton Asset,
the dialog serves to input the Prefixes or suffixes used in the skeleton to identify whether a bone is a twin bone and if so find it's corresponding twin.
At the same time it allows to setup quick initial values for all the bones that are being added to the new Mirror Table*/
class MIRRORANIMATIONSYSTEMEDITOR_API SMirrorTableFromSkeletonDialog : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMirrorTableFromSkeletonDialog) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, USkeleton* Skeleton);

	~SMirrorTableFromSkeletonDialog();

	static bool ShowWindow(USkeleton* SourceSkeleton);

	void SetMirrorTableFromSettings(UMirrorTable * MirrorTable);

private:

	USkeleton* SourceSkeleton;

	class UMirrorTableFromSkeletonSettings* MirrorTableSettings;
	
	FReply CreateClicked();

	FReply CancelClicked();

	void CloseContainingWindow();

	void CreateMirrorTableFromSkeletonFunction();

	TSharedPtr<class IDetailsView> MainPropertyView;
};
