// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "MirrorTableFromSkeletonDialog.h"
#include "MirrorAnimationSystemEditor.h"



#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "Misc/FeedbackContext.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "Layout/WidgetPath.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Framework/Docking/TabManager.h"
#include "EditorStyleSet.h"
#include "CanvasItem.h"
#include "AssetRegistryModule.h"
#include "PropertyEditorModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "CanvasTypes.h"

#include "Factories/DataAssetFactory.h"

#define LOCTEXT_NAMESPACE "MirrorAnimationSystemlEditor"

void SMirrorTableFromSkeletonDialog::Construct(const FArguments & InArgs, USkeleton * Skeleton)
{
	SourceSkeleton = Skeleton;

	MirrorTableSettings = NewObject<UMirrorTableFromSkeletonSettings>();

	MirrorTableSettings->AddToRoot();

	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(/*bUpdateFromSelection=*/ false, /*bLockable=*/ false, /*bAllowSearch=*/ false, /*InNameAreaSettings=*/ FDetailsViewArgs::HideNameArea, /*bHideSelectionTip=*/ true);
	MainPropertyView = EditModule.CreateDetailView(DetailsViewArgs);
	MainPropertyView->SetObject(MirrorTableSettings);




	ChildSlot
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("DetailsView.CategoryTop"))
		.Padding(FMargin(1.0f, 1.0f, 1.0f, 0.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.Padding(1.0f)
		.AutoHeight()
		[
			MainPropertyView.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.Padding(1.0f)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		[
			SNew(SUniformGridPanel)
			.SlotPadding(1)
		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
		.ForegroundColor(FLinearColor::White)
		.Text(LOCTEXT("PaperExtractSpritesExtractButton", "Create"))
		.OnClicked(this, &SMirrorTableFromSkeletonDialog::CreateClicked)
		]
	+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
		.ForegroundColor(FLinearColor::White)
		.Text(LOCTEXT("PaperExtractSpritesCancelButton", "Cancel"))
		.OnClicked(this, &SMirrorTableFromSkeletonDialog::CancelClicked)
		]
		]
		]
		]
		];
}

SMirrorTableFromSkeletonDialog::~SMirrorTableFromSkeletonDialog()
{
}

bool SMirrorTableFromSkeletonDialog::ShowWindow(USkeleton * SourceSkeleton)
{

	const FText TitleText = NSLOCTEXT("MirrorAnimationSystem", "Create_Mirror_Table_From_Skeleton", "Create Mirror Table From Skeleton");
	// Create the window to pick the class
	TSharedRef<SWindow> MirrorAnimSequenceWindow = SNew(SWindow)
		.Title(TitleText)
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2D(550.f, 375.f))
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.SupportsMinimize(false);

	TSharedRef<SMirrorTableFromSkeletonDialog> MirrorAnimSequenceDialog = SNew(SMirrorTableFromSkeletonDialog, SourceSkeleton);

	MirrorAnimSequenceWindow->SetContent(MirrorAnimSequenceDialog);
	TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(MirrorAnimSequenceWindow, RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(MirrorAnimSequenceWindow);
	}

	return false;
}

FReply SMirrorTableFromSkeletonDialog::CreateClicked()
{
	if (
		(MirrorTableSettings->Substring_A != "none")
		&& (!MirrorTableSettings->Substring_A.IsEmpty())
		&& (MirrorTableSettings->Substring_B != "none")
		&& (!MirrorTableSettings->Substring_B.IsEmpty())
		)
	{
		CreateMirrorTableFromSkeletonFunction();

		CloseContainingWindow();
	}
	else
	{
		FText DialogText = FText::AsCultureInvariant(FString(TEXT("Enter valid Substrings for correctly identifying twin bones")));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	}

	return FReply::Handled();
}

FReply SMirrorTableFromSkeletonDialog::CancelClicked()
{
	CloseContainingWindow();
	return FReply::Handled();
}

void SMirrorTableFromSkeletonDialog::CloseContainingWindow()
{
	TSharedPtr<SWindow> ContainingWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ContainingWindow.IsValid())
	{
		ContainingWindow->RequestDestroyWindow();
	}
}

void SMirrorTableFromSkeletonDialog::CreateMirrorTableFromSkeletonFunction()
{

	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<UObject*> ObjectsToSync;

	UDataAssetFactory* DataFactory = NewObject<UDataAssetFactory>();

	FString Name;
	FString PackageName;


	FString Suffix = TEXT("_MirrorTable");

	//////////


	AssetToolsModule.Get().CreateUniqueAssetName(SourceSkeleton->GetOutermost()->GetName(), Suffix, /*out*/ PackageName, /*out*/ Name);
	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

	if (UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, UMirrorTable::StaticClass(), DataFactory))
	{
		UMirrorTable* MirrorTable = Cast<UMirrorTable>(NewAsset);
		SetMirrorTableFromSettings(MirrorTable);

		ObjectsToSync.Add(NewAsset);

	}
	if (ObjectsToSync.Num() > 0)
	{
		ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
	}
}

