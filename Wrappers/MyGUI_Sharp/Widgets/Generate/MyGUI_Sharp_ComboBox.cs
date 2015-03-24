/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public  class ComboBox :
		EditBox
    {
        #region ComboBox

        protected override string GetWidgetType() { return "ComboBox"; }

        internal static BaseWidget RequestWrapComboBox(BaseWidget _parent, IntPtr _widget)
        {
			ComboBox widget = new ComboBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateComboBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ComboBox widget = new ComboBox();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event ComboChangePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBoxEvent_AdviseComboChangePosition(IntPtr _native, bool _advise);

		public delegate void HandleComboChangePosition(
			ComboBox _sender,
			uint _index);
			
		private HandleComboChangePosition mEventComboChangePosition;
		public event HandleComboChangePosition EventComboChangePosition
		{
			add
			{
				if (ExportEventComboChangePosition.mDelegate == null)
				{
					ExportEventComboChangePosition.mDelegate = new ExportEventComboChangePosition.ExportHandle(OnExportComboChangePosition);
					ExportEventComboChangePosition.ExportComboBoxEvent_DelegateComboChangePosition(ExportEventComboChangePosition.mDelegate);
				}

				if (mEventComboChangePosition == null)
					ExportComboBoxEvent_AdviseComboChangePosition(Native, true);
				mEventComboChangePosition += value;
			}
			remove
			{
				mEventComboChangePosition -= value;
				if (mEventComboChangePosition == null)
					ExportComboBoxEvent_AdviseComboChangePosition(Native, false);
			}
		}

		private struct ExportEventComboChangePosition
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportComboBoxEvent_DelegateComboChangePosition(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportComboChangePosition(
			IntPtr _sender,
			uint _index)
		{
			ComboBox sender = (ComboBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventComboChangePosition != null)
				sender.mEventComboChangePosition(
					sender ,
					_index);
		}

		#endregion
		#region Event ComboAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBoxEvent_AdviseComboAccept(IntPtr _native, bool _advise);

		public delegate void HandleComboAccept(
			ComboBox _sender,
			uint _index);
			
		private HandleComboAccept mEventComboAccept;
		public event HandleComboAccept EventComboAccept
		{
			add
			{
				if (ExportEventComboAccept.mDelegate == null)
				{
					ExportEventComboAccept.mDelegate = new ExportEventComboAccept.ExportHandle(OnExportComboAccept);
					ExportEventComboAccept.ExportComboBoxEvent_DelegateComboAccept(ExportEventComboAccept.mDelegate);
				}

				if (mEventComboAccept == null)
					ExportComboBoxEvent_AdviseComboAccept(Native, true);
				mEventComboAccept += value;
			}
			remove
			{
				mEventComboAccept -= value;
				if (mEventComboAccept == null)
					ExportComboBoxEvent_AdviseComboAccept(Native, false);
			}
		}

		private struct ExportEventComboAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportComboBoxEvent_DelegateComboAccept(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportComboAccept(
			IntPtr _sender,
			uint _index)
		{
			ComboBox sender = (ComboBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventComboAccept != null)
				sender.mEventComboAccept(
					sender ,
					_index);
		}

		#endregion
		#region Method BeginToItemSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_BeginToItemSelected(IntPtr _native);

		public void BeginToItemSelected( )
		{
			ExportComboBox_BeginToItemSelected(Native);
		}

		#endregion
		#region Method BeginToItemLast

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_BeginToItemLast(IntPtr _native);

		public void BeginToItemLast( )
		{
			ExportComboBox_BeginToItemLast(Native);
		}

		#endregion
		#region Method BeginToItemFirst

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_BeginToItemFirst(IntPtr _native);

		public void BeginToItemFirst( )
		{
			ExportComboBox_BeginToItemFirst(Native);
		}

		#endregion
		#region Method BeginToItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_BeginToItemAt__index(IntPtr _native,
			uint _index);

		public void BeginToItemAt(
			uint _index)
		{
			ExportComboBox_BeginToItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportComboBox_GetItemNameAt__index(IntPtr _native,
			uint _index);

		public string GetItemNameAt(
			uint _index)
		{
			return Marshal.PtrToStringUni(ExportComboBox_GetItemNameAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetItemNameAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemNameAt(
			uint _index,
			string _name)
		{
			ExportComboBox_SetItemNameAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Method ClearIndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_ClearIndexSelected(IntPtr _native);

		public void ClearIndexSelected( )
		{
			ExportComboBox_ClearIndexSelected(Native);
		}

		#endregion
		#region Method FindItemIndexWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportComboBox_FindItemIndexWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public uint FindItemIndexWith(
			string _name)
		{
			return ExportComboBox_FindItemIndexWith__name(Native,
				_name);
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_RemoveAllItems(IntPtr _native);

		public void RemoveAllItems( )
		{
			ExportComboBox_RemoveAllItems(Native);
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_RemoveItemAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemAt(
			uint _index)
		{
			ExportComboBox_RemoveItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_AddItem__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void AddItem(
			string _name)
		{
			ExportComboBox_AddItem__name(Native,
				_name);
		}

		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_InsertItemAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void InsertItemAt(
			uint _index,
			string _name)
		{
			ExportComboBox_InsertItemAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Property FlowDirection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern FlowDirection ExportComboBox_GetFlowDirection(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetFlowDirection(IntPtr _widget, [MarshalAs(UnmanagedType.I4)] FlowDirection _value);

		public FlowDirection FlowDirection
		{
			get { return ExportComboBox_GetFlowDirection(Native); }
			set { ExportComboBox_SetFlowDirection(Native, value); }
		}

		#endregion
		#region Property MaxListLength

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportComboBox_GetMaxListLength(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetMaxListLength(IntPtr _widget, int _value);

		public int MaxListLength
		{
			get { return ExportComboBox_GetMaxListLength(Native); }
			set { ExportComboBox_SetMaxListLength(Native, value); }
		}

		#endregion
		#region Property SmoothShow

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportComboBox_GetSmoothShow(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetSmoothShow(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool SmoothShow
		{
			get { return ExportComboBox_GetSmoothShow(Native); }
			set { ExportComboBox_SetSmoothShow(Native, value); }
		}

		#endregion
		#region Property ComboModeDrop

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportComboBox_GetComboModeDrop(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetComboModeDrop(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool ComboModeDrop
		{
			get { return ExportComboBox_GetComboModeDrop(Native); }
			set { ExportComboBox_SetComboModeDrop(Native, value); }
		}

		#endregion
		#region Property IndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportComboBox_GetIndexSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportComboBox_SetIndexSelected(IntPtr _widget, uint _value);

		public uint IndexSelected
		{
			get { return ExportComboBox_GetIndexSelected(Native); }
			set { ExportComboBox_SetIndexSelected(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportComboBox_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportComboBox_GetItemCount(Native); }
		}

		#endregion
		
    }
}
