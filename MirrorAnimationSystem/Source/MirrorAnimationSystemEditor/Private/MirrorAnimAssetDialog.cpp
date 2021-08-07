// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "MirrorAnimAssetDialog.h"
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

#include "PropertyEditorModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "CanvasTypes.h"

#include "Factories/AnimSequenceFactory.h"

#include "MASFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "MirrorAnimationSystemEditor"

void SMirrorAnimAssetDialog::Construct(const FArguments & InArgs, UAnimSequence * AnimSequence)
{
	SourceAnimSequence = AnimSequence;

	MirrorAnimAssetSettings = NewObject<UMirrorAnimAssetSettings>();
	MirrorAnimAssetSettings->AddToRoot();

	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(/*bUpdateFromSelection=*/ false, /*bLockable=*/ false, /*bAllowSearch=*/ false, /*InNameAreaSettings=*/ FDetailsViewArgs::HideNameArea, /*bHideSelectionTip=*/ true);
	MainPropertyView = EditModule.CreateDetailView(DetailsViewArgs);
	MainPropertyView->SetObject(MirrorAnimAssetSettings);




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
		.Text(LOCTEXT("PaperExtractSpritesExtractButton", "Mirror"))
		.OnClicked(this, &SMirrorAnimAssetDialog::MirrorClicked)
		]
	+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
		.ForegroundColor(FLinearColor::White)
		.Text(LOCTEXT("PaperExtractSpritesCancelButton", "Cancel"))
		.OnClicked(this, &SMirrorAnimAssetDialog::CancelClicked)
		]
		]
		]
		]
		];
}

SMirrorAnimAssetDialog::~SMirrorAnimAssetDialog()
{
}

bool SMirrorAnimAssetDialog::ShowWindow(UAnimSequence * SourceAnimSequence)
{

	const FText TitleText = NSLOCTEXT("MirrorAnimationSystem", "MirrorAnimationSystem_MirrorAnimSequence", "Mirror AnimSequence");
	// Create the window to pick the class
	TSharedRef<SWindow> MirrorAnimSequenceWindow = SNew(SWindow)
		.Title(TitleText)
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2D(720.f, 720.f))
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.SupportsMinimize(false);

	TSharedRef<SMirrorAnimAssetDialog> MirrorAnimSequenceDialog = SNew(SMirrorAnimAssetDialog, SourceAnimSequence);

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

bool SMirrorAnimAssetDialog::ValidateCSMirrorData()
{
	if (MirrorAnimAssetSettings->MirrorTable)
	{
		return true;
	}

	if (MirrorAnimAssetSettings->MirrorAxis == EAxis::None)
	{
		FText DialogText = FText::AsCultureInvariant(FString(TEXT("Mirror Axis Cannot be None")));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return false;
	}

	if ((MirrorAnimAssetSettings->Substring_A == "None") 
		|| 
		(MirrorAnimAssetSettings->Substring_B == "None"))
	{
		FText DialogText = FText::AsCultureInvariant(FString(TEXT("Invalid Substring value")));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return false;
	}

	return true;
}

FReply SMirrorAnimAssetDialog::MirrorClicked()
{
	if (ValidateCSMirrorData())
	{
		CreateMirroredAnimSequences();

		CloseContainingWindow();
	}

	return FReply::Handled();
}

FReply SMirrorAnimAssetDialog::CancelClicked()
{
	CloseContainingWindow();
	return FReply::Handled();
}

void SMirrorAnimAssetDialog::CloseContainingWindow()
{
	TSharedPtr<SWindow> ContainingWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ContainingWindow.IsValid())
	{
		ContainingWindow->RequestDestroyWindow();
	}
}

void SMirrorAnimAssetDialog::CreateMirroredAnimSequences()
{

	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<UObject*> ObjectsToSync;

	// Create the asset
	FString Name;
	FString PackageName;


	FString Suffix = TEXT("_Mirrored");

	AssetToolsModule.Get().CreateUniqueAssetName(SourceAnimSequence->GetOutermost()->GetName(), Suffix, /*out*/ PackageName, /*out*/ Name);
	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);


	UObject* NewAsset = AssetToolsModule.Get().DuplicateAsset(Name, PackagePath, SourceAnimSequence);

	if (NewAsset != NULL)
	{

		UAnimSequence* MirrorAnimSequence = Cast<UAnimSequence>(NewAsset);

		if (MirrorAnimAssetSettings->MirrorTable != NULL)
		{
			UMASFunctionLibrary::CreateMirrorSequenceFromAnimSequence(MirrorAnimSequence, MirrorAnimAssetSettings->MirrorTable);
		}
		else
		{
			UMASFunctionLibrary::CreateMirrorSequenceFromAnimSequence_CS(MirrorAnimSequence,
				MirrorAnimAssetSettings->MirrorAxis,
				MirrorAnimAssetSettings->Substring_A,
				MirrorAnimAssetSettings->Substring_B,
				MirrorAnimAssetSettings->CompletlySymmetrical);
		}

		ObjectsToSync.Add(NewAsset);
	}
	if (ObjectsToSync.Num() > 0)
	{
		ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
	}
}

#undef LOCTEXT_NAMESPACE