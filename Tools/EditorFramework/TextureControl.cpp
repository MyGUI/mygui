/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "TextureControl.h"
#include "Localise.h"
#include <cstring>

#if defined(MYGUI_OGRE_PLATFORM)
	#include <MyGUI_OgreRenderManager.h>
	#include <MyGUI_OgreDataManager.h>
	#include <OgreHighLevelGpuProgramManager.h>
	#include <OgreHighLevelGpuProgram.h>
	#include <OgreRenderSystem.h>
#elif defined(MYGUI_OGRENEXT_PLATFORM)
	#include <MyGUI_OgreNextRenderManager.h>
#endif

namespace tools
{

	bool TextureControl::hasTextureShader()
	{
		// Editors have one render-manager lifetime. Do not re-register programs
		// when another preview is opened: textures may already reference them.
		static const bool available = []
		{
			std::string vertex;
			std::string fragment;
#if defined(MYGUI_OPENGL3_PLATFORM)
			vertex = "MyGUI_OpenGL3_VP.glsl";
			fragment = "NearestFilter_FP.glsl";
#elif defined(MYGUI_OPENGLES_PLATFORM)
			vertex = "MyGUI_OpenGLES_VP.glsl";
			fragment = "NearestFilter_GLES_FP.glsl";
#elif defined(MYGUI_OSG_PLATFORM)
			vertex = "MyGUI_Osg_VP.glsl";
			fragment = "NearestFilter_FP.glsl";
#elif defined(MYGUI_DIRECTX11_PLATFORM)
			vertex = "MyGUI_DirectX11_VP.hlsl";
			fragment = "NearestFilter_FP.hlsl";
#elif defined(MYGUI_VULKAN_PLATFORM)
			vertex = "MyGUI_Vulkan_VP.spv";
			fragment = "NearestFilter_Vulkan_FP.spv";
#elif defined(MYGUI_OGRENEXT_PLATFORM)
			vertex = "mygui/VP";
			fragment = "NearestFilter_OgreNext_FP." + MyGUI::OgreNextRenderManager::getInstance().getShaderExtension();
#elif defined(MYGUI_OGRE_PLATFORM)
			auto& manager = MyGUI::OgreRenderManager::getInstance();
			if (manager.getRenderSystem()->getName() == "Direct3D9 Rendering Subsystem")
				return false;
			const auto extension = manager.getShaderExtension(true);
			vertex = "MyGUI_Ogre_VP." + extension;
			fragment = "NearestFilter_Ogre_FP." + extension;
			if (extension == "hlsl")
			{
				fragment = "NearestFilter_FP.hlsl";
				// Ogre's default loader targets SM3. Its existing-program path lets
				// this tools-only fragment shader use Texture.Load on D3D11.
				auto program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
					fragment,
					MyGUI::OgreDataManager::getInstance().getGroup(),
					"hlsl",
					Ogre::GPT_FRAGMENT_PROGRAM);
				program->setSourceFile(fragment);
				program->setParameter("target", "ps_4_0");
				program->setParameter("entry_point", "main");
				program->load();
			}
#endif
			if (vertex.empty())
				return false;
			MyGUI::RenderManager::getInstance().registerShader("Tools/NearestFilter", vertex, fragment);
			return true;
		}();
		return available;
	}

	void TextureControl::setTextureShader(MyGUI::ITexture* _texture)
	{
		if (_texture != nullptr && hasTextureShader())
			_texture->setShader("Tools/NearestFilter");
	}

	TextureControl::~TextureControl()
	{
		if (mTexture == nullptr)
			return;
		destroyNearestFilterTexture();
		mTexture->eventMouseWheel -= MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
		mTexture->eventMouseButtonPressed -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag -= MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
		mTexture->eventMouseMove -= MyGUI::newDelegate(this, &TextureControl::notifyMouseMove);
	}

	void TextureControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		hasTextureShader();
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackground, "Background");

		mTexture->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag += MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
		mTexture->eventMouseMove += MyGUI::newDelegate(this, &TextureControl::notifyMouseMove);
		mTexture->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
	}

	void TextureControl::updateScale()
	{
		double width = (double)mTextureRegion.width * mScaleValue;
		double height = (double)mTextureRegion.height * mScaleValue;

		mView->setCanvasSize(MyGUI::IntSize((int)width, (int)height));

		for (auto& selector : mSelectors)
			selector->setScale(mScaleValue);
	}

	void TextureControl::destroyNearestFilterTexture()
	{
		mTexture->setImageTexture(std::string_view{});
		if (mNearestFilterTexture != nullptr)
		{
			mNearestFilterTexture->setInvalidateListener(nullptr);
			MyGUI::RenderManager::getInstance().destroyTexture(mNearestFilterTexture);
			mNearestFilterTexture = nullptr;
		}
		mNearestFilterPixels.clear();
	}

	void TextureControl::textureInvalidate(MyGUI::ITexture* _texture)
	{
		void* pixels = _texture->lock(MyGUI::TextureUsage::Write);
		MYGUI_ASSERT(pixels != nullptr, "Cannot write preview texture");
		std::memcpy(pixels, mNearestFilterPixels.data(), mNearestFilterPixels.size());
		_texture->unlock();
		setTextureShader(_texture);
	}

	void TextureControl::createNearestFilterTexture(MyGUI::ITexture* _source)
	{
		auto& render = MyGUI::RenderManager::getInstance();
		// Retain a CPU snapshot for texture invalidation; never retain the
		// font's atlas pointer, since regeneration replaces the atlas.
		mNearestFilterPixels.resize(size_t(_source->getWidth()) * _source->getHeight() * _source->getNumElemBytes());
		const void* pixels = _source->lock(MyGUI::TextureUsage::Read);
		MYGUI_ASSERT(pixels != nullptr, "Cannot read preview source texture");
		std::memcpy(mNearestFilterPixels.data(), pixels, mNearestFilterPixels.size());
		_source->unlock();
		mNearestFilterTexture = render.createTexture(MyGUI::utility::toString("Tools/NearestFilter/", (size_t)this));
		try
		{
			mNearestFilterTexture->createManual(
				_source->getWidth(),
				_source->getHeight(),
				MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
				_source->getFormat());
			textureInvalidate(mNearestFilterTexture);
			mNearestFilterTexture->setInvalidateListener(this);
			// The stable preview name may now refer to a differently sized atlas.
			render.getTextureSize(mNearestFilterTexture->getName(), false);
			mTexture->setImageTexture(mNearestFilterTexture->getName());
		}
		catch (...)
		{
			destroyNearestFilterTexture();
			throw;
		}
	}

	void TextureControl::setTextureValue(const MyGUI::UString& _value, bool _copyTexture)
	{
		destroyNearestFilterTexture();
		auto& render = MyGUI::RenderManager::getInstance();
		mTextureSize = render.getTextureSize(_value, false);
		auto* source = _value.empty() ? nullptr : render.getTexture(_value);
		if (source != nullptr && mTextureSize.width > 0 && mTextureSize.height > 0)
		{
			if (_copyTexture)
			{
				createNearestFilterTexture(source);
			}
			else
			{
				setTextureShader(source);
				mTexture->setImageTexture(_value);
			}
		}
		mTextureRegion.set(0, 0, mTextureSize.width, mTextureSize.height);
		updateScale();
	}

	const MyGUI::IntSize& TextureControl::getTextureSize() const
	{
		return mTextureSize;
	}

	void TextureControl::setTextureRegion(const MyGUI::IntCoord& _value, float _dpiScale)
	{
		mTextureRegion = _value;

		MyGUI::IntCoord scaled(
			(int)(_value.left * _dpiScale),
			(int)(_value.top * _dpiScale),
			(int)(_value.width * _dpiScale),
			(int)(_value.height * _dpiScale));
		mTexture->setImageCoord(scaled);
		mTexture->setImageTile(scaled.size());
		mTexture->setImageIndex(0);

		updateScale();
	}

	const MyGUI::IntCoord& TextureControl::getTextureRegion() const
	{
		return mTextureRegion;
	}

	void TextureControl::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
	}

	void TextureControl::setColour(MyGUI::Colour _value)
	{
		mCurrentColour = _value;
		updateColours();
	}

	MyGUI::Colour TextureControl::getColour() const
	{
		return mCurrentColour;
	}

	void TextureControl::setScale(double _value)
	{
		mScaleValue = _value;
		updateScale();

		onChangeScale();
	}

	void TextureControl::onMouseMove()
	{
	}

	void TextureControl::onMouseWheel(int _rel)
	{
	}

	void TextureControl::onMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControl::onMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControl::onMouseDrag(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
	}

	bool TextureControl::getSelectorsCapture()
	{
		if (mMouseCapture)
			return true;

		for (auto& selector : mSelectors)
		{
			if (selector->getCapture())
				return true;
		}

		return false;
	}

	void TextureControl::notifyMouseWheel(MyGUI::Widget* _sender, int _rel)
	{
		mMouseLeftPressed = false;

		if (!getSelectorsCapture())
		{
			saveMouseRelative();
			onMouseWheel(_rel);
			loadMouseRelative();
		}
	}

	void TextureControl::registerSelectorControl(SelectorControl* _control)
	{
		mSelectors.push_back(_control);
		_control->setScale(mScaleValue);
		_control->getMainWidget()->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
		_control->getMainWidget()->eventMouseButtonPressed +=
			MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		_control->getMainWidget()->eventMouseButtonReleased +=
			MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		_control->getMainWidget()->eventMouseDrag += MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
		_control->getMainWidget()->eventMouseMove += MyGUI::newDelegate(this, &TextureControl::notifyMouseMove);
	}

	void TextureControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mMouseCapture = true;
			mRightMouseClick = MyGUI::InputManager::getInstance().getMousePositionByLayer();
			mViewOffset = mView->getViewOffset();

			mTexture->setPointer("hand");
			MyGUI::PointerManager::getInstance().setPointer("hand");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("hand");
		}
		else if (_id == MyGUI::MouseButton::Left)
		{
			mMouseLeftPressed = true;
			onMouseButtonPressed(getMousePosition());
		}
	}

	void TextureControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mMouseCapture = false;

			mTexture->setPointer("arrow");
			MyGUI::PointerManager::getInstance().setPointer("arrow");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("arrow");
		}
		else if (_id == MyGUI::MouseButton::Left)
		{
			if (!mMouseCapture && mMouseLeftPressed)
			{
				mMouseLeftPressed = false;
				onMouseButtonClick(getMousePosition());
			}
			onMouseButtonReleased(getMousePosition());
		}
	}

	void TextureControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		mMouseLeftPressed = false;

		if (_id == MyGUI::MouseButton::Right)
		{
			MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
			MyGUI::IntPoint mouseOffset = mousePoint - mRightMouseClick;

			MyGUI::IntPoint offset = mViewOffset + mouseOffset;
			mView->setViewOffset(offset);
		}
		else if (_id == MyGUI::MouseButton::Left)
		{
			onMouseDrag(getMousePosition());
		}
	}

	void TextureControl::notifyMouseMove(MyGUI::Widget* _sender, int _left, int _top)
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getLastPressedPosition(MyGUI::MouseButton::Left);
		if (point.left != _left && point.top != _top)
			onMouseMove();
	}

	void TextureControl::saveMouseRelative()
	{
		MyGUI::IntSize canvasSize = mView->getCanvasSize();
		MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
		MyGUI::IntPoint mouseOffset = mousePoint - mTexture->getAbsolutePosition();

		mMouseRelative.set(
			(float)mouseOffset.left / (float)canvasSize.width,
			(float)mouseOffset.top / (float)canvasSize.height);
	}

	void TextureControl::loadMouseRelative()
	{
		MyGUI::IntCoord viewCoord = mView->getViewCoord();
		MyGUI::IntSize canvasSize = mView->getCanvasSize();
		MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();

		// mouse offset relative to view
		MyGUI::IntPoint mouseOffset = mousePoint - mView->getAbsolutePosition() - viewCoord.point();
		// offset of target point inside texture in pixels
		MyGUI::IntPoint canvasPointOffset(
			(int)(mMouseRelative.left * (float)canvasSize.width),
			(int)(mMouseRelative.top * (float)canvasSize.height));
		// view offset in pixels
		MyGUI::IntPoint canvasOffset = canvasPointOffset - mouseOffset;

		mView->setViewOffset(MyGUI::IntPoint(-canvasOffset.left, -canvasOffset.top));
	}

	MyGUI::IntPoint TextureControl::getMousePosition()
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition() - mTexture->getAbsolutePosition();
		point.left = (int)((double)point.left / mScaleValue);
		point.top = (int)((double)point.top / mScaleValue);

		return point;
	}

	void TextureControl::onChangeScale()
	{
	}

	double TextureControl::getScale() const
	{
		return mScaleValue;
	}

	void TextureControl::removeSelectorControl(SelectorControl* _control)
	{
		mSelectors.erase(std::find(mSelectors.begin(), mSelectors.end(), _control));
		_control->Shutdown();
		delete _control;
	}

	void TextureControl::resetTextureRegion()
	{
		setTextureRegion(
			MyGUI::IntCoord(0, 0, mTextureSize.width, mTextureSize.height),
			MyGUI::Gui::getInstance().getDpiScale());
	}

}
