using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class ItemBox
    {

        #region Method GetItemDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportItemBox_GetItemDataAt_index(IntPtr _widget,
             uint _index);

        public object GetItemDataAt(
            uint _index)
        {
            return ExportItemBox_GetItemDataAt_index(mNative,
                _index);
        }

        #endregion

        #region Method GetIndexByWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]

        private static extern uint ExportItemBox_GetIndexByWidget_widget(IntPtr _native,
            [MarshalAs(UnmanagedType.Interface)]  BaseWidget _widget);

        public uint GetIndexByWidget(
            Widget _widget)
        {
            return ExportItemBox_GetIndexByWidget_widget(mNative,
                 _widget);
        }

        #endregion

    }

} // namespace MyGUI.Sharp.Widgets
