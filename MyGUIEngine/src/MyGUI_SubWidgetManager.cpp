/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SubWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(SubWidgetManager);

	void SubWidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mFactorySubSkin = new SubWidgetFactory<SubSkin>("SubSkin");
		mFactoryMainSkin = new SubWidgetFactory<MainSkin>("MainSkin");
		mFactorySimpleText = new SubWidgetFactory<SimpleText>("SimpleText");
		mFactoryEditText = new SubWidgetFactory<EditText>("EditText");
		mFactoryColourRect = new SubWidgetFactory<ColourRect>("ColourRect");
		mFactoryTileRect = new SubWidgetFactory<TileRect>("TileRect");

		registerFactory(mFactorySubSkin);
		registerFactory(mFactoryMainSkin);
		registerFactory(mFactorySimpleText);
		registerFactory(mFactoryEditText);
		registerFactory(mFactoryColourRect);
		registerFactory(mFactoryTileRect);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SubWidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mFactoryList.clear();

		delete mFactorySubSkin;
		delete mFactoryMainSkin;
		delete mFactorySimpleText;
		delete mFactoryEditText;
		delete mFactoryColourRect;
		delete mFactoryTileRect;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	SubWidgetInterface * SubWidgetManager::createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent)
	{
		for (std::list<SubWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createSubWidget(_info, _parent);
		}
		MYGUI_EXCEPT("factory type '" << _info.type << "' not found.");
		return 0;
	}

	void * SubWidgetManager::getStateData(const std::string & _factory, xml::xmlNodePtr _node, xml::xmlNodePtr _root)
	{
		for (std::list<SubWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _factory) return (*factory)->createData(_node, _root);
		}
		MYGUI_EXCEPT("factory type '" << _factory << "' not found.");
		return 0;
	}

} // namespace MyGUI
