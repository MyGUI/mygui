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
    public  class ImageBox :
		Widget
    {
        #region ImageBox

        protected override string GetWidgetType() { return "ImageBox"; }

        internal static BaseWidget RequestWrapImageBox(BaseWidget _parent, IntPtr _widget)
        {
			ImageBox widget = new ImageBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateImageBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ImageBox widget = new ImageBox();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemName__value(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _value);

		public void SetItemName(
			string _value)
		{
			ExportImageBox_SetItemName__value(Native,
				_value);
		}

		#endregion
		#region Method SetItemGroup

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemGroup__value(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _value);

		public void SetItemGroup(
			string _value)
		{
			ExportImageBox_SetItemGroup__value(Native,
				_value);
		}

		#endregion
		#region Method SetItemResource

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportImageBox_SetItemResource__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _name);

		public bool SetItemResource(
			string _name)
		{
			return ExportImageBox_SetItemResource__name(Native,
				_name);
		}

		#endregion
		#region Method GetItemFrameRate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportImageBox_GetItemFrameRate__index(IntPtr _native,
			uint _index);

		public float GetItemFrameRate(
			uint _index)
		{
			return ExportImageBox_GetItemFrameRate__index(Native,
				_index);
		}

		#endregion
		#region Method SetItemFrameRate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemFrameRate__index__rate(IntPtr _native,
			uint _index,
			float _rate);

		public void SetItemFrameRate(
			uint _index,
			float _rate)
		{
			ExportImageBox_SetItemFrameRate__index__rate(Native,
				_index,
				_rate);
		}

		#endregion
		#region Method DeleteAllItemFrames

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteAllItemFrames__index(IntPtr _native,
			uint _index);

		public void DeleteAllItemFrames(
			uint _index)
		{
			ExportImageBox_DeleteAllItemFrames__index(Native,
				_index);
		}

		#endregion
		#region Method DeleteItemFrame

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteItemFrame__index__indexFrame(IntPtr _native,
			uint _index,
			uint _indexFrame);

		public void DeleteItemFrame(
			uint _index,
			uint _indexFrame)
		{
			ExportImageBox_DeleteItemFrame__index__indexFrame(Native,
				_index,
				_indexFrame);
		}

		#endregion
		#region Method SetItemFrame

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemFrame__index__indexFrame__item(IntPtr _native,
			uint _index,
			uint _indexFrame,
			[In] ref IntCoord _item);

		public void SetItemFrame(
			uint _index,
			uint _indexFrame,
			IntCoord _item)
		{
			ExportImageBox_SetItemFrame__index__indexFrame__item(Native,
				_index,
				_indexFrame,
				ref _item);
		}

		#endregion
		#region Method InsertItemFrameDublicate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItemFrameDublicate__index__indexFrame__indexSourceFrame(IntPtr _native,
			uint _index,
			uint _indexFrame,
			uint _indexSourceFrame);

		public void InsertItemFrameDublicate(
			uint _index,
			uint _indexFrame,
			uint _indexSourceFrame)
		{
			ExportImageBox_InsertItemFrameDublicate__index__indexFrame__indexSourceFrame(Native,
				_index,
				_indexFrame,
				_indexSourceFrame);
		}

		#endregion
		#region Method AddItemFrameDublicate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItemFrameDublicate__index__indexSourceFrame(IntPtr _native,
			uint _index,
			uint _indexSourceFrame);

		public void AddItemFrameDublicate(
			uint _index,
			uint _indexSourceFrame)
		{
			ExportImageBox_AddItemFrameDublicate__index__indexSourceFrame(Native,
				_index,
				_indexSourceFrame);
		}

		#endregion
		#region Method InsertItemFrame

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItemFrame__index__indexFrame__item(IntPtr _native,
			uint _index,
			uint _indexFrame,
			[In] ref IntCoord _item);

		public void InsertItemFrame(
			uint _index,
			uint _indexFrame,
			IntCoord _item)
		{
			ExportImageBox_InsertItemFrame__index__indexFrame__item(Native,
				_index,
				_indexFrame,
				ref _item);
		}

		#endregion
		#region Method AddItemFrame

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItemFrame__index__item(IntPtr _native,
			uint _index,
			[In] ref IntCoord _item);

		public void AddItemFrame(
			uint _index,
			IntCoord _item)
		{
			ExportImageBox_AddItemFrame__index__item(Native,
				_index,
				ref _item);
		}

		#endregion
		#region Method DeleteAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteAllItems(IntPtr _native);

		public void DeleteAllItems( )
		{
			ExportImageBox_DeleteAllItems(Native);
		}

		#endregion
		#region Method DeleteItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteItem__index(IntPtr _native,
			uint _index);

		public void DeleteItem(
			uint _index)
		{
			ExportImageBox_DeleteItem__index(Native,
				_index);
		}

		#endregion
		#region Method SetItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItem__index__item(IntPtr _native,
			uint _index,
			[In] ref IntCoord _item);

		public void SetItem(
			uint _index,
			IntCoord _item)
		{
			ExportImageBox_SetItem__index__item(Native,
				_index,
				ref _item);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItem__item(IntPtr _native,
			[In] ref IntCoord _item);

		public void AddItem(
			IntCoord _item)
		{
			ExportImageBox_AddItem__item(Native,
				ref _item);
		}

		#endregion
		#region Method InsertItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItem__index__item(IntPtr _native,
			uint _index,
			[In] ref IntCoord _item);

		public void InsertItem(
			uint _index,
			IntCoord _item)
		{
			ExportImageBox_InsertItem__index__item(Native,
				_index,
				ref _item);
		}

		#endregion
		#region Method ResetItemSelect

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_ResetItemSelect(IntPtr _native);

		public void ResetItemSelect( )
		{
			ExportImageBox_ResetItemSelect(Native);
		}

		#endregion
		#region Method SetImageTile

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageTile__value(IntPtr _native,
			ref IntSize _value);

		public void SetImageTile(
			IntSize _value)
		{
			ExportImageBox_SetImageTile__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetImageCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageCoord__value(IntPtr _native,
			[In] ref IntCoord _value);

		public void SetImageCoord(
			IntCoord _value)
		{
			ExportImageBox_SetImageCoord__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetImageRect

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageRect__value(IntPtr _native,
			[In] ref IntRect _value);

		public void SetImageRect(
			IntRect _value)
		{
			ExportImageBox_SetImageRect__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetImageTexture

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageTexture__value(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _value);

		public void SetImageTexture(
			string _value)
		{
			ExportImageBox_SetImageTexture__value(Native,
				_value);
		}

		#endregion
		#region Method SetImageInfo

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageInfo__texture__coord__tile(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _texture,
			[In] ref IntCoord _coord,
			ref IntSize _tile);

		public void SetImageInfo(
			string _texture,
			IntCoord _coord,
			IntSize _tile)
		{
			ExportImageBox_SetImageInfo__texture__coord__tile(Native,
				_texture,
				ref _coord,
				ref _tile);
		}

		#endregion
		#region Property ItemSelect

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetItemSelect(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemSelect(IntPtr _widget, uint _value);

		public uint ItemSelect
		{
			get { return ExportImageBox_GetItemSelect(Native); }
			set { ExportImageBox_SetItemSelect(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportImageBox_GetItemCount(Native); }
		}

		#endregion
		#region Property ImageSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportImageBox_GetImageSize(IntPtr _native);

		public IntSize ImageSize
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportImageBox_GetImageSize(Native), typeof(IntSize)); }
		}

		#endregion
		#region Property ImageIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetImageIndex(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageIndex(IntPtr _widget, uint _value);

		public uint ImageIndex
		{
			get { return ExportImageBox_GetImageIndex(Native); }
			set { ExportImageBox_SetImageIndex(Native, value); }
		}

		#endregion
		
    }
}
