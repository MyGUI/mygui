/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#ifndef MYGUI_OGRE21_DATA_STREAM_H_
#define MYGUI_OGRE21_DATA_STREAM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IDataStream.h"

#include <OgreDataStream.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class Ogre21DataStream :
		public IDataStream
	{
	public:
		Ogre21DataStream(Ogre::DataStreamPtr _stream);
		virtual ~Ogre21DataStream();

		virtual bool eof();
		virtual size_t size();
		virtual void readline(std::string& _source, Char _delim);
		virtual size_t read(void* _buf, size_t _count);

	private:
		Ogre::DataStreamPtr mStream;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_DATA_STREAM_H_
