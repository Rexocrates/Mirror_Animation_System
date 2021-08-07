// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "MirrorTableDetails.h"
#include "MirrorAnimationSystemEditor.h"


#include "Widgets/SWidget.h"
#include "Layout/Margin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "EditorStyleSet.h"
#include "IDetailChildrenBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"

#include "Misc/MessageDialog.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailsView.h"
#include "PropertyCustomizationHelpers.h"

#include "MirrorTable.h"
//#include "DialogueWaveWidgets.h"

#define LOCTEXT_NAMESPACE "MirrorTableDetails"

FMirrorTableNodeBuilder::FMirrorTableNodeBuilder(IDetailLayoutBuilder* InDetailLayoutBuilder, const TSharedPtr<IPropertyHandle>& InPropertyHandle)
	: DetailLayoutBuilder(InDetailLayoutBuilder)
	, ContextMappingPropertyHandle(InPropertyHandle)
	, BoneNameHandle(ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, BoneName)))
	, TwinBoneNameHandle(ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, TwinBoneName)))
	, IsTwinBoneHandle(ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, IsTwinBone)))
{
	//check(BoneNameHandle.IsValid());
	//check(TwinBoneNameHandle.IsValid());
//	check(IsTwinBoneHandle.IsValid());
}

void FMirrorTableNodeBuilder::GenerateHeaderRowContent(FDetailWidgetRow& NodeRow)
{
	if (ContextMappingPropertyHandle->IsValidHandle())
	{
		const TSharedPtr<IPropertyHandle> ContextPropertyHandle = ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, MirrorAxis));
		if (ContextPropertyHandle->IsValidHandle())//if (BoneNameHandle->IsValidHandle())
		{
			const TSharedPtr<IPropertyHandle> FlipAxisPropertyHandle = ContextPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, FlipAxis));
			const TSharedPtr<IPropertyHandle> RotationOffsetPropertyHandle = ContextPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, RotationOffset));
			const TSharedPtr<IPropertyHandle> BoneNamePropertyHandle = ContextPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, BoneName));
			const TSharedPtr<IPropertyHandle> ParentHandle = ContextMappingPropertyHandle->GetParentHandle();
			const TSharedPtr<IPropertyHandleArray> ParentArrayHandle = ParentHandle->AsArray();


			uint32 ContextCount;
			ParentArrayHandle->GetNumElements(ContextCount);

			TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeDeleteButton(FSimpleDelegate::CreateSP(this, &FMirrorTableNodeBuilder::RemoveContextButton_OnClick),
				ContextCount > 1 ? LOCTEXT("RemoveBoneToolTip", "Remove Bone.") : LOCTEXT("RemoveBoneDisabledToolTip", "Cannot remove Bone - a mirror table must have at least one bone."),
				ContextCount > 1);

			NodeRow
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("DialogueWaveDetails.HeaderBorder"))
				[
					//SNew(SDialogueContextHeaderWidget, ContextPropertyHandle.ToSharedRef(), DetailLayoutBuilder->GetThumbnailPool().ToSharedRef())
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					// Voice Description
					SNew(STextBlock)
					.Text(this, &FMirrorTableNodeBuilder::GetText)
				]
				]
				]
			+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					ClearButton
				]
				];
		}
	}
}

void FMirrorTableNodeBuilder::Tick(float DeltaTime)
{

}

void FMirrorTableNodeBuilder::GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder)
{
	if (ContextMappingPropertyHandle->IsValidHandle())
	{
		ChildrenBuilder.AddProperty(BoneNameHandle.ToSharedRef());

		const TSharedPtr<IPropertyHandle> MirrorAxisPropertyHandle = ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, MirrorAxis));
		ChildrenBuilder.AddProperty(MirrorAxisPropertyHandle.ToSharedRef());

		const TSharedPtr<IPropertyHandle> FlipAxisPropertyHandle = ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, FlipAxis));
		ChildrenBuilder.AddProperty(FlipAxisPropertyHandle.ToSharedRef());

		const TSharedPtr<IPropertyHandle> RotationOffsetPropertyHandle = ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, RotationOffset));
		ChildrenBuilder.AddProperty(RotationOffsetPropertyHandle.ToSharedRef());

		ChildrenBuilder.AddProperty(IsTwinBoneHandle.ToSharedRef());

		ChildrenBuilder.AddProperty(TwinBoneNameHandle.ToSharedRef());

		const TSharedPtr<IPropertyHandle> MirrorTranslationPropertyHandle = ContextMappingPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMirrorBone, MirrorTranslation));
		ChildrenBuilder.AddProperty(MirrorTranslationPropertyHandle.ToSharedRef());
	
		
	}
}

