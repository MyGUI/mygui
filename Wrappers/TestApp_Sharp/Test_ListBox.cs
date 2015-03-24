using MyGUI.Sharp;

namespace TestApp.Sharp
{
	public class Test_ListBox
	{
		public static void Test()
		{
			ListBox box = Gui.Instance.CreateWidget<ListBox>("ListBox", new IntCoord(20, 420, 100, 100), Align.Default, "Main");
			box.EventListChangeScroll += new ListBox.HandleListChangeScroll(box_EventListChangeScroll);
			box.EventListMouseItemFocus += new ListBox.HandleListMouseItemFocus(box_EventListMouseItemFocus);
			box.EventListMouseItemActivate += new ListBox.HandleListMouseItemActivate(box_EventListMouseItemActivate);
			box.EventListChangePosition += new ListBox.HandleListChangePosition(box_EventListChangePosition);
			box.EventListSelectAccept += new ListBox.HandleListSelectAccept(box_EventListSelectAccept);

			box.AddItem("line0");
			box.AddItem("line1");
			box.AddItem("line2");
			box.AddItem("line3");
			box.InsertItemAt(0, "insert line0");

			int height = box.OptimalHeight;
			box.SetScrollVisible(true);
			bool visible = box.IsItemSelectedVisible(true);
			visible = box.IsItemVisibleAt(0, true);
			box.BeginToItemSelected();
			box.BeginToItemLast();
			box.BeginToItemFirst();
			box.BeginToItemAt(0);
			string name = box.GetItemNameAt(0);
			box.SetItemNameAt(0, "set line0");
			name = box.GetItemNameAt(0);
			//box.SetItemDataAt(0, "0");
			//string data = box.GetItemDataAt(0) as string;
			//box.ClearItemDataAt(0);
			//data = box.GetItemDataAt(0) as string;
			box.ClearIndexSelected();
			box.IndexSelected = 0;
			uint index = box.FindItemIndexWith("line3");
			index = box.FindItemIndexWith("line33");
			box.SwapItemsAt(0, 1);
			box.RemoveItemAt(2);
			uint count = box.ItemCount;
			//box.RemoveAllItems();
			//count = box.ItemCount;
		}

		static void box_EventListSelectAccept(ListBox _sender, uint _index)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventListSelectAccept  index=" + _index.ToString());
		}

		static void box_EventListChangePosition(ListBox _sender, uint _index)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventListChangePosition  index=" + _index.ToString());
		}

		static void box_EventListMouseItemActivate(ListBox _sender, uint _index)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventListMouseItemActivate  index=" + _index.ToString());
		}

		static void box_EventListMouseItemFocus(ListBox _sender, uint _index)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventListMouseItemFocus  index=" + _index.ToString());
		}

		static void box_EventListChangeScroll(ListBox _sender, uint _position)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventListChangeScroll  index=" + _position.ToString());
		}
	}
}
