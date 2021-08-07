// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
//#include "AnimGraphNode_Base.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNode_MirrorCS.h"
#include "AnimGraphNode_MirrorCS.generated.h"
/**
 * 
 */
/*class that holds Editor version of the AnimGraph Node Mirror Pose, along its tittle, tooltip, Node Color, and the category of the node*/
UCLASS()
class MIRRORANIMATIONSYSTEMEDITOR_API UAnimGraphNode_MirrorCS : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_MirrorCS Node;
	/*
	//~ Begin UEdGraphNode Interface.
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	//~ End UAnimGraphNode_Base Interface

	UAnimGraphNode_MirrorCS(const FObjectInitializer& ObjectInitializer);
	*/


public:
	UAnimGraphNode_MirrorCS(const FObjectInitializer& ObjectInitializer);

	// UEdGraphNode interface
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// validate if this is within VehicleAnimInstance
	virtual void ValidateAnimNodePostCompile(class FCompilerResultsLog& MessageLog, class UAnimBlueprintGeneratedClass* CompiledClass, int32 CompiledNodeIndex) override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	// End of UEdGraphNode interface

protected:
	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	// End of UAnimGraphNode_SkeletalControlBase interface
};
