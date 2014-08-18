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
    public  class EditBox :
		TextBox
    {
        #region EditBox

        protected override string GetWidgetType() { return "EditBox"; }

        internal static BaseWidget RequestWrapEditBox(BaseWidget _parent, IntPtr _widget)
        {
			EditBox widget = new EditBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateEditBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			EditBox widget = new EditBox();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event EditTextChange

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBoxEvent_AdviseEditTextChange(IntPtr _native, bool _advise);

		public delegate void HandleEditTextChange(
			EditBox _sender);
			
		private HandleEditTextChange mEventEditTextChange;
		public event HandleEditTextChange EventEditTextChange
		{
			add
			{
				if (ExportEventEditTextChange.mDelegate == null)
				{
					ExportEventEditTextChange.mDelegate = new ExportEventEditTextChange.ExportHandle(OnExportEditTextChange);
					ExportEventEditTextChange.ExportEditBoxEvent_DelegateEditTextChange(ExportEventEditTextChange.mDelegate);
				}

				if (mEventEditTextChange == null)
					ExportEditBoxEvent_AdviseEditTextChange(Native, true);
				mEventEditTextChange += value;
			}
			remove
			{
				mEventEditTextChange -= value;
				if (mEventEditTextChange == null)
					ExportEditBoxEvent_AdviseEditTextChange(Native, false);
			}
		}

		private struct ExportEventEditTextChange
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportEditBoxEvent_DelegateEditTextChange(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportEditTextChange(
			IntPtr _sender)
		{
			EditBox sender = (EditBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventEditTextChange != null)
				sender.mEventEditTextChange(
					sender);
		}

		#endregion
		#region Event EditSelectAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBoxEvent_AdviseEditSelectAccept(IntPtr _native, bool _advise);

		public delegate void HandleEditSelectAccept(
			EditBox _sender);
			
		private HandleEditSelectAccept mEventEditSelectAccept;
		public event HandleEditSelectAccept EventEditSelectAccept
		{
			add
			{
				if (ExportEventEditSelectAccept.mDelegate == null)
				{
					ExportEventEditSelectAccept.mDelegate = new ExportEventEditSelectAccept.ExportHandle(OnExportEditSelectAccept);
					ExportEventEditSelectAccept.ExportEditBoxEvent_DelegateEditSelectAccept(ExportEventEditSelectAccept.mDelegate);
				}

				if (mEventEditSelectAccept == null)
					ExportEditBoxEvent_AdviseEditSelectAccept(Native, true);
				mEventEditSelectAccept += value;
			}
			remove
			{
				mEventEditSelectAccept -= value;
				if (mEventEditSelectAccept == null)
					ExportEditBoxEvent_AdviseEditSelectAccept(Native, false);
			}
		}

		private struct ExportEventEditSelectAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportEditBoxEvent_DelegateEditSelectAccept(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportEditSelectAccept(
			IntPtr _sender)
		{
			EditBox sender = (EditBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventEditSelectAccept != null)
				sender.mEventEditSelectAccept(
					sender);
		}

		#endregion
		#region Method SetPasswordChar

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetPasswordChar__char(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _char);

		public void SetPasswordChar(
			string _char)
		{
			ExportEditBox_SetPasswordChar__char(Native,
				_char);
		}

		#endregion
		#region Method EraseText

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_EraseText__start__count(IntPtr _native,
			uint _start,
			uint _count);

		public void EraseText(
			uint _start,
			uint _count)
		{
			ExportEditBox_EraseText__start__count(Native,
				_start,
				_count);
		}

		#endregion
		#region Method AddText

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_AddText__text(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _text);

		public void AddText(
			string _text)
		{
			ExportEditBox_AddText__text(Native,
				_text);
		}

		#endregion
		#region Method InsertText

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_InsertText__text__index(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _text,
			uint _index);

		public void InsertText(
			string _text,
			uint _index)
		{
			ExportEditBox_InsertText__text__index(Native,
				_text,
				_index);
		}

		#endregion
		#region Method SetTextSelectionColour

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetTextSelectionColour__value(IntPtr _native,
			[In] ref Colour _value);

		public void SetTextSelectionColour(
			Colour _value)
		{
			ExportEditBox_SetTextSelectionColour__value(Native,
				ref _value);
		}

		#endregion
		#region Method DeleteTextSelection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_DeleteTextSelection(IntPtr _native);

		public void DeleteTextSelection( )
		{
			ExportEditBox_DeleteTextSelection(Native);
		}

		#endregion
		#region Method SetTextSelection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetTextSelection__start__end(IntPtr _native,
			uint _start,
			uint _end);

		public void SetTextSelection(
			uint _start,
			uint _end)
		{
			ExportEditBox_SetTextSelection__start__end(Native,
				_start,
				_end);
		}

		#endregion
		#region Method GetTextInterval

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportEditBox_GetTextInterval__start__count(IntPtr _native,
			uint _start,
			uint _count);

		public string GetTextInterval(
			uint _start,
			uint _count)
		{
			return Marshal.PtrToStringUni(ExportEditBox_GetTextInterval__start__count(Native,
				_start,
				_count));
		}

		#endregion
		#region Method SetTextIntervalColour

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetTextIntervalColour__start__count__colour(IntPtr _native,
			uint _start,
			uint _count,
			[In] ref Colour _colour);

		public void SetTextIntervalColour(
			uint _start,
			uint _count,
			Colour _colour)
		{
			ExportEditBox_SetTextIntervalColour__start__count__colour(Native,
				_start,
				_count,
				ref _colour);
		}

		#endregion
		#region Property HScrollPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetHScrollPosition(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetHScrollPosition(IntPtr _widget, uint _value);

		public uint HScrollPosition
		{
			get { return ExportEditBox_GetHScrollPosition(Native); }
			set { ExportEditBox_SetHScrollPosition(Native, value); }
		}

		#endregion
		#region Property HScrollRange

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetHScrollRange(IntPtr _native);

		public uint HScrollRange
		{
			get { return ExportEditBox_GetHScrollRange(Native); }
		}

		#endregion
		#region Property IsVisibleHScroll

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_IsVisibleHScroll(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetVisibleHScroll(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool IsVisibleHScroll
		{
			get { return ExportEditBox_IsVisibleHScroll(Native); }
			set { ExportEditBox_SetVisibleHScroll(Native, value); }
		}

		#endregion
		#region Property VScrollPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetVScrollPosition(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetVScrollPosition(IntPtr _widget, uint _value);

		public uint VScrollPosition
		{
			get { return ExportEditBox_GetVScrollPosition(Native); }
			set { ExportEditBox_SetVScrollPosition(Native, value); }
		}

		#endregion
		#region Property VScrollRange

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetVScrollRange(IntPtr _native);

		public uint VScrollRange
		{
			get { return ExportEditBox_GetVScrollRange(Native); }
		}

		#endregion
		#region Property IsVisibleVScroll

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_IsVisibleVScroll(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetVisibleVScroll(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool IsVisibleVScroll
		{
			get { return ExportEditBox_IsVisibleVScroll(Native); }
			set { ExportEditBox_SetVisibleVScroll(Native, value); }
		}

		#endregion
		#region Property InvertSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetInvertSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetInvertSelected(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool InvertSelected
		{
			get { return ExportEditBox_GetInvertSelected(Native); }
			set { ExportEditBox_SetInvertSelected(Native, value); }
		}

		#endregion
		#region Property TabPrinting

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetTabPrinting(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetTabPrinting(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool TabPrinting
		{
			get { return ExportEditBox_GetTabPrinting(Native); }
			set { ExportEditBox_SetTabPrinting(Native, value); }
		}

		#endregion
		#region Property EditWordWrap

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetEditWordWrap(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetEditWordWrap(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool EditWordWrap
		{
			get { return ExportEditBox_GetEditWordWrap(Native); }
			set { ExportEditBox_SetEditWordWrap(Native, value); }
		}

		#endregion
		#region Property PasswordChar

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetPasswordChar(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetPasswordChar(IntPtr _widget, uint _value);

		public uint PasswordChar
		{
			get { return ExportEditBox_GetPasswordChar(Native); }
			set { ExportEditBox_SetPasswordChar(Native, value); }
		}

		#endregion
		#region Property EditStatic

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetEditStatic(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetEditStatic(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool EditStatic
		{
			get { return ExportEditBox_GetEditStatic(Native); }
			set { ExportEditBox_SetEditStatic(Native, value); }
		}

		#endregion
		#region Property EditMultiLine

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetEditMultiLine(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetEditMultiLine(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool EditMultiLine
		{
			get { return ExportEditBox_GetEditMultiLine(Native); }
			set { ExportEditBox_SetEditMultiLine(Native, value); }
		}

		#endregion
		#region Property EditPassword

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetEditPassword(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetEditPassword(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool EditPassword
		{
			get { return ExportEditBox_GetEditPassword(Native); }
			set { ExportEditBox_SetEditPassword(Native, value); }
		}

		#endregion
		#region Property EditReadOnly

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetEditReadOnly(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetEditReadOnly(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool EditReadOnly
		{
			get { return ExportEditBox_GetEditReadOnly(Native); }
			set { ExportEditBox_SetEditReadOnly(Native, value); }
		}

		#endregion
		#region Property MaxTextLength

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetMaxTextLength(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetMaxTextLength(IntPtr _widget, uint _value);

		public uint MaxTextLength
		{
			get { return ExportEditBox_GetMaxTextLength(Native); }
			set { ExportEditBox_SetMaxTextLength(Native, value); }
		}

		#endregion
		#region Property OverflowToTheLeft

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_GetOverflowToTheLeft(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetOverflowToTheLeft(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool OverflowToTheLeft
		{
			get { return ExportEditBox_GetOverflowToTheLeft(Native); }
			set { ExportEditBox_SetOverflowToTheLeft(Native, value); }
		}

		#endregion
		#region Property TextLength

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetTextLength(IntPtr _native);

		public uint TextLength
		{
			get { return ExportEditBox_GetTextLength(Native); }
		}

		#endregion
		#region Property OnlyText

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportEditBox_GetOnlyText(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetOnlyText(IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)] string _value);

		public string OnlyText
		{
			get { return Marshal.PtrToStringUni(ExportEditBox_GetOnlyText(Native)); }
			set { ExportEditBox_SetOnlyText(Native, value); }
		}

		#endregion
		#region Property TextCursor

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetTextCursor(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportEditBox_SetTextCursor(IntPtr _widget, uint _value);

		public uint TextCursor
		{
			get { return ExportEditBox_GetTextCursor(Native); }
			set { ExportEditBox_SetTextCursor(Native, value); }
		}

		#endregion
		#region Property IsTextSelection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportEditBox_IsTextSelection(IntPtr _native);

		public bool IsTextSelection
		{
			get { return ExportEditBox_IsTextSelection(Native); }
		}

		#endregion
		#region Property TextSelection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportEditBox_GetTextSelection(IntPtr _native);

		public string TextSelection
		{
			get { return Marshal.PtrToStringUni(ExportEditBox_GetTextSelection(Native)); }
		}

		#endregion
		#region Property TextSelectionLength

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetTextSelectionLength(IntPtr _native);

		public uint TextSelectionLength
		{
			get { return ExportEditBox_GetTextSelectionLength(Native); }
		}

		#endregion
		#region Property TextSelectionEnd

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetTextSelectionEnd(IntPtr _native);

		public uint TextSelectionEnd
		{
			get { return ExportEditBox_GetTextSelectionEnd(Native); }
		}

		#endregion
		#region Property TextSelectionStart

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportEditBox_GetTextSelectionStart(IntPtr _native);

		public uint TextSelectionStart
		{
			get { return ExportEditBox_GetTextSelectionStart(Native); }
		}

		#endregion
		
    }
}
