using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class TabBar
    {

        #region Method GetItemDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportTabBar_GetItemDataAt_index(IntPtr _widget,
             uint _index);

        public object GetItemDataAt(
            uint _index)
        {
            return ExportTabBar_GetItemDataAt_index(mNative,
                _index);
        }

        #endregion

        #region Method GetItemData

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportTabBar_GetItemData_item(IntPtr _native,
            [MarshalAs(UnmanagedType.Interface)]  BaseWidget _item);

        public object GetItemData(
            TabItem _item)
        {
            return ExportTabBar_GetItemData_item(mNative,
                 _item);
        }

        #endregion

    }

}