FText FMirrorTableNodeBuilder::GetText() const
{
	FString ValueStr;
	FText RowText = FText::FromString(TEXT("Basic"));
	if (BoneNameHandle->GetValue(ValueStr) == FPropertyAccess::Success)
	{
		bool ValueBl;
		RowText = FText::FromString(TEXT("BoneNameHandle"));
		if (IsTwinBoneHandle->GetValue(ValueBl) == FPropertyAccess::Success)
		{
			RowText = FText::FromString(TEXT("IsTwinBoneHandle"));
			if (ValueBl)
			{
				RowText = FText::FromString(TEXT("ValueBl"));
				FString AddedStr;
				if (TwinBoneNameHandle->GetValue(AddedStr) == FPropertyAccess::Success)
				{
					ValueStr += ("   /   " + AddedStr);
				}
			}
		}
		RowText = FText::FromString(MoveTemp(ValueStr));
	}
	return RowText;
}

void FMirrorTableNodeBuilder::RemoveContextButton_OnClick()
{
	if (ContextMappingPropertyHandle->IsValidHandle())
	{
		const TSharedPtr<IPropertyHandle> ParentHandle = ContextMappingPropertyHandle->GetParentHandle();
		const TSharedPtr<IPropertyHandleArray> ParentArrayHandle = ParentHandle->AsArray();

		uint32 ContextCount;
		ParentArrayHandle->GetNumElements(ContextCount);
		if (ContextCount != 1) // Mustn't remove the only context.
		{
			ParentArrayHandle->DeleteItem(ContextMappingPropertyHandle->GetIndexInArray());
		}
	}
}

TSharedRef<IDetailCustomization> FMirrorTableDetails::MakeInstance()
{
	return MakeShareable(new FMirrorTableDetails);
}

void FMirrorTableDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
	DetailLayoutBuilder = &DetailBuilder;

	IDetailCategoryBuilder& MirrorBonesDetailCategoryBuilder = DetailBuilder.EditCategory("MirrorBones", FText::GetEmpty(), ECategoryPriority::Important);

	// Add Context Button
	MirrorBonesDetailCategoryBuilder.AddCustomRow(LOCTEXT("AddBoneToMirrorTable", "Add Bone to Mirror Table"))
		[
			SNew(SBox)
			.Padding(2.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SButton)
			.Text(LOCTEXT("AddBoneToMirrorTable", "Add Bone to Mirror Table"))
		.ToolTipText(LOCTEXT("AddMirrorBoneToolTip", "Adds a new Bone to the Mirror table."))
		.OnClicked(FOnClicked::CreateSP(this, &FMirrorTableDetails::AddMirrorTableMapping_OnClicked))
		]
		];

	// Individual Context Mappings
	const TSharedPtr<IPropertyHandle> MirrorBonesPropertyHandle = DetailLayoutBuilder->GetProperty(GET_MEMBER_NAME_CHECKED(UMirrorTable, MirrorBones), UMirrorTable::StaticClass());
	MirrorBonesPropertyHandle->MarkHiddenByCustomization();

	const TSharedPtr<IPropertyHandleArray> MirrorBonesPropertyArrayHandle = MirrorBonesPropertyHandle->AsArray();

	uint32 DialogueContextMappingCount;
	MirrorBonesPropertyArrayHandle->GetNumElements(DialogueContextMappingCount);

	FSimpleDelegate RebuildChildrenDelegate = FSimpleDelegate::CreateRaw(this, &FMirrorTableDetails::RebuildChildren);
	//MirrorBonesPropertyArrayHandle->SetSetOnPropertyValueChanged(RebuildChildrenDelegate);
	MirrorBonesPropertyArrayHandle->SetOnNumElementsChanged(RebuildChildrenDelegate);
	
	for (uint32 j = 0; j < DialogueContextMappingCount; ++j)
	{
		const TSharedPtr<IPropertyHandle> ChildContextMappingPropertyHandle = MirrorBonesPropertyArrayHandle->GetElement(j);

		const TSharedRef<FMirrorTableNodeBuilder> DialogueContextMapping = MakeShareable(new FMirrorTableNodeBuilder(DetailLayoutBuilder, ChildContextMappingPropertyHandle));
		MirrorBonesDetailCategoryBuilder.AddCustomBuilder(DialogueContextMapping);
	}
}

void FMirrorTableDetails::RebuildChildren()
{
	const TSharedPtr<IPropertyHandle> MirrorBonesPropertyHandle = DetailLayoutBuilder->GetProperty(GET_MEMBER_NAME_CHECKED(UMirrorTable, MirrorBones), UMirrorTable::StaticClass());
	const TSharedPtr<IPropertyHandleArray> MirrorBonesPropertyArrayHandle = MirrorBonesPropertyHandle->AsArray();

	DetailLayoutBuilder->ForceRefreshDetails();
}



FReply FMirrorTableDetails::AddMirrorTableMapping_OnClicked()
{
	const TSharedPtr<IPropertyHandle> MirrorBonesPropertyHandle = DetailLayoutBuilder->GetProperty(GET_MEMBER_NAME_CHECKED(UMirrorTable, MirrorBones), UMirrorTable::StaticClass());
	const TSharedPtr<IPropertyHandleArray> MirrorBonesPropertyArrayHandle = MirrorBonesPropertyHandle->AsArray();
	MirrorBonesPropertyArrayHandle->AddItem();

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
