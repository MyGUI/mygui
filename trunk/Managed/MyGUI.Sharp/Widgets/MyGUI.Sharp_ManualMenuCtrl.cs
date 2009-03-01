using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class MenuCtrl
    {

        #region Method GetItemDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMenuCtrl_GetItemDataAt_index(IntPtr _widget,
             uint _index);

        public object GetItemDataAt(
            uint _index)
        {
            return ExportMenuCtrl_GetItemDataAt_index(mNative,
                _index);
        }

        #endregion

        #region Method GetItemData

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMenuCtrl_GetItemData_item(IntPtr _native,
            [MarshalAs(UnmanagedType.Interface)]  BaseWidget _item);

        public object GetItemData(
            MenuItem _item)
        {
            return ExportMenuCtrl_GetItemData_item(mNative,
                 _item);
        }

        #endregion

    }

}
