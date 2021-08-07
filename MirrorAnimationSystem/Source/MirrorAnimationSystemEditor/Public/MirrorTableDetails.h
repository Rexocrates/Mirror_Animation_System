// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "IDetailCustomization.h"
#include "IDetailCustomNodeBuilder.h"

class FDetailWidgetRow;
class IDetailChildrenBuilder;
class IDetailLayoutBuilder;
class IPropertyHandle;
class SEditableTextBox;
/*class containing the custom layout for the Mirror Table class.
It merely displays the data to setup inside the Mirror Table in a more user friendly way, 
where it is easy to identify the bone's name and whether it is a twin bone or not*/
class FMirrorTableNodeBuilder : public IDetailCustomNodeBuilder, public TSharedFromThis<FMirrorTableNodeBuilder>
{
public:
	FMirrorTableNodeBuilder(IDetailLayoutBuilder* InDetailLayoutBuilder, const TSharedPtr<IPropertyHandle>& InPropertyHandle);

	/** IDetailCustomNodeBuilder interface */
	virtual void SetOnRebuildChildren(FSimpleDelegate InOnRebuildChildren) override { OnRebuildChildren = InOnRebuildChildren; }
	virtual bool RequiresTick() const override { return true; }
	virtual void Tick(float DeltaTime) override;
	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override;
	virtual void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override;
	virtual bool InitiallyCollapsed() const override { return true; }
	virtual FName GetName() const override { return NAME_None; }
	virtual FText GetText() const;
private:
	void RemoveContextButton_OnClick();

private:
	/** Called to rebuild the children of the detail tree */
	FSimpleDelegate OnRebuildChildren;

	/** Associated detail layout builder */
	IDetailLayoutBuilder* DetailLayoutBuilder;

	/** Property handle to associated context mapping */
	TSharedPtr<IPropertyHandle> ContextMappingPropertyHandle;

	/** Property handle to the localization key format property within this context mapping */
	TSharedPtr<IPropertyHandle> BoneNameHandle;


	/** Property handle to the localization key format property within this context mapping */
	TSharedPtr<IPropertyHandle> TwinBoneNameHandle;

	/** Property handle to the localization key format property within this context mapping */
	TSharedPtr<IPropertyHandle> IsTwinBoneHandle;
};

class FMirrorTableDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** ILayoutDetails interface */
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;

	void RebuildChildren();
private:
	FReply AddMirrorTableMapping_OnClicked();
	

private:
	/** Associated detail layout builder */
	IDetailLayoutBuilder* DetailLayoutBuilder;
};
