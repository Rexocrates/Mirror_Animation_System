// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "MirrorAnimationSystemStyle.h"
#include "MirrorAnimationSystemEditor.h"

#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FMirrorAnimationSystemStyle::StyleInstance = NULL;

void FMirrorAnimationSystemStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMirrorAnimationSystemStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMirrorAnimationSystemStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MirrorAnimationSystemStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )



TSharedRef< FSlateStyleSet > FMirrorAnimationSystemStyle::Create()
{
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MirrorAnimationSystemStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MirrorAnimationSystem")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Icon.MirrorAnimAsset", new IMAGE_BRUSH(TEXT("MirrorAnimAsset_Icon"), Icon16x16));
	Style->Set("Icon.CreateMirrorTable", new IMAGE_BRUSH(TEXT("CreateMirrorTable_Icon"), Icon16x16));
	Style->Set("ClassIcon.MirrorTable", new IMAGE_BRUSH(TEXT("MirrorTable_ClassIcon"), Icon20x20));
	Style->Set("ClassThumbnail.MirrorTable", new IMAGE_BRUSH(TEXT("MirrorTable_ClassIcon"), Icon64x64));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
