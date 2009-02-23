using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_DDContainer
    {
        public static void Test()
        {
            DDContainer container = new DDContainer(IntPtr.Zero, WidgetStyle.Overlapped, "Button", new IntCoord(420, 420, 100, 100), Align.Default, "Overlapped", "");
            container.NeedDragDrop = !container.NeedDragDrop;

            container.EventChangeDDState += new DDContainer.HandleChangeDDState(container_EventChangeDDState);
            container.EventDropResult += new DDContainer.HandleDropResult(container_EventDropResult);
            container.EventRequestDrop += new DDContainer.HandleRequestDrop(container_EventRequestDrop);
            container.EventStartDrag += new DDContainer.HandleStartDrag(container_EventStartDrag);
            container.EventUpdateDropState += new DDContainer.HandleUpdateDropState(container_EventUpdateDropState);
            container.RequestDragWidgetInfo += new DDContainer.HandleDragWidgetInfo(container_RequestDragWidgetInfo);

        }

        static void container_RequestDragWidgetInfo(DDContainer _sender, ref Widget _item, ref IntCoord _dimension)
        {
            Widget widget = new Widget(IntPtr.Zero, WidgetStyle.Overlapped, "Button", new IntCoord(0, 0, 10, 10), Align.Default, "Popup", "");

            _item = widget;
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
        }

        static void container_EventChangeDDState(DDContainer _sender, DDItemState _state)
        {
            Export.DebugOut("EventChangeDDState    state=" + _state.ToString());
        }

        /*
            #region Event StartDrag

            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportDDContainerEvent_AdviseStartDrag( IntPtr _widget, bool _advise );

            public delegate void HandleStartDrag(
                DDContainer _sender ,
                ref DDItemInfo _info ,
                ref bool _result );
			
            private HandleStartDrag mEventStartDrag;
            public event HandleStartDrag EventStartDrag
            {
                add
                {
                    if (mEventStartDrag == null) ExportDDContainerEvent_AdviseStartDrag( mNative, true );
                    mEventStartDrag += value;
                }
                remove
                {
                    mEventStartDrag -= value;
                    if (mEventStartDrag == null) ExportDDContainerEvent_AdviseStartDrag( mNative, false );
                }
            }


            private struct ExportEventStartDrag
            {
                [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
                private static extern void ExportDDContainerEvent_DelegateStartDrag( ExportHandle _delegate );
                public delegate void ExportHandle(
                    [MarshalAs(UnmanagedType.Interface)] DDContainer _sender ,
                    [In] ref DDItemInfo _info,
                    [MarshalAs(UnmanagedType.U1)] ref bool _result);
				
                private static ExportHandle mDelegate;
                public ExportEventStartDrag( ExportHandle _delegate )
                {
                    mDelegate = _delegate;
                    ExportDDContainerEvent_DelegateStartDrag( mDelegate );
                }
            }
            static ExportEventStartDrag mExportStartDrag =
                new ExportEventStartDrag(new ExportEventStartDrag.ExportHandle( OnExportStartDrag ));

            private static void OnExportStartDrag(
                DDContainer _sender ,
                ref DDItemInfo _info ,
                ref bool _result )
            {
                if (_sender.mEventStartDrag != null)
                    _sender.mEventStartDrag(
                        _sender ,
                        ref _info ,
                        ref _result );
            }

            #endregion
         
         */
    }
}
