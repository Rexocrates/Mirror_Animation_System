// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Object.h"
#include "MirrorTable.h"
#include "MirrorAnimAssetSettings.generated.h"
/**
 * 
 */
/*class used in Mirror AnimAsset Dialog for choosing the Mirror Table asset in a details view*/
UCLASS()
class MIRRORANIMATIONSYSTEMEDITOR_API UMirrorAnimAssetSettings : public UObject
{
	GENERATED_BODY()
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentSpaceMirrorSettings)
		TEnumAsByte<EAxis::Type> MirrorAxis = EAxis::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentSpaceMirrorSettings)
		bool CompletlySymmetrical = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentSpaceMirrorSettings)
		FString Substring_A = "None";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentSpaceMirrorSettings)
		FString Substring_B = "None";

	UPROPERTY(Category = BoneSpaceMirrorSettings, EditAnywhere, meta = (HideAlphaChannel))
		UMirrorTable* MirrorTable;

	UMirrorAnimAssetSettings(const FObjectInitializer& ObjectInitializer);
};
