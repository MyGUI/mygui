
#include "ParserManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(ParserManager);

	void ParserManager::initialise()
	{
		assert(!mIsInitialise);

		// создаем парсеры виджетов
		mWidgetParser = new parser::WidgetParser();
		mWindowParser = new parser::WindowParser();

		mIsInitialise = true;
	}

	void ParserManager::shutdown()
	{
		if (!mIsInitialise) return;

		m_delegates.clear();

		// удаляем парсеры
		delete mWindowParser;
		delete mWidgetParser;


		mIsInitialise = false;
	}

	void ParserManager::parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
	{
		MapDelegate::iterator iter = m_delegates.find(_key);
		if (iter == m_delegates.end()) assert(0 && "name delegate is not find");
		iter->second(_widget, _key, _value);
	}

	ParseDelegate & ParserManager::registerDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = m_delegates.find(_key);
		if (iter != m_delegates.end()) assert(0 && "name delegate is exist");
		return (m_delegates[_key] = ParseDelegate());
	}

} // namespace MyGUI