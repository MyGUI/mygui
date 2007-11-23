
#include "BasisWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(BasisWidgetManager);

	void BasisWidgetManager::initialise()
	{
		assert(!mIsInitialise);

		mFactoryMainSkin = new BasisWidgetFactory<MainSkin>();
		registerFactory(mFactoryMainSkin);

		mFactorySubSkin = new BasisWidgetFactory<SubSkin>();
		registerFactory(mFactorySubSkin);

		mFactoryTextSimple = new BasisWidgetFactory<TextSimple>();
		registerFactory(mFactoryTextSimple);

		mFactoryTextEdit = new BasisWidgetFactory<TextEdit>();
		registerFactory(mFactoryTextEdit);

		mIsInitialise = true;
	}

	void BasisWidgetManager::shutdown()
	{
		if (!mIsInitialise) return;

		m_factoryList.clear();

		delete mFactoryMainSkin;
		delete mFactorySubSkin;
		delete mFactoryTextSimple;
		delete mFactoryTextEdit;

		mIsInitialise = false;
	}

	BasisWidget * BasisWidgetManager::createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent)
	{
		for (std::list<BasisWidgetFactoryInterface*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
		}
		OGRE_EXCEPT(0, _info.type + " - no find factory BasisWidgetFactory", "BasisWidgetManager::createBasisWidget");
		return 0;
	}

} // namespace MyGUI