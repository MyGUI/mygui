/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "BackwardCompatibilityManager.h"
#include "SettingsManager.h"

template <> tools::BackwardCompatibilityManager* MyGUI::Singleton<tools::BackwardCompatibilityManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::BackwardCompatibilityManager>::mClassTypeName = "BackwardCompatibilityManager";

namespace tools
{

	BackwardCompatibilityManager::BackwardCompatibilityManager()
	{
		mVersions.push_back("3.2.0");
		mVersions.push_back("3.1.0");

		mPropertyNames["3.1.0-StateSelected"] = "Button_Pressed";
		mPropertyNames["3.1.0-ModeImage"] = "Button_ModeImage";
		mPropertyNames["3.1.0-ImageResource"] = "Button_ImageResource";

		mPropertyNames["3.1.0-ModeDrop"] = "Combo_ModeDrop";

		mPropertyNames["3.1.0-NeedDragDrop"] = "DDContainer_NeedDragDrop";

		mPropertyNames["3.1.0-CursorPosition"] = "Edit_CursorPosition";
		mPropertyNames["3.1.0-TextSelect"] = "Edit_TextSelect";
		mPropertyNames["3.1.0-ReadOnly"] = "Edit_ReadOnly";
		mPropertyNames["3.1.0-Password"] = "Edit_Password";
		mPropertyNames["3.1.0-MultiLine"] = "Edit_MultiLine";
		mPropertyNames["3.1.0-PasswordChar"] = "Edit_PasswordChar";
		mPropertyNames["3.1.0-MaxTextLength"] = "Edit_MaxTextLength";
		mPropertyNames["3.1.0-OverflowToTheLeft"] = "Edit_OverflowToTheLeft";
		mPropertyNames["3.1.0-Static"] = "Edit_Static";
		mPropertyNames["3.1.0-WordWrap"] = "Edit_WordWrap";
		mPropertyNames["3.1.0-TabPrinting"] = "Edit_TabPrinting";
		mPropertyNames["3.1.0-InvertSelected"] = "Edit_InvertSelected";

		mPropertyNames["3.1.0-MenuItemId"] = "MenuItem_Id";
		mPropertyNames["3.1.0-MenuItemType"] = "MenuItem_Type";

		mPropertyNames["3.1.0-MessageText"] = "Message_Message";
		mPropertyNames["3.1.0-Modal"] = "Message_Modal";
		mPropertyNames["3.1.0-MessageButton"] = "Message_Button";
		mPropertyNames["3.1.0-SmoothShow"] = "Message_SmoothShow";
		mPropertyNames["3.1.0-Fade"] = "Message_Fade";

		mPropertyNames["3.1.0-Range"] = "Progress_Range";
		mPropertyNames["3.1.0-RangePosition"] = "Progress_Position";
		mPropertyNames["3.1.0-AutoTrack"] = "Progress_AutoTrack";

		mPropertyNames["3.1.0-CanvasAlign"] = "ScrollView_CanvasAlign";
		mPropertyNames["3.1.0-CanvasSize"] = "ScrollView_CanvasSize";

		mPropertyNames["3.1.0-ImageTexture"] = "Image_Texture";
		mPropertyNames["3.1.0-ImageRegion"] = "Image_Region";
		mPropertyNames["3.1.0-ImageTile"] = "Image_Tile";
		mPropertyNames["3.1.0-ImageIndex"] = "Image_Index";
		mPropertyNames["3.1.0-ImageResource"] = "Image_Resource";
		mPropertyNames["3.1.0-ImageGroup"] = "Image_Group";
		mPropertyNames["3.1.0-ImageName"] = "Image_Name";

		mPropertyNames["3.1.0-TextColour"] = "Text_TextColour";
		mPropertyNames["3.1.0-TextAlign"] = "Text_TextAlign";
		mPropertyNames["3.1.0-FontName"] = "Text_FontName";
		mPropertyNames["3.1.0-FontHeight"] = "Text_FontHeight";

		mPropertyNames["3.1.0-ButtonWidth"] = "Tab_ButtonWidth";
		mPropertyNames["3.1.0-ButtonAutoWidth"] = "Tab_ButtonAutoWidth";
		mPropertyNames["3.1.0-SmoothShow"] = "Tab_SmoothShow";
		mPropertyNames["3.1.0-SelectItem"] = "Tab_SelectItem";

		mPropertyNames["3.1.0-ButtonWidth"] = "TabItem_ButtonWidth";

		mPropertyNames["3.1.0-Page"] = "Scroll_Page";
		mPropertyNames["3.1.0-ViewPage"] = "Scroll_ViewPage";
		mPropertyNames["3.1.0-MoveToClick"] = "Scroll_MoveToClick";

		mPropertyNames["3.1.0-Caption"] = "Widget_Caption";
		mPropertyNames["3.1.0-Position"] = "Widget_Position";
		mPropertyNames["3.1.0-Size"] = "Widget_Size";
		mPropertyNames["3.1.0-Coord"] = "Widget_Coord";
		mPropertyNames["3.1.0-Visible"] = "Widget_Visible";
		mPropertyNames["3.1.0-Alpha"] = "Widget_Alpha";
		mPropertyNames["3.1.0-Colour"] = "Widget_Colour";
		mPropertyNames["3.1.0-InheritsAlpha"] = "Widget_InheritsAlpha";
		mPropertyNames["3.1.0-InheritsPick"] = "Widget_InheritsPick";
		mPropertyNames["3.1.0-MaskPick"] = "Widget_MaskPick";
		mPropertyNames["3.1.0-State"] = "Widget_State";
		mPropertyNames["3.1.0-NeedKey"] = "Widget_NeedKey";
		mPropertyNames["3.1.0-NeedMouse"] = "Widget_NeedMouse";
		mPropertyNames["3.1.0-Enabled"] = "Widget_Enabled";
		mPropertyNames["3.1.0-NeedToolTip"] = "Widget_NeedToolTip";
		mPropertyNames["3.1.0-Pointer"] = "Widget_Pointer";

		mPropertyNames["3.1.0-AutoAlpha"] = "Window_AutoAlpha";
		mPropertyNames["3.1.0-Snap"] = "Window_Snap";
		mPropertyNames["3.1.0-MinSize"] = "Window_MinSize";
		mPropertyNames["3.1.0-MaxSize"] = "Window_MaxSize";

		mPropertyNames["3.1.0-VScroll-Range"] = "Scroll_Range";
		mPropertyNames["3.1.0-VScroll-RangePosition"] = "Scroll_Position";
		mPropertyNames["3.1.0-HScroll-Range"] = "Scroll_Range";
		mPropertyNames["3.1.0-HScroll-RangePosition"] = "Scroll_Position";
		mPropertyNames["3.1.0-Progress-Range"] = "Progress_Range";
		mPropertyNames["3.1.0-Progress-RangePosition"] = "Progress_Position";
		mPropertyNames["3.1.0-ProgressBar-Range"] = "Progress_Range";
		mPropertyNames["3.1.0-ProgressBar-RangePosition"] = "Progress_Position";
		mPropertyNames["3.1.0-Edit-VisibleVScroll"] = "Edit_ShowVScroll";
		mPropertyNames["3.1.0-Edit-VisibleHScroll"] = "Edit_ShowHScroll";
		mPropertyNames["3.1.0-EditBox-VisibleVScroll"] = "Edit_ShowVScroll";
		mPropertyNames["3.1.0-EditBox-VisibleHScroll"] = "Edit_ShowHScroll";
		mPropertyNames["3.1.0-Message-Caption"] = "Message_Caption";
		mPropertyNames["3.1.0-ScrollView-VisibleVScroll"] = "ScrollView_VScroll";
		mPropertyNames["3.1.0-ScrollView-VisibleHScroll"] = "ScrollView_HScroll";

		mPropertyNames["3.1.0-List-AddItem"] = "List_AddItem";
		mPropertyNames["3.1.0-ListBox-AddItem"] = "List_AddItem";
		mPropertyNames["3.1.0-ComboBox-AddItem"] = "Combo_AddItem";
	}

