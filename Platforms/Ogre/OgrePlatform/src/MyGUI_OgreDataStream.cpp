/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreDataStream.h"

namespace MyGUI
{

	OgreDataStream::OgreDataStream(Ogre::DataStreamPtr _stream) :
		mStream(_stream)
	{
	}

	OgreDataStream::~OgreDataStream()
	{
		mStream.setNull();
	}

	bool OgreDataStream::eof()
	{
		return mStream.isNull() ? true : mStream->eof();
	}

	size_t OgreDataStream::size()
	{
		return mStream.isNull() ? 0 : mStream->size();
	}

	void OgreDataStream::readline(std::string& _source, Char _delim)
	{
		if (mStream.isNull())
		{
			_source.clear();
			return;
		}
		_source = mStream->getLine(false);
	}

	size_t OgreDataStream::read(void* _buf, size_t _count)
	{
		if (mStream.isNull()) return 0;
		return mStream->read(_buf, _count);
	}

} // namespace MyGUI
