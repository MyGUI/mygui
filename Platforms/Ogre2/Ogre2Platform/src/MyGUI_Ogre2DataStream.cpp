/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#include "MyGUI_Ogre2DataStream.h"

namespace MyGUI
{

	Ogre2DataStream::Ogre2DataStream(Ogre::DataStreamPtr _stream) :
		mStream(_stream)
	{
	}

	Ogre2DataStream::~Ogre2DataStream()
	{
		mStream.setNull();
	}

	bool Ogre2DataStream::eof()
	{
		return mStream.isNull() ? true : mStream->eof();
	}

	size_t Ogre2DataStream::size()
	{
		return mStream.isNull() ? 0 : mStream->size();
	}

	void Ogre2DataStream::readline(std::string& _source, Char _delim)
	{
		if (mStream.isNull())
		{
			_source.clear();
			return;
		}
		_source = mStream->getLine(false);
	}

	size_t Ogre2DataStream::read(void* _buf, size_t _count)
	{
		if (mStream.isNull()) return 0;
		return mStream->read(_buf, _count);
	}

} // namespace MyGUI
