
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

		mDelegates.clear();

		// удаляем парсеры
		delete mWindowParser;
		delete mWidgetParser;


		mIsInitialise = false;
	}

	void ParserManager::parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter == mDelegates.end()) assert(0 && "name delegate is not find");
		iter->second(_widget, _key, _value);
	}

	ParseDelegate & ParserManager::registerDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) assert(0 && "name delegate is exist");
		return (mDelegates[_key] = ParseDelegate());
	}

	void ParserManager::unregisterDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) mDelegates.erase(iter);
	}

} // namespace MyGUI