using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class TabItem
    {

        #region Property ItemData

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportTabItem_GetItemData(IntPtr _widget);
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportTabItem_SetItemData(IntPtr _widget, [MarshalAs(UnmanagedType.IUnknown)] object _value);

        public object ItemData
        {
            get { return ExportTabItem_GetItemData(mNative); }
            set { ExportTabItem_SetItemData(mNative, value); }
        }

        #endregion

    }

}
