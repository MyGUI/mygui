using System;
using System.Collections.Generic;
using System.Text;

namespace MyGUI.Sharp
{
	public interface IPlatformRenderManager
	{
		void GetTextureSize(string _name, out int _width, out int _height);
	}
}
