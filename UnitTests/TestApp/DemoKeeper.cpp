#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "DataInfoManager.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "sigslot.h"

namespace demo
{
	class A
	{
	public:
		sigslot::signal1<A*> event;
	};

	class B :
		public sigslot::has_slots<>
	{
	public:
		void advise(A* _a)
		{
			if (!_a->event.exist(this, &B::onEvent))
				_a->event.connect(this, &B::onEvent);
		}

		void onEvent(A* _sender)
		{
		}
	};

	DemoKeeper::DemoKeeper() :
		mDataListUI(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp/DataInfo");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		new tools::DataInfoManager();
		new tools::DataManager();
		new tools::ActionManager();
		
		tools::DataInfoManager::getInstance().initialise();
		tools::DataManager::getInstance().initialise();
		tools::ActionManager::getInstance().initialise();

		tools::DataInfoManager::getInstance().load("ImageDataInfo.xml");

		mDataListUI = new DataListUI();

		A a;
		B b;

		b.advise(&a);
		b.advise(&a);


		MyGUI::IndexImage index;
		index.name = "MyIndex1";
		index.rate = 0;
		index.frames.push_back(MyGUI::IntPoint(0, 0));

		MyGUI::GroupImage group;
		group.name = "MyGroup1";
		group.texture = "MyTexture1.png";
		group.size = MyGUI::texture_utility::getTextureSize(group.texture, false);
		group.indexes.push_back(index);

		std::string category = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::ResourceImageSet* imageSet = MyGUI::FactoryManager::getInstance().createObject<MyGUI::ResourceImageSet>(category);
		imageSet->setResourceName("ResourceImageSet_Manual1");
		imageSet->AddGroupImage(group);

		MyGUI::ResourceManager::getInstance().addResource(imageSet);
	}

	void DemoKeeper::destroyScene()
	{
		tools::DataInfoManager::getInstance().shutdown();
		tools::DataManager::getInstance().shutdown();
		tools::ActionManager::getInstance().shutdown();

		delete tools::DataManager::getInstancePtr();
		delete tools::DataInfoManager::getInstancePtr();
		delete tools::ActionManager::getInstancePtr();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
