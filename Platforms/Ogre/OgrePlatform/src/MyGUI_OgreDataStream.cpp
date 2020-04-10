/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#include "MyGUI_OgreDataStream.h"

namespace MyGUI
{

	OgreDataStream::OgreDataStream(Ogre::DataStreamPtr _stream) :
		mStream(_stream)
	{
	}

	OgreDataStream::~OgreDataStream()
	{
		mStream.reset();
	}

	bool OgreDataStream::eof()
	{
		return !mStream ? true : mStream->eof();
	}

	size_t OgreDataStream::size()
	{
		return !mStream ? 0 : mStream->size();
	}

	void OgreDataStream::readline(std::string& _source, Char _delim)
	{
		if (!mStream)
		{
			_source.clear();
			return;
		}
		_source = mStream->getLine(false);
	}

	size_t OgreDataStream::read(void* _buf, size_t _count)
	{
		if (!mStream) return 0;
		return mStream->read(_buf, _count);
	}

} // namespace MyGUI
