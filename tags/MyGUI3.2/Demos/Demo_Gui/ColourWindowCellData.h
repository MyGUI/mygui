/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __COLOUR_WINDOW_CELL_DATA_H__
#define __COLOUR_WINDOW_CELL_DATA_H__

#include <MyGUI.h>

namespace demo
{

	class ColourWindowCellData
	{
	public:
		ColourWindowCellData()
		{
		}

		ColourWindowCellData(const MyGUI::Colour& _colour, const std::string& _name) :
			m_colour(_colour),
			m_name(_name)
		{
		}

		const std::string& getName() const
		{
			return m_name;
		}

		const MyGUI::Colour& getColour() const
		{
			return m_colour;
		}

	private:
		MyGUI::Colour m_colour;
		std::string m_name;
	};

} // namespace demo

#endif // __COLOUR_WINDOW_CELL_DATA_H__
