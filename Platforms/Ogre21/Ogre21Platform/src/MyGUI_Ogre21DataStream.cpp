/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#include "MyGUI_Ogre21DataStream.h"

namespace MyGUI
{

	Ogre21DataStream::Ogre21DataStream(Ogre::DataStreamPtr _stream) :
		mStream(_stream)
	{
	}

	Ogre21DataStream::~Ogre21DataStream()
	{
		mStream.setNull();
	}

	bool Ogre21DataStream::eof()
	{
		return mStream.isNull() ? true : mStream->eof();
	}

	size_t Ogre21DataStream::size()
	{
		return mStream.isNull() ? 0 : mStream->size();
	}

	void Ogre21DataStream::readline(std::string& _source, Char _delim)
	{
		if (mStream.isNull())
		{
			_source.clear();
			return;
		}
		_source = mStream->getLine(false);
	}

	size_t Ogre21DataStream::read(void* _buf, size_t _count)
	{
		if (mStream.isNull()) return 0;
		return mStream->read(_buf, _count);
	}

} // namespace MyGUI
