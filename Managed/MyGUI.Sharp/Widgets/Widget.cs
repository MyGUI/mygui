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
        public static extern void ExportWidget_GetCaption(IntPtr _widget, [OutAttribute][MarshalAs(UnmanagedType.LPWStr)]out string _result);
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ExportWidget_SetCaption(IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]string _caption);

        public string Caption
        {
            get
            {
                string value;
                ExportWidget_GetCaption(mNative, out value);
                return value;
            }
            set
            {
                ExportWidget_SetCaption(mNative, value);
            }
        }

        #endregion

    }

}
