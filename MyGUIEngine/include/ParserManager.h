#ifndef _PARSERMANAGER_H_
#define _PARSERMANAGER_H_

#include "Prerequest.h"
#include "Instance.h"
#include <vector>
#include "Widget.h"
#include "delegate.h"


namespace MyGUI
{

	typedef CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParceDelegate;
	typedef std::map<Ogre::String, ParceDelegate> MapDelegate;

	class _MyGUIExport ParserManager
	{
		INSTANCE_HEADER(ParserManager);

	private:
		ParserManager();

	public:
		void parce(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		ParceDelegate & registerDelegate(const Ogre::String & _key);

	private:
		MapDelegate m_delegates;
	};

} // namespace MyGUI

#endif
