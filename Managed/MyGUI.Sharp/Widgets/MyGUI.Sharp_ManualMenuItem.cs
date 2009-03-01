using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class MenuItem
    {

        #region Property ItemData

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMenuItem_GetItemData(IntPtr _widget);
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportMenuItem_SetItemData(IntPtr _widget, [MarshalAs(UnmanagedType.IUnknown)] object _value);

        public object ItemData
        {
            get { return ExportMenuItem_GetItemData(mNative); }
            set { ExportMenuItem_SetItemData(mNative, value); }
        }

        #endregion

    }

}
