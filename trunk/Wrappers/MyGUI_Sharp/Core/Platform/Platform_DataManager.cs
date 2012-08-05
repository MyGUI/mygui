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

		struct GetData
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.U4)]
			private delegate uint HandleDelegateData([MarshalAs(UnmanagedType.LPStr)]string _name, [Out, In] ref IntPtr _data);

			private static HandleDelegateData mHandleDelegateData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetData(HandleDelegateData _delegate);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegateFreeData([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegateFreeData mHandleDelegateFreeData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateFreeData(HandleDelegateFreeData _delegate);

			private static uint OnHandleDelegateData(string _name, ref IntPtr _data)
			{
				byte[] data = mDataManager.GetData(_name);
				if (data == null)
				{
					_data = IntPtr.Zero;
					return 0;
				}

				IntPtr ptr;
				if (mCashe.TryGetValue(_name, out ptr))
				{
					Marshal.FreeHGlobal(ptr);
					ptr = IntPtr.Zero;

					Log(LogLevel.Warning, string.Format("Cashe for data {0} already exist", _name));
				}

				ptr = Marshal.AllocHGlobal(data.Length);
				Marshal.Copy(data, 0, ptr, data.Length);
				_data = ptr;

				mCashe.Add(_name, ptr);

				return (uint)data.Length;
			}

			private static void OnHandleDelegateFreeData(string _name)
			{
				IntPtr ptr;
				if (mCashe.TryGetValue(_name, out ptr))
				{
					Marshal.FreeHGlobal(ptr);
					ptr = IntPtr.Zero;
				}
				else
				{
					Log(LogLevel.Warning, string.Format("Cashe for data {0} not found", _name));
				}

				mDataManager.FreeData(_name);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegateData += OnHandleDelegateData;
					ExportDataManager_DelegateGetData(mHandleDelegateData);

					mHandleDelegateFreeData += OnHandleDelegateFreeData;
					ExportDataManager_DelegateFreeData(mHandleDelegateFreeData);
				}
				else
				{
					mHandleDelegateData -= OnHandleDelegateData;
					ExportDataManager_DelegateGetData(null);

					mHandleDelegateFreeData -= OnHandleDelegateFreeData;
					ExportDataManager_DelegateFreeData(null);
				}
			}

			private static Dictionary<string, IntPtr> mCashe = new Dictionary<string, IntPtr>();
		}

		#endregion

		private static void InitialiseDataManager(IPlatformDataManager _dataManager)
		{
			mDataManager = _dataManager;

			IsDataExist.Advise(true);
			GetData.Advise(true);
		}

		private static void ShutdownDataManager()
		{
			IsDataExist.Advise(false);
			GetData.Advise(false);

			mDataManager = null;
			mRenderManager = null;
		}

		private static IPlatformDataManager mDataManager;
	}
}
