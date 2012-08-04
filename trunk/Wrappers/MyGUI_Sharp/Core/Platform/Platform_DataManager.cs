using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{

		#region Export

		struct IsDataExist
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.U1)]
			private delegate bool HandleDelegate([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateIsDataExist(HandleDelegate _delegate);

			private static bool OnHandleDelegate(string _name)
			{
				return mDataManager.IsDataExist(_name);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportDataManager_DelegateIsDataExist(mHandleDelegate);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportDataManager_DelegateIsDataExist(null);
				}
			}
		}

		struct GetDataPath
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.LPStr)]
			private delegate string HandleDelegate([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetDataPath(HandleDelegate _delegate);

			private static string OnHandleDelegate(string _name)
			{
				return mDataManager.GetDataPath(_name);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportDataManager_DelegateGetDataPath(mHandleDelegate);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportDataManager_DelegateGetDataPath(null);
				}
			}
		}

		struct GetData
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.U4)]
			private delegate uint HandleDelegateSize([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegateSize mHandleDelegateSize;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetDataSize(HandleDelegateSize _delegate);

			private static uint OnHandleDelegateSize(string _name)
			{
				mData = mDataManager.GetData(_name);
				mDataName = _name;

				if (mData == null)
					return 0;

				return (uint)mData.Length;
			}

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegateData([MarshalAs(UnmanagedType.LPStr)]string _name, [Out, In] ref IntPtr _data);

			private static HandleDelegateData mHandleDelegateData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetData(HandleDelegateData _delegate);

			private static void OnHandleDelegateData(string _name, ref IntPtr _data)
			{
				if (mDataName == _name && mData != null)
				{
					IntPtr ptr = Marshal.AllocHGlobal(mData.Length);
					Marshal.Copy(mData, 0, ptr, mData.Length);
					_data = ptr;
				}
			}

			private static byte[] mData;
			private static string mDataName;

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegateSize += OnHandleDelegateSize;
					ExportDataManager_DelegateGetDataSize(mHandleDelegateSize);

					mHandleDelegateData += OnHandleDelegateData;
					ExportDataManager_DelegateGetData(mHandleDelegateData);
				}
				else
				{
					mHandleDelegateSize -= OnHandleDelegateSize;
					ExportDataManager_DelegateGetDataSize(null);

					mHandleDelegateData -= OnHandleDelegateData;
					ExportDataManager_DelegateGetData(null);
				}
			}
		}


		#endregion

		private static void InitialiseDataManager(IPlatformDataManager _dataManager)
		{
			mDataManager = _dataManager;

			IsDataExist.Advise(true);
			GetDataPath.Advise(true);
			GetData.Advise(true);
		}

		private static void ShutdownDataManager()
		{
			IsDataExist.Advise(false);
			GetDataPath.Advise(false);
			GetData.Advise(false);

			mDataManager = null;
		}

		private static IPlatformDataManager mDataManager;
	}
}