	BackwardCompatibilityManager::~BackwardCompatibilityManager()
	{
	}

	void BackwardCompatibilityManager::initialise()
	{
		if (!SettingsManager::getInstance().tryGetValue("Settings/LayoutVersion", mCurrentVersion))
			mCurrentVersion = getDefaultVersion();

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &BackwardCompatibilityManager::notifySettingsChanged);
	}

	void BackwardCompatibilityManager::shutdown()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
		SettingsManager::getInstance().setValue("Settings/LayoutVersion", mCurrentVersion);
	}

	void BackwardCompatibilityManager::serialiseProperty(MyGUI::xml::Element* _node, const std::string& _widgetType, const MyGUI::PairString& _property, bool _compatibility)
	{
		std::string propertyName = _property.first;

		if (_compatibility && getDefaultVersion() != getCurrentVersion())
		{
			std::string key = MyGUI::utility::toString(getCurrentVersion(), "-", propertyName);

			MyGUI::MapString::const_iterator item = mPropertyNames.find(key);
			if (item != mPropertyNames.end())
			{
				propertyName = (*item).second;
			}
			else
			{
				key = MyGUI::utility::toString(getCurrentVersion(), "-", _widgetType, "-", propertyName);

				item = mPropertyNames.find(key);
				if (item != mPropertyNames.end())
				{
					propertyName = (*item).second;
				}
				else
				{
					MYGUI_LOG(Warning, "Property key not found '" << key << "'");
				}
			}
		}

		MyGUI::xml::ElementPtr nodeProp = _node->createChild("Property");
		nodeProp->addAttribute("key", propertyName);
		nodeProp->addAttribute("value", _property.second);
	}

	const MyGUI::VectorString& BackwardCompatibilityManager::getVersions() const
	{
		return mVersions;
	}

	void BackwardCompatibilityManager::notifySettingsChanged(const std::string& _path)
	{
		if (_path == "Settings/LayoutVersion")
			mCurrentVersion = SettingsManager::getInstance().getValue("Settings/LayoutVersion");
	}

	const std::string& BackwardCompatibilityManager::getDefaultVersion() const
	{
		return mVersions.front();
	}

	const std::string& BackwardCompatibilityManager::getCurrentVersion() const
	{
		return mCurrentVersion;
	}

	void BackwardCompatibilityManager::setCurrentVersion(const std::string& _value)
	{
		mCurrentVersion = _value;
	}

}