void SMirrorTableFromSkeletonDialog::SetMirrorTableFromSettings(UMirrorTable * MirrorTable)
{
	if (MirrorTable != NULL)
	{
			int NumBones = SourceSkeleton->GetReferenceSkeleton().GetNum();
			TArray <FString> CreatedBoneStrings;
			for (int i = 0; i < NumBones; i++)
			{
				FString CurrentBoneString = SourceSkeleton->GetReferenceSkeleton().GetBoneName(i).ToString();

				int ResultA = CurrentBoneString.Find(MirrorTableSettings->Substring_A, ESearchCase::IgnoreCase, ESearchDir::FromEnd, -1);
				int ResultB = CurrentBoneString.Find(MirrorTableSettings->Substring_B, ESearchCase::IgnoreCase, ESearchDir::FromEnd, -1);

				if ((ResultA != -1) || (ResultB != -1))
				{
					FString TwinBoneString;
					bool ValidSymmetry = true;
					if (ResultB != -1)
					{

						TwinBoneString = CurrentBoneString.Mid(0, ResultB);
						TwinBoneString += MirrorTableSettings->Substring_A;
						TwinBoneString += CurrentBoneString.Mid(ResultB + MirrorTableSettings->Substring_B.Len());

						if ((!(abs(TwinBoneString.Len() - MirrorTableSettings->Substring_A.Len()) == abs(CurrentBoneString.Len() - MirrorTableSettings->Substring_B.Len()))) && ValidSymmetry)
						{
							ValidSymmetry = false;
						}
					}
					else
					{

						TwinBoneString = CurrentBoneString.Mid(0, ResultA);
						TwinBoneString += MirrorTableSettings->Substring_B;
						TwinBoneString += CurrentBoneString.Mid(ResultA + MirrorTableSettings->Substring_A.Len());

						if ((!(abs(TwinBoneString.Len() - MirrorTableSettings->Substring_B.Len()) == abs(CurrentBoneString.Len() - MirrorTableSettings->Substring_A.Len()))) && ValidSymmetry)
						{
							ValidSymmetry = false;
						}
					}
					if (ValidSymmetry)
					{

						FName TwinBoneName = FName(*TwinBoneString);
						int TwinBoneIndex = SourceSkeleton ? SourceSkeleton->GetReferenceSkeleton().FindBoneIndex(FName(*TwinBoneString)) : INDEX_NONE;
						if (TwinBoneIndex != INDEX_NONE)
						{
							if ((!CreatedBoneStrings.Contains(TwinBoneString)) && (!CreatedBoneStrings.Contains(CurrentBoneString)))
							{
								FMirrorBone NewMirrorBone = FMirrorBone();
								NewMirrorBone.BoneName = FName(*CurrentBoneString);
								NewMirrorBone.MirrorAxis = MirrorTableSettings->DefaultTwinMirrorAxis;
								NewMirrorBone.FlipAxis = MirrorTableSettings->DefaultTwinFlipAxis;
								NewMirrorBone.RotationOffset = MirrorTableSettings->DefaultTwinRotationOffset;
								NewMirrorBone.IsTwinBone = true;
								NewMirrorBone.TwinBoneName = FName(*TwinBoneString);
								NewMirrorBone.MirrorTranslation = MirrorTableSettings->DefaultTwinMirrorTranslation;

								MirrorTable->MirrorBones.Add(NewMirrorBone);
								CreatedBoneStrings.Add(TwinBoneString);
								CreatedBoneStrings.Add(CurrentBoneString);
								continue;
							}

						}
					}
				}
				if (!CreatedBoneStrings.Contains(CurrentBoneString))
				{
					/////
					FMirrorBone NewMirrorBone = FMirrorBone();
					NewMirrorBone.BoneName = FName(*CurrentBoneString);
					NewMirrorBone.MirrorAxis = MirrorTableSettings->DefaultMirrorAxis;
					NewMirrorBone.FlipAxis = MirrorTableSettings->DefaultFlipAxis;
					NewMirrorBone.RotationOffset = MirrorTableSettings->DefaultRotationOffset;
					NewMirrorBone.IsTwinBone = false;

					MirrorTable->MirrorBones.Add(NewMirrorBone);
					CreatedBoneStrings.Add(CurrentBoneString);
				}
			}
	}
}

#undef LOCTEXT_NAMESPACE