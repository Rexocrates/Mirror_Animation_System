// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "AnimGraphNode_Mirror.h"
#include "MirrorAnimationSystemEditor.h"


#define LOCTEXT_NAMESPACE "A3Nodes"

UAnimGraphNode_Mirror::UAnimGraphNode_Mirror(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

FLinearColor UAnimGraphNode_Mirror::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}

FText UAnimGraphNode_Mirror::GetTooltipText() const
{
	return LOCTEXT("Mirrors_the_designated_bones", "Mirrors the pose based on the designated Mirror Table");
}

FText UAnimGraphNode_Mirror::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Mirror_Pose", "Mirror Pose");
}

FString UAnimGraphNode_Mirror::GetNodeCategory() const
{
	return TEXT("Tools");
}

#undef LOCTEXT_NAMESPACE
