/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#ifndef MYGUI_OGRE2_DATA_STREAM_H_
#define MYGUI_OGRE2_DATA_STREAM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IDataStream.h"

#include <OgreDataStream.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class Ogre2DataStream :
		public IDataStream
	{
	public:
		Ogre2DataStream(Ogre::DataStreamPtr _stream);
		virtual ~Ogre2DataStream();

		virtual bool eof();
		virtual size_t size();
		virtual void readline(std::string& _source, Char _delim);
		virtual size_t read(void* _buf, size_t _count);

	private:
		Ogre::DataStreamPtr mStream;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_DATA_STREAM_H_
