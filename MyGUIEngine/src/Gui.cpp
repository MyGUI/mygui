
#include "Gui.h"
#include "Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		assert(!mIsInitialise);

		// ������������ ������� ������ � ������
		Ogre::OverlayManager &manager = Ogre::OverlayManager::getSingleton();
		manager.addOverlayElementFactory(&m_factoryTextSimpleOverlay);
		manager.addOverlayElementFactory(&m_factoryTextEditOverlay);
		manager.addOverlayElementFactory(&m_factoryPanelAlphaOverlay);

		Ogre::Viewport * port = _window->getViewport(0);
		m_height = port->getActualHeight();
		m_width = port->getActualWidth();

		// ������� � �������������� ���������
		mInputManager = new InputManager();
		mInputManager->initialise();
		mInputManager->load("main.lang");

		mBasisWidgetManager = new BasisWidgetManager();
		mBasisWidgetManager->initialise();

		mClipboardManager = new ClipboardManager();

		mLayerManager = new LayerManager();
		mLayerManager->initialise();
		mLayerManager->load("main.layer");

		mSkinManager = new SkinManager();
		mSkinManager->initialise();
		mSkinManager->load("main.skin");

		mParserManager = new ParserManager();
		mParserManager->initialise();

		mWidgetManager = new WidgetManager();
		mWidgetManager->initialise();

		mLayoutManager = new LayoutManager();

		mFontManager = new FontManager();
		mFontManager->load("MyGUI.font");

		mPointerManager = new PointerManager();
		mPointerManager->initialise();
		mPointerManager->load("main.pointer");
		mPointerManager->show();

		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (!mIsInitialise) return;

		// ���������������� � ������� ���������
		delete mPointerManager;

		mParserManager->shutdown();
		delete mParserManager;

		delete mFontManager;

		delete mLayoutManager;

		destroyWidget();
		mWidgetManager->shutdown();
		delete mWidgetManager;

		mInputManager->shutdown();
		delete mInputManager;

		mSkinManager->shutdown();
		delete mSkinManager;

		mLayerManager->shutdown();
		delete mLayerManager;

		mBasisWidgetManager->shutdown();
		delete mBasisWidgetManager;

		delete mClipboardManager;

		mIsInitialise = false;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, 0, _name);
		m_widgetChild.push_back(widget);
		// ������������ ������ � ������
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	// ������� ������ �������� ������� ��������
	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < m_widgetChild.size(); index++) {
			WidgetPtr widget = m_widgetChild[index];
			if (_widget == widget) {
				// ������� ���� ���
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// ������� �� ������
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1];
				m_widgetChild.pop_back();
				return;
			}
		}
	}

	// ������� ���� �����
	void Gui::destroyWidget()
	{
		for (VectorWidgetPtr::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// ������� ���� ���
			WidgetManager::getInstance().clearName(widget);
			// ����������� ������ �� ������, ���� �� ��� �����������
			LayerManager::getInstance().detachItem(widget);
			// � �������
			delete widget;
		}
		m_widgetChild.clear();
	}

} // namespace MyGUI