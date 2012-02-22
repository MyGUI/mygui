using System;

namespace MyGUI.Sharp
{

	public partial class Gui
	{
		void InitialiseWidgetCreator()
		{
			//InsertPoint

   			mMapWrapper.Add("MultiListItem", MultiListItem.RequestWrapMultiListItem);
			mMapCreator.Add("MultiListItem", MultiListItem.RequestCreateMultiListItem);


   			mMapWrapper.Add("TabItem", TabItem.RequestWrapTabItem);
			mMapCreator.Add("TabItem", TabItem.RequestCreateTabItem);


   			mMapWrapper.Add("MenuItem", MenuItem.RequestWrapMenuItem);
			mMapCreator.Add("MenuItem", MenuItem.RequestCreateMenuItem);


   			mMapWrapper.Add("Window", Window.RequestWrapWindow);
			mMapCreator.Add("Window", Window.RequestCreateWindow);


   			mMapWrapper.Add("ScrollBar", ScrollBar.RequestWrapScrollBar);
			mMapCreator.Add("ScrollBar", ScrollBar.RequestCreateScrollBar);


   			mMapWrapper.Add("TabControl", TabControl.RequestWrapTabControl);
			mMapCreator.Add("TabControl", TabControl.RequestCreateTabControl);


   			mMapWrapper.Add("TextBox", TextBox.RequestWrapTextBox);
			mMapCreator.Add("TextBox", TextBox.RequestCreateTextBox);


   			mMapWrapper.Add("ImageBox", ImageBox.RequestWrapImageBox);
			mMapCreator.Add("ImageBox", ImageBox.RequestCreateImageBox);


   			mMapWrapper.Add("ScrollView", ScrollView.RequestWrapScrollView);
			mMapCreator.Add("ScrollView", ScrollView.RequestCreateScrollView);


   			mMapWrapper.Add("ProgressBar", ProgressBar.RequestWrapProgressBar);
			mMapCreator.Add("ProgressBar", ProgressBar.RequestCreateProgressBar);


   			mMapWrapper.Add("PopupMenu", PopupMenu.RequestWrapPopupMenu);
			mMapCreator.Add("PopupMenu", PopupMenu.RequestCreatePopupMenu);


   			mMapWrapper.Add("MultiListBox", MultiListBox.RequestWrapMultiListBox);
			mMapCreator.Add("MultiListBox", MultiListBox.RequestCreateMultiListBox);


   			mMapWrapper.Add("MenuControl", MenuControl.RequestWrapMenuControl);
			mMapCreator.Add("MenuControl", MenuControl.RequestCreateMenuControl);


   			mMapWrapper.Add("MenuBar", MenuBar.RequestWrapMenuBar);
			mMapCreator.Add("MenuBar", MenuBar.RequestCreateMenuBar);


   			mMapWrapper.Add("ListBox", ListBox.RequestWrapListBox);
			mMapCreator.Add("ListBox", ListBox.RequestCreateListBox);


   			mMapWrapper.Add("ItemBox", ItemBox.RequestWrapItemBox);
			mMapCreator.Add("ItemBox", ItemBox.RequestCreateItemBox);


   			mMapWrapper.Add("EditBox", EditBox.RequestWrapEditBox);
			mMapCreator.Add("EditBox", EditBox.RequestCreateEditBox);


   			mMapWrapper.Add("DDContainer", DDContainer.RequestWrapDDContainer);
			mMapCreator.Add("DDContainer", DDContainer.RequestCreateDDContainer);


   			mMapWrapper.Add("ComboBox", ComboBox.RequestWrapComboBox);
			mMapCreator.Add("ComboBox", ComboBox.RequestCreateComboBox);


   			mMapWrapper.Add("Button", Button.RequestWrapButton);
			mMapCreator.Add("Button", Button.RequestCreateButton);


   			mMapWrapper.Add("Widget", Widget.RequestWrapWidget);
			mMapCreator.Add("Widget", Widget.RequestCreateWidget);

		}
	}
	
} // namespace MyGUI.Sharp
