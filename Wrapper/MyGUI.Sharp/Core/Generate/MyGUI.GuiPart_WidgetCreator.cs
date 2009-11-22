using System;

namespace MyGUI.Sharp
{

    public partial class Gui
    {
        static void InitialiseWidgetCreator()
        {
			//InsertPoint

   			mMapWrapper.Add("TabItem", TabItem.RequestWrapTabItem);
			mMapCreator.Add("TabItem", TabItem.RequestCreateTabItem);


   			mMapWrapper.Add("MenuItem", MenuItem.RequestWrapMenuItem);
			mMapCreator.Add("MenuItem", MenuItem.RequestCreateMenuItem);


   			mMapWrapper.Add("Window", Window.RequestWrapWindow);
			mMapCreator.Add("Window", Window.RequestCreateWindow);


   			mMapWrapper.Add("VScroll", VScrollBar.RequestWrapVScrollBar);
			mMapCreator.Add("VScroll", VScrollBar.RequestCreateVScrollBar);


   			mMapWrapper.Add("Tab", TabBar.RequestWrapTabBar);
			mMapCreator.Add("Tab", TabBar.RequestCreateTabBar);


   			mMapWrapper.Add("StaticText", StaticText.RequestWrapStaticText);
			mMapCreator.Add("StaticText", StaticText.RequestCreateStaticText);


   			mMapWrapper.Add("StaticImage", StaticImage.RequestWrapStaticImage);
			mMapCreator.Add("StaticImage", StaticImage.RequestCreateStaticImage);


   			mMapWrapper.Add("ScrollView", ScrollView.RequestWrapScrollView);
			mMapCreator.Add("ScrollView", ScrollView.RequestCreateScrollView);


   			mMapWrapper.Add("Progress", ProgressBar.RequestWrapProgressBar);
			mMapCreator.Add("Progress", ProgressBar.RequestCreateProgressBar);


   			mMapWrapper.Add("PopupMenu", PopupMenu.RequestWrapPopupMenu);
			mMapCreator.Add("PopupMenu", PopupMenu.RequestCreatePopupMenu);


   			mMapWrapper.Add("MultiList", MultiListBox.RequestWrapMultiListBox);
			mMapCreator.Add("MultiList", MultiListBox.RequestCreateMultiListBox);


   			mMapWrapper.Add("Message", MessageBox.RequestWrapMessageBox);
			mMapCreator.Add("Message", MessageBox.RequestCreateMessageBox);


   			mMapWrapper.Add("MenuCtrl", MenuCtrl.RequestWrapMenuCtrl);
			mMapCreator.Add("MenuCtrl", MenuCtrl.RequestCreateMenuCtrl);


   			mMapWrapper.Add("MenuBar", MenuBar.RequestWrapMenuBar);
			mMapCreator.Add("MenuBar", MenuBar.RequestCreateMenuBar);


   			mMapWrapper.Add("List", ListBox.RequestWrapListBox);
			mMapCreator.Add("List", ListBox.RequestCreateListBox);


   			mMapWrapper.Add("ItemBox", ItemBox.RequestWrapItemBox);
			mMapCreator.Add("ItemBox", ItemBox.RequestCreateItemBox);


   			mMapWrapper.Add("HScroll", HScrollBar.RequestWrapHScrollBar);
			mMapCreator.Add("HScroll", HScrollBar.RequestCreateHScrollBar);


   			mMapWrapper.Add("Edit", EditBox.RequestWrapEditBox);
			mMapCreator.Add("Edit", EditBox.RequestCreateEditBox);


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
