// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "UObject/NoExportTypes.h"
#include "MirrorTableFromSkeletonSettings.generated.h"
/**
 * 
 */
/*class used in Mirror Table from Skeleton Dialog for creating the details view panel 
and settting up the Suffix or preffix for twin identification as well as
defining the default values for the bones that will be added to the new Mirror Table*/
UCLASS()
class MIRRORANIMATIONSYSTEMEDITOR_API UMirrorTableFromSkeletonSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = SearchSettings, EditAnywhere, meta = (HideAlphaChannel))
		FString Substring_A = "none";
	UPROPERTY(Category = SearchSettings, EditAnywhere, meta = (HideAlphaChannel))
		FString Substring_B = "none";

	UPROPERTY(Category = NonTwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		TEnumAsByte<EAxis::Type> DefaultMirrorAxis = EAxis::None;
	UPROPERTY(Category = NonTwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		TEnumAsByte<EAxis::Type> DefaultFlipAxis = EAxis::None;
	UPROPERTY(Category = NonTwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		FRotator DefaultRotationOffset = FRotator(0, 0, 0);

	UPROPERTY(Category = TwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		TEnumAsByte<EAxis::Type> DefaultTwinMirrorAxis = EAxis::None;
	UPROPERTY(Category = TwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		TEnumAsByte<EAxis::Type> DefaultTwinFlipAxis = EAxis::None;
	UPROPERTY(Category = TwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		FRotator DefaultTwinRotationOffset = FRotator(0, 0, 0);
	UPROPERTY(Category = TwinBonesSettings, EditAnywhere, meta = (HideAlphaChannel))
		bool DefaultTwinMirrorTranslation = false;


	UMirrorTableFromSkeletonSettings(const FObjectInitializer& ObjectInitializer);
};
