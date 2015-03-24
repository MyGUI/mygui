using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Managed;

namespace TestApp.Managed
{
    public class Test_DDContainer
    {
        static Widget mWidget;

        public static void Test()
        {
            DDContainer container = Gui.Instance.CreateWidget<DDContainer>("Button", new IntCoord(420, 520, 100, 100), Align.Default, "Main");

            container.EventChangeDDState += new DDContainer.HandleChangeDDState(container_EventChangeDDState);
            container.EventDropResult += new DDContainer.HandleDropResult(container_EventDropResult);
            container.EventRequestDrop += new DDContainer.HandleRequestDrop(container_EventRequestDrop);
            container.EventStartDrag += new DDContainer.HandleStartDrag(container_EventStartDrag);
            container.RequestDragWidgetInfo += new DDContainer.HandleDragWidgetInfo(container_RequestDragWidgetInfo);

			container.NeedDragDrop = !container.NeedDragDrop;

			mWidget = Gui.Instance.CreateWidget<Widget>("Button", new IntCoord(0, 0, 10, 10), Align.Default, "Popup");
            mWidget.Visible = false;
        }

        static void container_RequestDragWidgetInfo(DDContainer _sender, ref Widget _item, ref IntCoord _dimension)
        {
            mWidget.Visible = true;
            _item = mWidget;
            _dimension = new IntCoord(0, 0, 20, 20);
        }

        static void container_EventStartDrag(DDContainer _sender, DDItemInfo _info, ref bool _result)
        {
            _result = true;
        }

        static void container_EventRequestDrop(DDContainer _sender, DDItemInfo _info, ref bool _result)
        {
            _result = true;
        }

        static void container_EventDropResult(DDContainer _sender, DDItemInfo _info, bool _result)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventDropResult    result=" + _result.ToString());
            mWidget.Visible = false;
        }

        static void container_EventChangeDDState(DDContainer _sender, DDItemState _state)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventChangeDDState    state=" + _state.ToString());
        }

    }
}
