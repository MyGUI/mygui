using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_DDContainer
    {
        static Widget mWidget;

        public static void Test()
        {
            DDContainer container = Gui.Instance.CreateWidget<DDContainer>("Button", new IntCoord(420, 520, 100, 100), Align.Default, "Main");
            container.NeedDragDrop = !container.NeedDragDrop;

            container.EventChangeDDState += new DDContainer.HandleChangeDDState(container_EventChangeDDState);
            container.EventDropResult += new DDContainer.HandleDropResult(container_EventDropResult);
            container.EventRequestDrop += new DDContainer.HandleRequestDrop(container_EventRequestDrop);
            container.EventStartDrag += new DDContainer.HandleStartDrag(container_EventStartDrag);
            container.EventUpdateDropState += new DDContainer.HandleUpdateDropState(container_EventUpdateDropState);
            container.RequestDragWidgetInfo += new DDContainer.HandleDragWidgetInfo(container_RequestDragWidgetInfo);

            mWidget = Gui.Instance.CreateWidget<Widget>("Button", new IntCoord(0, 0, 10, 10), Align.Default, "Popup");
            mWidget.Visible = false;
        }

        static void container_RequestDragWidgetInfo(DDContainer _sender, ref Widget _item, ref IntCoord _dimension)
        {
            mWidget.Visible = true;
            _item = mWidget;
            _dimension = new IntCoord(0, 0, 20, 20);
        }

        static void container_EventUpdateDropState(DDContainer _sender, Widget _items, ref DDWidgetState _state)
        {
            Export.DebugOut("EventUpdateDropState    state=" + _state.ToString());
        }

        static void container_EventStartDrag(DDContainer _sender, ref DDItemInfo _info, ref bool _result)
        {
            _result = true;
        }

        static void container_EventRequestDrop(DDContainer _sender, ref DDItemInfo _info, ref bool _result)
        {
            _result = true;
        }

        static void container_EventDropResult(DDContainer _sender, ref DDItemInfo _info, bool _result)
        {
            Export.DebugOut("EventDropResult    result=" + _result.ToString());
            mWidget.Visible = false;
        }

        static void container_EventChangeDDState(DDContainer _sender, DDItemState _state)
        {
            Export.DebugOut("EventChangeDDState    state=" + _state.ToString());
        }

    }
}
