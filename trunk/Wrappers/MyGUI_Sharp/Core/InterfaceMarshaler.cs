using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public class InterfaceMarshaler :
        ICustomMarshaler
    {
        #region ICustomMarshaler Members

        public void CleanUpManagedData(object ManagedObj)
        {
        }

        public void CleanUpNativeData(IntPtr pNativeData)
        {
        }

        public int GetNativeDataSize()
        {
            return -1;
        }

        public IntPtr MarshalManagedToNative(object ManagedObj)
        {
            return Marshal.GetIUnknownForObject(ManagedObj);
        }

        public object MarshalNativeToManaged(IntPtr pNativeData)
        {
            return Marshal.GetObjectForIUnknown(pNativeData);
        }

        #endregion

        public static ICustomMarshaler GetInstance(string cookie)
        {
            if (marshaler == null)
                return marshaler = new InterfaceMarshaler();
            return marshaler;
        }

        static InterfaceMarshaler marshaler;
    }
}
