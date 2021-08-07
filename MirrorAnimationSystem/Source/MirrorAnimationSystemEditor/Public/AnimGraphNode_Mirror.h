// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "AnimGraphNode_Base.h"
#include "AnimNode_Mirror.h"
#include "AnimGraphNode_Mirror.generated.h"
/**
 * 
 */
/*class that holds Editor version of the AnimGraph Node Mirror Pose, along its tittle, tooltip, Node Color, and the category of the node*/
UCLASS()
class MIRRORANIMATIONSYSTEMEDITOR_API UAnimGraphNode_Mirror : public UAnimGraphNode_Base
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_Mirror Node;

	//~ Begin UEdGraphNode Interface.
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	//~ End UAnimGraphNode_Base Interface

	UAnimGraphNode_Mirror(const FObjectInitializer& ObjectInitializer);
};
