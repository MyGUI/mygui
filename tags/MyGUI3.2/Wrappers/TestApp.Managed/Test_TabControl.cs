using System;
using MyGUI.Managed;
using MyGUI.Managed.Demo;

namespace TestApp.Sharp
{
    public class Test_TabControl
    {
        public static void Test()
        {
            TabControl tab = Gui.Instance.CreateWidget<TabControl>("TabControl", new IntCoord(120, 420, 200, 100), Align.Default, "Main");
            tab.EventTabChangeSelect += new TabControl.HandleTabChangeSelect(tab_EventTabChangeSelect);

            TabItem item = tab.AddItem("sheet1");
            tab.AddItem("sheet2", "2");
            tab.InsertItemAt(0, "insert sheet1");
            tab.InsertItemAt(0, "insert sheet2", "2");

            tab.SmoothShow = !tab.SmoothShow;
            tab.ButtonAutoWidth = !tab.ButtonAutoWidth;
            tab.ButtonDefaultWidth = tab.ButtonDefaultWidth + 80;

            int width = tab.GetButtonWidthAt(0);
            width = tab.GetButtonWidth(item);

            tab.SetButtonWidthAt(0, 82);
            tab.SetButtonWidth(item, 62);

            width = tab.GetButtonWidthAt(0);
            width = tab.GetButtonWidth(item);

            tab.BeginToItemSelected();
            tab.BeginToItem(item);
            tab.BeginToItemAt(0);

            string name = tab.GetItemName(item);
            name = tab.GetItemNameAt(0);

            tab.SetItemNameAt(0, "new name0");
            name = tab.GetItemNameAt(0);

            object data = tab.GetItemDataAt(0) as string;
            tab.SetItemDataAt(0, "0");
            data = tab.GetItemDataAt(0) as string;
            tab.ClearItemDataAt(0);
            data = tab.GetItemDataAt(0) as string;

            uint index = tab.IndexSelected;
            tab.IndexSelected = 0;

            TabItem find = tab.FindItemWith("new name0");
            find = tab.FindItemWith("new name00");

            index = tab.FindItemIndexWith("sheet1");
            index = tab.FindItemIndexWith("sheet11");

            index = tab.GetItemIndex(item);
            item = tab.GetItemAt(0);

            //TabItem test start
            /*item.SetItemSelected();
            name = item.ItemName;
            item.ItemName = "new item name";
            item.ButtonWidth = item.ButtonWidth + 1;*/

            /*data = item.ItemData as string;
            item.ItemData = "new data";
            data = tab.GetItemData(item) as string;
            data = tab.GetItemDataAt(0) as string;
            data = item.ItemData as string;
            item.ItemData = null;
            data = item.ItemData as string;*/
            //TabItem test end

            tab.RemoveItem(item);
            item = null;

            tab.AddItem("add item");
            uint count = tab.ItemCount;
        }

        static void tab_EventTabChangeSelect(TabControl _sender, uint _index)
        {
            Export.DebugOut("EventTabChangeSelect  index=" + _index.ToString());
        }
    }
}
