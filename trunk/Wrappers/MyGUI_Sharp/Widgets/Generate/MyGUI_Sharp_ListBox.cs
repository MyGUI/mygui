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

    public  class ListBox : Widget
    {

        #region ListBox

        protected override string GetWidgetType() { return "ListBox"; }

        internal static BaseWidget RequestWrapListBox(BaseWidget _parent, IntPtr _widget)
        {
			ListBox widget = new ListBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateListBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ListBox widget = new ListBox();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event ListChangeScroll

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListChangeScroll( IntPtr _native, bool _advise );

		public delegate void HandleListChangeScroll(
			 ListBox _sender ,
			 uint _position );
			
		private HandleListChangeScroll mEventListChangeScroll;
		public event HandleListChangeScroll EventListChangeScroll
		{
			add
			{
				if (ExportEventListChangeScroll.mDelegate == null)
				{
					ExportEventListChangeScroll.mDelegate = new ExportEventListChangeScroll.ExportHandle( OnExportListChangeScroll );
					ExportEventListChangeScroll.ExportListBoxEvent_DelegateListChangeScroll( ExportEventListChangeScroll.mDelegate );
				}

				if (mEventListChangeScroll == null)
					ExportListBoxEvent_AdviseListChangeScroll( mNative, true );
				mEventListChangeScroll += value;
			}
			remove
			{
				mEventListChangeScroll -= value;
				if (mEventListChangeScroll == null)
					ExportListBoxEvent_AdviseListChangeScroll( mNative, false );
			}
		}

		private struct ExportEventListChangeScroll
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListChangeScroll( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ListBox _sender ,
				  uint _position );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListChangeScroll(
			 ListBox _sender ,
			 uint _position )
		{
			if (_sender.mEventListChangeScroll != null)
				_sender.mEventListChangeScroll(
					 _sender ,
					 _position );
		}

		#endregion
		#region Event ListMouseItemFocus

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListMouseItemFocus( IntPtr _native, bool _advise );

		public delegate void HandleListMouseItemFocus(
			 ListBox _sender ,
			 uint _index );
			
		private HandleListMouseItemFocus mEventListMouseItemFocus;
		public event HandleListMouseItemFocus EventListMouseItemFocus
		{
			add
			{
				if (ExportEventListMouseItemFocus.mDelegate == null)
				{
					ExportEventListMouseItemFocus.mDelegate = new ExportEventListMouseItemFocus.ExportHandle( OnExportListMouseItemFocus );
					ExportEventListMouseItemFocus.ExportListBoxEvent_DelegateListMouseItemFocus( ExportEventListMouseItemFocus.mDelegate );
				}

				if (mEventListMouseItemFocus == null)
					ExportListBoxEvent_AdviseListMouseItemFocus( mNative, true );
				mEventListMouseItemFocus += value;
			}
			remove
			{
				mEventListMouseItemFocus -= value;
				if (mEventListMouseItemFocus == null)
					ExportListBoxEvent_AdviseListMouseItemFocus( mNative, false );
			}
		}

		private struct ExportEventListMouseItemFocus
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListMouseItemFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ListBox _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListMouseItemFocus(
			 ListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListMouseItemFocus != null)
				_sender.mEventListMouseItemFocus(
					 _sender ,
					 _index );
		}

		#endregion
		#region Event ListMouseItemActivate

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListMouseItemActivate( IntPtr _native, bool _advise );

		public delegate void HandleListMouseItemActivate(
			 ListBox _sender ,
			 uint _index );
			
		private HandleListMouseItemActivate mEventListMouseItemActivate;
		public event HandleListMouseItemActivate EventListMouseItemActivate
		{
			add
			{
				if (ExportEventListMouseItemActivate.mDelegate == null)
				{
					ExportEventListMouseItemActivate.mDelegate = new ExportEventListMouseItemActivate.ExportHandle( OnExportListMouseItemActivate );
					ExportEventListMouseItemActivate.ExportListBoxEvent_DelegateListMouseItemActivate( ExportEventListMouseItemActivate.mDelegate );
				}

				if (mEventListMouseItemActivate == null)
					ExportListBoxEvent_AdviseListMouseItemActivate( mNative, true );
				mEventListMouseItemActivate += value;
			}
			remove
			{
				mEventListMouseItemActivate -= value;
				if (mEventListMouseItemActivate == null)
					ExportListBoxEvent_AdviseListMouseItemActivate( mNative, false );
			}
		}

		private struct ExportEventListMouseItemActivate
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListMouseItemActivate( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ListBox _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListMouseItemActivate(
			 ListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListMouseItemActivate != null)
				_sender.mEventListMouseItemActivate(
					 _sender ,
					 _index );
		}

		#endregion
		#region Event ListChangePosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListChangePosition( IntPtr _native, bool _advise );

		public delegate void HandleListChangePosition(
			 ListBox _sender ,
			 uint _index );
			
		private HandleListChangePosition mEventListChangePosition;
		public event HandleListChangePosition EventListChangePosition
		{
			add
			{
				if (ExportEventListChangePosition.mDelegate == null)
				{
					ExportEventListChangePosition.mDelegate = new ExportEventListChangePosition.ExportHandle( OnExportListChangePosition );
					ExportEventListChangePosition.ExportListBoxEvent_DelegateListChangePosition( ExportEventListChangePosition.mDelegate );
				}

				if (mEventListChangePosition == null)
					ExportListBoxEvent_AdviseListChangePosition( mNative, true );
				mEventListChangePosition += value;
			}
			remove
			{
				mEventListChangePosition -= value;
				if (mEventListChangePosition == null)
					ExportListBoxEvent_AdviseListChangePosition( mNative, false );
			}
		}

		private struct ExportEventListChangePosition
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListChangePosition( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ListBox _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListChangePosition(
			 ListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListChangePosition != null)
				_sender.mEventListChangePosition(
					 _sender ,
					 _index );
		}

		#endregion
		#region Event ListSelectAccept

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListSelectAccept( IntPtr _native, bool _advise );

		public delegate void HandleListSelectAccept(
			 ListBox _sender ,
			 uint _index );
			
		private HandleListSelectAccept mEventListSelectAccept;
		public event HandleListSelectAccept EventListSelectAccept
		{
			add
			{
				if (ExportEventListSelectAccept.mDelegate == null)
				{
					ExportEventListSelectAccept.mDelegate = new ExportEventListSelectAccept.ExportHandle( OnExportListSelectAccept );
					ExportEventListSelectAccept.ExportListBoxEvent_DelegateListSelectAccept( ExportEventListSelectAccept.mDelegate );
				}

				if (mEventListSelectAccept == null)
					ExportListBoxEvent_AdviseListSelectAccept( mNative, true );
				mEventListSelectAccept += value;
			}
			remove
			{
				mEventListSelectAccept -= value;
				if (mEventListSelectAccept == null)
					ExportListBoxEvent_AdviseListSelectAccept( mNative, false );
			}
		}

		private struct ExportEventListSelectAccept
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ListBox _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListSelectAccept(
			 ListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListSelectAccept != null)
				_sender.mEventListSelectAccept(
					 _sender ,
					 _index );
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportListBox_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportListBox_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportListBox_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion
		#region Method SetCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetCoord_left_top_width_height( IntPtr _native ,
			  int _left ,
			  int _top ,
			  int _width ,
			  int _height );

		public void SetCoord(
			int _left ,
			int _top ,
			int _width ,
			int _height )
		{
			ExportListBox_SetCoord_left_top_width_height( mNative , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetSize(
			int _width ,
			int _height )
		{
			ExportListBox_SetSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetPosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetPosition_left_top( IntPtr _native ,
			  int _left ,
			  int _top );

		public void SetPosition(
			int _left ,
			int _top )
		{
			ExportListBox_SetPosition_left_top( mNative , 
				 _left ,
				 _top );
		}

		#endregion
		#region Method SetScrollPosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetScrollPosition_position( IntPtr _native ,
			  uint _position );

		public void SetScrollPosition(
			uint _position )
		{
			ExportListBox_SetScrollPosition_position( mNative , 
				 _position );
		}

		#endregion
		#region Method SetScrollVisible

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetScrollVisible_visible( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetScrollVisible(
			bool _visible )
		{
			ExportListBox_SetScrollVisible_visible( mNative , 
				 _visible );
		}

		#endregion
		#region Method IsItemSelectedVisible

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportListBox_IsItemSelectedVisible_fill( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _fill );

		public bool IsItemSelectedVisible(
			bool _fill )
		{
			return  ExportListBox_IsItemSelectedVisible_fill( mNative , 
				 _fill )  ;
		}

		#endregion
		#region Method IsItemVisibleAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportListBox_IsItemVisibleAt_index_fill( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.U1)]  bool _fill );

		public bool IsItemVisibleAt(
			uint _index ,
			bool _fill )
		{
			return  ExportListBox_IsItemVisibleAt_index_fill( mNative , 
				 _index ,
				 _fill )  ;
		}

		#endregion
		#region Method BeginToItemSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemSelected( IntPtr _native );

		public void BeginToItemSelected( )
		{
			ExportListBox_BeginToItemSelected(  mNative );
		}

		#endregion
		#region Method BeginToItemLast

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemLast( IntPtr _native );

		public void BeginToItemLast( )
		{
			ExportListBox_BeginToItemLast(  mNative );
		}

		#endregion
		#region Method BeginToItemFirst

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemFirst( IntPtr _native );

		public void BeginToItemFirst( )
		{
			ExportListBox_BeginToItemFirst(  mNative );
		}

		#endregion
		#region Method BeginToItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemAt_index( IntPtr _native ,
			  uint _index );

		public void BeginToItemAt(
			uint _index )
		{
			ExportListBox_BeginToItemAt_index( mNative , 
				 _index );
		}

		#endregion
		#region Method GetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportListBox_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportListBox_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion
		#region Method ClearItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportListBox_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion
		#region Method SetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportListBox_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion
		#region Method ClearIndexSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_ClearIndexSelected( IntPtr _native );

		public void ClearIndexSelected( )
		{
			ExportListBox_ClearIndexSelected(  mNative );
		}

		#endregion
		#region Method FindItemIndexWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_FindItemIndexWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindItemIndexWith(
			string _name )
		{
			return  ExportListBox_FindItemIndexWith_name( mNative , 
				 _name )  ;
		}

		#endregion
		#region Method SwapItemsAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SwapItemsAt_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapItemsAt(
			uint _index1 ,
			uint _index2 )
		{
			ExportListBox_SwapItemsAt_index1_index2( mNative , 
				 _index1 ,
				 _index2 );
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportListBox_RemoveAllItems(  mNative );
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportListBox_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion
		#region Method AddItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_AddItem_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void AddItem(
			string _name ,
			object _data )
		{
			ExportListBox_AddItem_name_data( mNative , 
				 _name ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void AddItem(
			string _name )
		{
			ExportListBox_AddItem_name( mNative , 
				 _name );
		}

		#endregion
		#region Method InsertItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_InsertItemAt_index_name_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void InsertItemAt(
			uint _index ,
			string _name ,
			object _data )
		{
			ExportListBox_InsertItemAt_index_name_data( mNative , 
				 _index ,
				 _name ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void InsertItemAt(
			uint _index ,
			string _name )
		{
			ExportListBox_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion
		#region Property OptimalHeight

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportListBox_GetOptimalHeight( IntPtr _native );

		public int OptimalHeight
		{
			get { return  ExportListBox_GetOptimalHeight( mNative )  ; }
		}

		#endregion
		#region Property IndexSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_GetIndexSelected( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportListBox_GetIndexSelected( mNative )  ; }
			set { ExportListBox_SetIndexSelected( mNative,  value ); }
		}

		#endregion
		#region Property ItemCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportListBox_GetItemCount( mNative )  ; }
		}

		#endregion
		
    }

}
