using System;
using MyGUI.Managed;
using MyGUI.Managed.Demo;

namespace TestApp.Sharp
{
    public class Test_MultiListBox
    {
        public static void Test()
        {
            MultiListBox box = Gui.Instance.CreateWidget<MultiListBox>("MultiListBox", new IntCoord(220, 520, 200, 200), Align.Default, "Main");
            box.EventListChangePosition += new MultiListBox.HandleListChangePosition(box_EventListChangePosition);
            box.EventListSelectAccept += new MultiListBox.HandleListSelectAccept(box_EventListSelectAccept);

            box.AddColumn("column2", 96, "column2");
            box.AddColumn("column3", 96);
            box.InsertColumnAt(0, "column1", 96, "column1");
            box.InsertColumnAt(0, "column4", 96);

            string data = box.GetColumnDataAt(0) as string;
            box.ClearColumnDataAt(0);
            data = box.GetColumnDataAt(0) as string;

            uint count = box.ColumnCount;

            box.SetColumnNameAt(0, "new column");
            string name = box.GetColumnNameAt(0);
            int width = box.GetColumnWidthAt(0);
            box.SetColumnWidthAt(0, width + 1);
            width = box.GetColumnWidthAt(0);

            box.AddItem("item0", "0");
            box.AddItem("item1", "1");
            box.AddItem("item2");
            box.AddItem("item3", "3");
            box.InsertItemAt(0, "insert item4", "4");
            box.InsertItemAt(0, "insert item5");

            count = box.ItemCount;

            name = box.GetItemNameAt(0);
            box.SetItemNameAt(0, "new item 0");
            name = box.GetItemNameAt(0);

            data = box.GetItemDataAt(0) as string;
            box.ClearItemDataAt(0);
            data = box.GetItemDataAt(0) as string;

            box.SetSubItemNameAt(0, 3, "sub item 3");
            name = box.GetSubItemNameAt(0, 0);
            box.SetSubItemNameAt(1, 4, "sub item 4");

            box.SetSubItemDataAt(0, 0, "sub item0");
            data = box.GetSubItemDataAt(0, 0) as string;
            box.ClearSubItemDataAt(0, 0);
            data = box.GetSubItemDataAt(0, 0) as string;

            box.SwapItemsAt(0, 1);

            count = box.ItemCount;
            box.RemoveItemAt(0);
            count = box.ItemCount;

            box.IndexSelected = 0;
            box.IndexSelected = box.IndexSelected + 1;

            box.SortByColumn(0, false);
        }

        static void box_EventListSelectAccept(MultiListBox _sender, uint _index)
        {
            Export.DebugOut("EventListSelectAccept  index=" + _index.ToString());
        }

        static void box_EventListChangePosition(MultiListBox _sender, uint _index)
        {
            Export.DebugOut("EventListChangePosition  index=" + _index.ToString());
        }
    }
}
