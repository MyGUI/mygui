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
    public  class Button :
		TextBox
    {
        #region Button

        protected override string GetWidgetType() { return "Button"; }

        internal static BaseWidget RequestWrapButton(BaseWidget _parent, IntPtr _widget)
        {
			Button widget = new Button();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateButton(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			Button widget = new Button();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetImageName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageName__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _name);

		public void SetImageName(
			string _name)
		{
			ExportButton_SetImageName__name(Native,
				_name);
		}

		#endregion
		#region Method SetImageGroup

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageGroup__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _name);

		public void SetImageGroup(
			string _name)
		{
			ExportButton_SetImageGroup__name(Native,
				_name);
		}

		#endregion
		#region Method SetImageResource

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageResource__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _name);

		public void SetImageResource(
			string _name)
		{
			ExportButton_SetImageResource__name(Native,
				_name);
		}

		#endregion
		#region Property ModeImage

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportButton_GetModeImage(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetModeImage(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool ModeImage
		{
			get { return ExportButton_GetModeImage(Native); }
			set { ExportButton_SetModeImage(Native, value); }
		}

		#endregion
		#region Property StateSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportButton_GetStateSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetStateSelected(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool StateSelected
		{
			get { return ExportButton_GetStateSelected(Native); }
			set { ExportButton_SetStateSelected(Native, value); }
		}

		#endregion
		
    }
}
