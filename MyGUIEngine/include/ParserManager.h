#ifndef _PARSERMANAGER_H_
#define _PARSERMANAGER_H_

#include "Prerequest.h"
#include <vector>
#include <Ogre.h>
#include "Widget.h"
#include "delegate.h"
#include "Instance.h"


namespace widget
{

	typedef CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParceDelegate;
	typedef std::map<Ogre::String, ParceDelegate> MapDelegate;

	class _MyGUIExport ParserManager
	{
	private:
		ParserManager() {};

	public:
		INSTANCE(ParserManager)

		void parce(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MapDelegate::iterator iter = m_delegates.find(_key);
			if (iter == m_delegates.end()) assert(0 && "name delegate is not find");
			iter->second(_widget, _key, _value);
		}

		ParceDelegate & registerDelegate(const Ogre::String & _key)
		{
			MapDelegate::iterator iter = m_delegates.find(_key);
			if (iter != m_delegates.end()) assert(0 && "name delegate is exist");
			return (m_delegates[_key] = ParceDelegate());
		}

	private:
		MapDelegate m_delegates;
	};

} // namespace widget

#endif
