/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SHADER_PARAM_H_
#define MYGUI_SHADER_PARAM_H_

#include <cstdint>
#include <string>
#include <string_view>

namespace MyGUI
{

	struct ShaderParam
	{
		std::string name;
		float values[4] = {};
		uint8_t count = 0; // 1=float, 2=vec2, 4=vec4
		mutable bool warned = false;

		ShaderParam() = default;

		ShaderParam(std::string_view _name, float _v) :
			name(_name),
			count(1)
		{
			values[0] = _v;
		}

		ShaderParam(std::string_view _name, float _x, float _y) :
			name(_name),
			count(2)
		{
			values[0] = _x;
			values[1] = _y;
		}

		ShaderParam(std::string_view _name, float _x, float _y, float _z, float _w) :
			name(_name),
			count(4)
		{
			values[0] = _x;
			values[1] = _y;
			values[2] = _z;
			values[3] = _w;
		}
	};

} // namespace MyGUI

#endif // MYGUI_SHADER_PARAM_H_
