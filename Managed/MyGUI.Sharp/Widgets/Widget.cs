using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public class Widget : BaseWidget
    {

        #region Widget

        public Widget()
            : base()
        {
        }

        public Widget(IntPtr _widget)
            : base(_widget)
        {
        }

        public Widget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Widget"; }

        #endregion

        #region Property Caption

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportWidget_GetCaption([MarshalAs(UnmanagedType.LPWStr)]out string _result, IntPtr _widget);
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportWidget_SetCaption(IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]string _caption);

        public string Caption
        {
            get { string result; ExportWidget_GetCaption(out result, mNative); return result; }
            set { ExportWidget_SetCaption(mNative, value); }
        }

        #endregion

        #region Property NeedToolTip

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportWidget_GetNeedToolTip(out bool _result, IntPtr _widget);
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportWidget_SetNeedToolTip(IntPtr _widget, bool _value);

        public bool NeedToolTip
        {
            get { bool result; ExportWidget_GetNeedToolTip(out result, mNative); return result; }
            set { ExportWidget_SetNeedToolTip(mNative, value); }
        }

        #endregion

        #region Event ToolTip

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportWidgetEvent_AdviseToolTip(IntPtr _widget, bool _advise);

        public delegate void HandleToolTip(Widget _sender, ToolTipInfo _info);
        private HandleToolTip mEventToolTip;
        public event HandleToolTip EventToolTip
        {
            add
            {
                if (mEventToolTip == null) ExportWidgetEvent_AdviseToolTip(mNative, true);
                mEventToolTip += value;
            }
            remove
            {
                mEventToolTip -= value;
                if (mEventToolTip == null) ExportWidgetEvent_AdviseToolTip(mNative, false);
            }
        }


        private struct ExportEventToolTip
        {
            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportWidgetEvent_ExportToolTip(ExportHandle _delegate);
            public delegate void ExportHandle([MarshalAs(UnmanagedType.IUnknown)]object _wrapper, ref ToolTipInfo _info);
            private static ExportHandle mDelegate;
            public ExportEventToolTip(ExportHandle _delegate)
            {
                mDelegate = _delegate;
                ExportWidgetEvent_ExportToolTip(mDelegate);
            }
        }
        static ExportEventToolTip mExportToolTip = new ExportEventToolTip(new ExportEventToolTip.ExportHandle(OnExportToolTip));

        private static void OnExportToolTip(object _wrapper, ref ToolTipInfo _info)
        {
            Widget widget = _wrapper as Widget;
            if (widget != null && widget.mEventToolTip != null) widget.mEventToolTip(widget, _info);
        }

        #endregion

    }

}
