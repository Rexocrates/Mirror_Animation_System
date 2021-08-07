// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


#include "Toolkits/AssetEditorToolkit.h"
#include "Modules/ModuleInterface.h"

#include "AssetTypeCategories.h"
#include "Engine/Texture2D.h"
#include "Editor.h"
#include "EditorModeRegistry.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"

#include "IAssetTypeActions.h"

#include "ISettingsModule.h"

#include "PropertyEditorDelegates.h"

class FToolBarBuilder;
class FMenuBuilder;


class FMirrorAnimationSystemEditorModule : public IModuleInterface
{
public:
	FMirrorAnimationSystemEditorModule()
		:MirrorTableAssetCategoryBit(EAssetTypeCategories::Misc)
	{
	}
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:


	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);


	TSet< FName > RegisteredClassNames;
	EAssetTypeCategories::Type MirrorTableAssetCategoryBit;
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

};
