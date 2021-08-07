// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Engine/Texture2D.h"
#include "IDetailsView.h"
#include "Animation/AnimSequence.h"
#include "MirrorAnimAssetSettings.h"

/*Dialog used to choose the Mirror Table,
it then Mirrors the animation according to the data contained in the Mirror Table*/
class MIRRORANIMATIONSYSTEMEDITOR_API SMirrorAnimAssetDialog : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMirrorAnimAssetDialog) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UAnimSequence* AnimSequence);

	~SMirrorAnimAssetDialog();

	static bool ShowWindow(UAnimSequence* SourceAnimSequence);

	static void CreateMirrorSequenceFromAnimSequence(UAnimSequence* MirrorSequence, UMirrorTable* MirrorTable);
private:

	UAnimSequence* SourceAnimSequence;

	class UMirrorAnimAssetSettings* MirrorAnimAssetSettings;

	bool ValidateCSMirrorData();

	FReply MirrorClicked();

	FReply CancelClicked();

	void CloseContainingWindow();

	void CreateMirroredAnimSequences();

	TSharedPtr<class IDetailsView> MainPropertyView;
};
