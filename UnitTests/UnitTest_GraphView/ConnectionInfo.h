/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef CONNECTION_INFO_H_
#define CONNECTION_INFO_H_

#include <MyGUI.h>

namespace wraps
{

	struct ConnectionInfo
	{
		ConnectionInfo() = default;

		ConnectionInfo(
			const MyGUI::IntPoint& _point_start,
			const MyGUI::IntPoint& _point_end,
			const MyGUI::Colour& _colour,
			const MyGUI::IntSize& _start_offset,
			const MyGUI::IntSize& _end_offset) :
			point_start(_point_start),
			point_end(_point_end),
			colour(_colour),
			start_offset(_start_offset),
			end_offset(_end_offset)
		{
		}

		MyGUI::IntPoint point_start;
		MyGUI::IntPoint point_end;
		MyGUI::Colour colour;
		MyGUI::IntSize start_offset;
		MyGUI::IntSize end_offset;
	};

} // namespace wraps

#endif // CONNECTION_INFO_H_
