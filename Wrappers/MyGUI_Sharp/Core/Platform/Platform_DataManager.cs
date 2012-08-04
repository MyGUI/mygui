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
					mHandleDelegate += OnHandleDelegate;
				else
					mHandleDelegate -= OnHandleDelegate;
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
					mHandleDelegate += OnHandleDelegate;
				else
					mHandleDelegate -= OnHandleDelegate;
			}
		}

		struct GetData
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate uint HandleDelegateSize([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegateSize mHandleDelegateSize;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetDataSize(HandleDelegateSize _delegate);

			private static uint OnHandleDelegateSize(string _name)
			{
				mData = mDataManager.GetData(_name);
				mDataName = _name;

				return (uint)mData.Length;
			}

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.LPArray)]
			private delegate byte[] HandleDelegateData([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegateData mHandleDelegateData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetData(HandleDelegateData _delegate);

			private static byte[] OnHandleDelegateData(string _name)
			{
				if (mDataName == _name)
					return mData;

				return null;
			}

			private static byte[] mData;
			private static string mDataName;

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegateSize += OnHandleDelegateSize;
					mHandleDelegateData += OnHandleDelegateData;
				}
				else
				{
					mHandleDelegateSize -= OnHandleDelegateSize;
					mHandleDelegateData -= OnHandleDelegateData;
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
