// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MASFunctionLibrary.generated.h"


class UAnimSequence;
class UMirrorTable;


UCLASS(MinimalAPI, meta = (ScriptName = "MirrorLibrary"))
class UMASFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Mirror Animation", meta = (DevelopmentOnly))
    static MIRRORANIMATIONSYSTEMDEV_API void BulkMirrorEditorOnly(const TArray <UAnimSequence*> Anims, const UMirrorTable* MirrorTable, TArray <UAnimSequence*>& OutNewAnims);
    UFUNCTION(BlueprintCallable, Category = "Mirror Animation", meta = (DevelopmentOnly))
    static MIRRORANIMATIONSYSTEMDEV_API void BulkMirror_CS_EditorOnly(const TArray <UAnimSequence*> Anims, 
        const TEnumAsByte<EAxis::Type> MirrorAxis, const FString Substring_A, const FString Substring_B, const bool Symmetrical, TArray <UAnimSequence*>& OutNewAnims);
#if WITH_EDITOR
    static MIRRORANIMATIONSYSTEMDEV_API void CreateMirrorSequenceFromAnimSequence(UAnimSequence* MirrorSequence, const UMirrorTable* MirrorTable);
    static MIRRORANIMATIONSYSTEMDEV_API void CreateMirrorSequenceFromAnimSequence_CS(UAnimSequence* MirrorSequence,
        const TEnumAsByte<EAxis::Type> MirrorAxis, const FString Substring_A, const FString Substring_B, const bool Symmetrical);
#endif
};
