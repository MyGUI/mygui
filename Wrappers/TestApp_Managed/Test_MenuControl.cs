using System;
using MyGUI.Managed;

namespace TestApp.Managed
{
    public class Test_MenuControl
    {
        public static void Test()
        {
            MenuControl menu = Gui.Instance.CreateWidget<MenuControl>("PopupMenu", new IntCoord(220, 20, 300, 200), Align.Default, "Main");

            menu.EventMenuCtrlClose += new MenuControl.HandleMenuCtrlClose(menu_EventMenuControlClose);
            menu.EventMenuCtrlAccept += new MenuControl.HandleMenuCtrlAccept(menu_EventMenuControlAccept);

            menu.AddItem("line0", MenuItemType.Normal, "id1", "0");
            menu.InsertItemAt(0, "insert line0", MenuItemType.Normal, "id2", "0");
            menu.AddItem("line2", MenuItemType.Normal, "id3", "0");

            MenuItem parent = menu.MenuItemParent;
            menu.PopupAccept = !menu.PopupAccept;
            menu.SetItemTypeAt(0, MenuItemType.Popup);
            MenuItemType type = menu.GetItemTypeAt(0);

            MenuControl child = menu.CreateItemChildAt(0);
            child = menu.GetItemChildAt(0);
            menu.SetItemChildVisibleAt(0, true);

            string name = menu.GetItemNameAt(0);
            menu.SetItemNameAt(0, "new name line0");
            uint index = menu.GetItemIndexById("id1");
            MenuItem item = menu.GetItemById("id2");
            string id = menu.GetItemIdAt(0);

            string data = menu.GetItemDataAt(0) as string;
            menu.ClearItemDataAt(0);
            data = menu.GetItemDataAt(0) as string;
            menu.SetItemDataAt(0, "new data0");
            data = menu.GetItemDataAt(0) as string;

            //MenuItem test start
            child = item.ItemChild;
            MenuControl parent2 = item.MenuCtrlParent;
            item.SetItemChildVisible(true);
            type =  item.ItemType;
            item.ItemType = type;
            index = item.ItemIndex;
            item.ItemId = item.ItemId + "_newid";
            item.ItemName = item.ItemName + "_newname";

            data = item.ItemData as string;
            item.ItemData = "new data";
            data = menu.GetItemData(item) as string;
            data = menu.GetItemDataAt(0) as string;
            data = item.ItemData as string;
            item.ItemData = null;
            data = item.ItemData as string;
            //MenuItem test end

            index = menu.GetItemIndex(item);
            menu.RemoveItem(item);

            uint count = menu.ItemCount;
            menu.SetVisibleSmooth(true);
        }

        static void menu_EventMenuControlAccept(MenuControl _sender, MenuItem _item)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMenuControlAccept  item=" + _item.ToString());
        }

        static void menu_EventMenuControlClose(MenuControl _sender)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMenuControlClose");
        }
    }
}
