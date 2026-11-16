/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXT_TAG_H_
#define MYGUI_TEXT_TAG_H_

#include "MyGUI_UString.h"
#include <algorithm>

namespace MyGUI::text_detail
{

	constexpr size_t ColourTagLength = 7; // #RRGGBB

	// Preserve the legacy permissive syntax, including incomplete colour tags.
	// Leave the iterator on the last token character, or end for a lone '#'.
	inline bool advanceColourTag(UString::const_iterator& _iter, UString::const_iterator _end)
	{
		if (*_iter != U'#')
			return false;
		++_iter;
		if (_iter == _end)
			return false;
		if (*_iter != U'#')
		{
			const size_t remaining = static_cast<size_t>(_end - _iter);
			_iter += std::min(remaining, ColourTagLength - 1) - 1;
			return true;
		}
		return false;
	}

	inline bool advanceColourTag(UString::iterator& _iter, UString::const_iterator _end)
	{
		UString::const_iterator iter = _iter;
		const bool result = advanceColourTag(iter, _end);
		_iter += iter - _iter;
		return result;
	}

}

#endif // MYGUI_TEXT_TAG_H_
