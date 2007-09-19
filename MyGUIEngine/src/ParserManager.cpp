
#include "ParserManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(ParserManager);

	ParserManager::ParserManager() {}
	
	void ParserManager::parce(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
	{
		MapDelegate::iterator iter = m_delegates.find(_key);
		if (iter == m_delegates.end()) assert(0 && "name delegate is not find");
		iter->second(_widget, _key, _value);
	}

	ParceDelegate & ParserManager::registerDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = m_delegates.find(_key);
		if (iter != m_delegates.end()) assert(0 && "name delegate is exist");
		return (m_delegates[_key] = ParceDelegate());
	}

} // namespace MyGUI