// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "AnimGraphNode_MirrorCS.h"
#include "MirrorAnimationSystemEditor.h"


#define LOCTEXT_NAMESPACE "A3Nodes"


UAnimGraphNode_MirrorCS::UAnimGraphNode_MirrorCS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FLinearColor UAnimGraphNode_MirrorCS::GetNodeTitleColor() const
{
	return FLinearColor::Blue;
}

FText UAnimGraphNode_MirrorCS::GetControllerDescription() const
{
	return LOCTEXT("AnimGraphNode_MirrorCS", "Mirror Pose CS");
}

FText UAnimGraphNode_MirrorCS::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_MirrorCS_Tooltip", "Mirror the pose in Component Space.");
}

FText UAnimGraphNode_MirrorCS::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText NodeTitle;
	if (TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle)
	{
		NodeTitle = GetControllerDescription();
	}
	else
	{
		NodeTitle = FText(LOCTEXT("AnimGraphNode_MirrorCS_Title", "Mirror Pose CS"));
	}
	return NodeTitle;
}

void UAnimGraphNode_MirrorCS::ValidateAnimNodePostCompile(class FCompilerResultsLog& MessageLog, class UAnimBlueprintGeneratedClass* CompiledClass, int32 CompiledNodeIndex)
{
	Super::ValidateAnimNodePostCompile(MessageLog, CompiledClass, CompiledNodeIndex);
}

bool UAnimGraphNode_MirrorCS::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	return Super::IsCompatibleWithGraph(TargetGraph);
}

#undef LOCTEXT_NAMESPACE
