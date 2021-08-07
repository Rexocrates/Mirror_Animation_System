// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "ContentBrowserTools.h"
#include "MirrorAnimationSystemEditor.h"


#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "ContentBrowserModule.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EditorStyleSet.h"
#include "IContentBrowserSingleton.h"
#include "MirrorAnimationSystemStyle.h"


#include "MirrorAnimAssetDialog.h"
#include "MirrorTableFromSkeletonDialog.h"


#include "IAssetTools.h"
#include "AssetToolsModule.h"

//////////////////////////////////////////////////////////////////////////
// FContentBrowserSelectedAssetExtensionBase

#define LOCTEXT_NAMESPACE "MirrorAnimationSystem"

//DECLARE_LOG_CATEGORY_EXTERN(LogMirrorCBExtensions, Log, All);
//DEFINE_LOG_CATEGORY(LogMirrorCBExtensions);

FContentBrowserMenuExtender_SelectedAssets ContentBrowserExtenderDelegate;
FDelegateHandle ContentBrowserExtenderDelegateHandle;

//Base struct to be extended and merely hold primordial data
struct FContentBrowserSelectedAssetExtensionBase
{
public:
	TArray<struct FAssetData> SelectedAssets;

public:
	virtual void Execute() {}
	virtual ~FContentBrowserSelectedAssetExtensionBase() {}
};

//Struct that contains the functionality for opening the Dialog of the "Mirror AnimAsset" tool
struct FMirrorAnimAssetExtension : public FContentBrowserSelectedAssetExtensionBase
{
	FMirrorAnimAssetExtension()
	{
	}

	void MirrorAnimAssets(TArray<UAnimSequence*>& Animations)
	{
		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		TArray<UObject*> ObjectsToSync;

		for (auto AnimationIt = Animations.CreateConstIterator(); AnimationIt; ++AnimationIt)
		{
			UAnimSequence* Animation = *AnimationIt;

			SMirrorAnimAssetDialog::ShowWindow(Animation);

		}

		if (ObjectsToSync.Num() > 0)
		{
			ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
		}
	}

	virtual void Execute() override
	{
		// Mirror Animations from selected animassets
		TArray<UAnimSequence*> Animations;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (UAnimSequence* Animation = Cast<UAnimSequence>(AssetData.GetAsset()))
			{
				Animations.Add(Animation);
			}
		}

		MirrorAnimAssets(Animations);
	}
};

//////////////////////////////////////////////////////////////////////////

//Struct that contains the functionality for opening the Dialog of the "Mirror Table from Skeleton" tool
struct FCreateMirrorTableExtension : public FContentBrowserSelectedAssetExtensionBase
{
	FCreateMirrorTableExtension()
	{
	}

	void CreateMirrorTable(TArray<USkeleton*>& Skeletons)
	{
		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		TArray<UObject*> ObjectsToSync;

		for (auto SkeletonIt = Skeletons.CreateConstIterator(); SkeletonIt; ++SkeletonIt)
		{
			USkeleton* Skeleton = *SkeletonIt;

			SMirrorTableFromSkeletonDialog::ShowWindow(Skeleton);

		}

		if (ObjectsToSync.Num() > 0)
		{
			ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
		}
	}

	virtual void Execute() override
	{
		// Create Mirror Table
		TArray<USkeleton*> Skeletons;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (USkeleton* Skeleton = Cast<USkeleton>(AssetData.GetAsset()))
			{
				Skeletons.Add(Skeleton);
			}
		}
		CreateMirrorTable(Skeletons);
	}
}


//////////////////////////////////////////////////////////////////////////
// FContentBrowserToolExtensions_Impl
;
/*class that identifies and constructs the tool's buttons adding the functionality for opening 
the corresponding dialog when clicked*/
class FContentBrowserToolExtensions_Impl
{
public:
	static void ExecuteSelectedContentFunctor(TSharedPtr<FContentBrowserSelectedAssetExtensionBase> SelectedAssetFunctor)
	{
		SelectedAssetFunctor->Execute();
	}

	static void CreateMirrorButton(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		TSharedPtr<FMirrorAnimAssetExtension> AnimationMirrorFunctor = MakeShareable(new FMirrorAnimAssetExtension());
		AnimationMirrorFunctor->SelectedAssets = SelectedAssets;

		FUIAction Action_MirrorAnimAsset(
			FExecuteAction::CreateStatic(&FContentBrowserToolExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(AnimationMirrorFunctor)));

		const FName MyToolEditorStyleSetName = FMirrorAnimationSystemStyle::GetStyleSetName();

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Mirror_Animation_Asset", "Mirror Animation Asset"),
			LOCTEXT("Mirror_This_Animation_Based_On_Mirror_Table", "Mirror this Animation asset based on a Mirror Table"),
			FSlateIcon(MyToolEditorStyleSetName, "Icon.MirrorAnimAsset"),
			Action_MirrorAnimAsset,
			NAME_None,
			EUserInterfaceActionType::Button);
	}

	static void CreateMirrorTableButton(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		TSharedPtr<FCreateMirrorTableExtension> CreateMirrorTableFunctor = MakeShareable(new FCreateMirrorTableExtension());
		CreateMirrorTableFunctor->SelectedAssets = SelectedAssets;

		FUIAction Action_CreateMirrorTable(
			FExecuteAction::CreateStatic(&FContentBrowserToolExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(CreateMirrorTableFunctor)));

		const FName MyToolEditorStyleSetName = FMirrorAnimationSystemStyle::GetStyleSetName();

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Create_Mirror_Table", "Mirror Table From Skeleton"),
			LOCTEXT("Create_Mirror_Table_From_Skeleton", "Creates a Mirror Table template based on this Skeleton's Hierarchy"),
			FSlateIcon(MyToolEditorStyleSetName, "Icon.CreateMirrorTable"),
			Action_CreateMirrorTable,
			NAME_None,
			EUserInterfaceActionType::Button);
	}
	
	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		// Run thru the assets to determine if any meet our criteria
		bool bShowButton = false;
		bool bIsSkeletonAsset = false;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
			bShowButton = bShowButton || (Asset.AssetClass == UAnimSequence::StaticClass()->GetFName()) || (Asset.AssetClass == USkeleton::StaticClass()->GetFName());
			bIsSkeletonAsset = (Asset.AssetClass == USkeleton::StaticClass()->GetFName());
		}

		if (bShowButton)
		{
			if (bIsSkeletonAsset)
			{
				// Add the Mirror AnimAsset sub-menu extender
				Extender->AddMenuExtension(
					"GetAssetActions",
					EExtensionHook::After,
					nullptr,
					FMenuExtensionDelegate::CreateStatic(&FContentBrowserToolExtensions_Impl::CreateMirrorTableButton, SelectedAssets));
			}
			else
			{
				// Add the Mirror AnimAsset sub-menu extender
				Extender->AddMenuExtension(
					"GetAssetActions",
					EExtensionHook::After,
					nullptr,
					FMenuExtensionDelegate::CreateStatic(&FContentBrowserToolExtensions_Impl::CreateMirrorButton, SelectedAssets));
			}
		}

		return Extender;
	}

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetExtenderDelegates()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	}
};

//////////////////////////////////////////////////////////////////////////


void  FContentBrowserTools::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FContentBrowserToolExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu);

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FContentBrowserToolExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}

void  FContentBrowserTools::RemoveHooks()
{
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FContentBrowserToolExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE