// Copyright 2017-2021 Rexocrates. All Rights Reserved.
#include "AssetTypeActions_MirrorTable.h"
#include "MirrorAnimationSystemEditor.h"

#include "MirrorTable.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

//////////////////////////////////////////////////////////////////////////
// FFlipbookAssetTypeActions

FAssetTypeActions_MirrorTable::FAssetTypeActions_MirrorTable(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_MirrorTable::GetName() const
{
	return LOCTEXT("FMirrorTableAssetTypeActionsName", "Mirror Table");
}

FColor FAssetTypeActions_MirrorTable::GetTypeColor() const
{
	return FColor::Blue;
}

UClass* FAssetTypeActions_MirrorTable::GetSupportedClass() const
{
	return UMirrorTable::StaticClass();
}

void FAssetTypeActions_MirrorTable::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UMirrorTable* MirrorTable = Cast<UMirrorTable>(*ObjIt))
		{
			FSimpleAssetEditor::CreateEditor(Mode, Mode == EToolkitMode::WorldCentric ? EditWithinLevelEditor : TSharedPtr<IToolkitHost>(), MirrorTable);
			/*
			TSharedRef<FFlipbookEditor> NewFlipbookEditor(new FFlipbookEditor());
			NewFlipbookEditor->InitFlipbookEditor(Mode, EditWithinLevelEditor, Flipbook);
			*/
		}
	}
}

uint32 FAssetTypeActions_MirrorTable::GetCategories()
{
	return EAssetTypeCategories::Animation | MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE