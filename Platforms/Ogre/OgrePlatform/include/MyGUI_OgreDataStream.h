/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/

#ifndef MYGUI_OGRE_DATA_STREAM_H_
#define MYGUI_OGRE_DATA_STREAM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IDataStream.h"

#include <OgreDataStream.h>

namespace MyGUI
{

	class OgreDataStream : public IDataStream
	{
	public:
		OgreDataStream(Ogre::DataStreamPtr _stream);
		~OgreDataStream() override;

		bool eof() override;
		size_t size() override;
		void readline(std::string& _source, Char _delim) override;
		size_t read(void* _buf, size_t _count) override;

	private:
		Ogre::DataStreamPtr mStream;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_DATA_STREAM_H_
