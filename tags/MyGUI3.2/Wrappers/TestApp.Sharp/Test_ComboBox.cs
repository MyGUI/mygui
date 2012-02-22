using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_ComboBox
    {
        public static void Test()
        {
            ComboBox box = Gui.Instance.CreateWidget<ComboBox>("ComboBox", new IntCoord(120, 320, 200, 26), Align.Default, "Main");
            box.AddItem("line0", (int)0);
            box.AddItem("line1");
            box.AddItem("line2", (int)2);
            box.AddItem("line3");
            box.AddItem("line4");
            box.AddItem("line5");
            box.AddItem("line6");
            box.AddItem("line7");
            box.AddItem("line8");

            box.InsertItemAt(6, "insert item6", "6");
            box.InsertItemAt(7, "insert item7");

            box.MaxListLength = box.MaxListLength + 10;
            box.SmoothShow = !box.SmoothShow;
            box.ComboModeDrop = !box.ComboModeDrop;
            box.ComboModeDrop = !box.ComboModeDrop;

            box.BeginToItemSelected();
            box.BeginToItemLast();
            box.BeginToItemFirst();
            box.BeginToItemAt(2);

            string name = box.GetItemNameAt(2);
            box.SetItemNameAt(2, "new line2");
            name = box.GetItemNameAt(2);

            box.SetItemDataAt(0, (int)0);
            object data = box.GetItemDataAt(0);
            box.SetItemDataAt(3, (int)3);
            data = box.GetItemDataAt(3);

            box.ClearItemDataAt(0);
            data = box.GetItemDataAt(0);
            data = box.GetItemDataAt(3);

            box.IndexSelected = 1;
            box.IndexSelected = box.IndexSelected + 1;
            box.ClearIndexSelected();

            uint index = box.FindItemIndexWith("line3");
            box.RemoveItemAt(3);
            index = box.FindItemIndexWith("line3");

            box.InsertItemAt(2, "insert line2", (int)2);
            name = box.GetItemNameAt(2);
            name = box.GetItemNameAt(3);

            uint count = box.ItemCount;

            box.EventComboAccept += new ComboBox.HandleComboAccept(box_EventComboAccept);
            box.EventComboChangePosition += new ComboBox.HandleComboChangePosition(box_EventComboChangePosition);
        }
        static void box_EventComboChangePosition(ComboBox _sender, uint _index)
        {
            Export.DebugOut("EventComboChangePosition  index=" + _index.ToString());
        }

        static void box_EventComboAccept(ComboBox _sender, uint _index)
        {
            Export.DebugOut("EventComboAccept  index=" + _index.ToString());
        }
    }
}
