/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#include "MyGUI_OgreNextDataStream.h"

namespace MyGUI
{

	OgreNextDataStream::OgreNextDataStream(Ogre::DataStreamPtr _stream) :
		mStream(_stream)
	{
	}

	OgreNextDataStream::~OgreNextDataStream()
	{
		mStream.reset();
	}

	bool OgreNextDataStream::eof()
	{
		return mStream ? mStream->eof() : true;
	}

	size_t OgreNextDataStream::size()
	{
		return mStream ? mStream->size() : 0;
	}

	void OgreNextDataStream::readline(std::string& _source, Char _delim)
	{
		if (!mStream)
		{
			_source.clear();
			return;
		}
		_source = mStream->getLine(false);
	}

	size_t OgreNextDataStream::read(void* _buf, size_t _count)
	{
		if (!mStream)
			return 0;
		return mStream->read(_buf, _count);
	}

} // namespace MyGUI
