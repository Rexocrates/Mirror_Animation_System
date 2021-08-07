// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MirrorAnimationSystemEditor.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "LevelEditor.h"
#include "ContentBrowserTools.h"
#include "MirrorAnimationSystemStyle.h"

#include "AssetToolsModule.h"
#include "MirrorTableDetails.h"
#include "AssetTypeActions_MirrorTable.h"


static const FName MirrorAnimationSystemEditorTabName("MirrorAnimationSystemEditor");


#define LOCTEXT_NAMESPACE "FMirrorAnimationSystemEditorModule"

void FMirrorAnimationSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FMirrorAnimationSystemStyle::Initialize();


	if (!IsRunningCommandlet())
	{
		FContentBrowserTools::InstallHooks();
	}



	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_MirrorTable(MirrorTableAssetCategoryBit)));

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RegisterCustomClassLayout("MirrorTable", FOnGetDetailCustomizationInstance::CreateStatic(&FMirrorTableDetails::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();

}

void FMirrorAnimationSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (!IsRunningCommandlet())
	{
		FContentBrowserTools::RemoveHooks();
	}
	FMirrorAnimationSystemStyle::Shutdown();




	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		// Unregister all classes customized by name
		for (auto It = RegisteredClassNames.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*It);
			}
		}
		PropertyModule.NotifyCustomizationModuleChanged();
	}

}


void FMirrorAnimationSystemEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

void FMirrorAnimationSystemEditorModule::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	check(ClassName != NAME_None);

	RegisteredClassNames.Add(ClassName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMirrorAnimationSystemEditorModule, MirrorAnimationSystemEditor)