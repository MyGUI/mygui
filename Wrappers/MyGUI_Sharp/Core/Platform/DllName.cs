using System;

namespace MyGUI.Sharp
{
	public static class DllName
	{
#if DEBUG
		public const string m_dllName = "MyGUI_Export_d.dll";
#else
		public const string m_dllName = "MyGUI_Export.dll";
#endif
	}
}
