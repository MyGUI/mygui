#pragma once

#include "BehaviourTestSupport.h"
#include "SkinTestContext.h"
#include <memory>

namespace unittest
{

	class CanvasTexture : public MyGUI::ITexture
	{
	public:
		explicit CanvasTexture(std::string _name) :
			name(std::move(_name))
		{
		}
		const std::string& getName() const override
		{
			return name;
		}
		void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format) override
		{
			require(
				_width > 0 && _height > 0 && _width <= 1024 && _height <= 1024,
				"Canvas texture size must be bounded");
			size = {_width, _height};
			usage = _usage;
			format = _format;
			pixels.assign(size_t(_width) * _height * 4, 0);
		}
		void loadFromFile(const std::string&) override
		{
			require(false, "Canvas fixture does not load texture files");
		}
		void saveToFile(const std::string&) override
		{
			require(false, "Canvas fixture does not save texture files");
		}
		void setShader(const std::string&) override
		{
		}
		void setInvalidateListener(MyGUI::ITextureInvalidateListener* _listener) override
		{
			listener = _listener;
		}
		void destroy() override
		{
			pixels.clear();
		}
		void* lock(MyGUI::TextureUsage) override
		{
			require(!locked, "Texture must not be locked twice");
			locked = true;
			return pixels.data();
		}
		void unlock() override
		{
			require(locked, "Texture must be locked before unlocking");
			locked = false;
		}
		bool isLocked() const override
		{
			return locked;
		}
		int getWidth() const override
		{
			return size.width;
		}
		int getHeight() const override
		{
			return size.height;
		}
		MyGUI::PixelFormat getFormat() const override
		{
			return format;
		}
		MyGUI::TextureUsage getUsage() const override
		{
			return usage;
		}
		size_t getNumElemBytes() const override
		{
			return 4;
		}
		std::string name;
		MyGUI::IntSize size;
		MyGUI::TextureUsage usage;
		MyGUI::PixelFormat format;
		std::vector<unsigned char> pixels;
		bool locked{false};
		MyGUI::ITextureInvalidateListener* listener{nullptr};
	};

	class CanvasRenderer : public SkinRenderManager
	{
	public:
		MyGUI::ITexture* createTexture(const std::string& _name) override
		{
			require(textures.count(_name) == 0, "Canvas must release the old texture before recreating its name");
			auto texture = std::make_unique<CanvasTexture>(_name);
			auto* result = texture.get();
			textures.emplace(_name, std::move(texture));
			++created;
			return result;
		}
		void destroyTexture(MyGUI::ITexture* _texture) override
		{
			require(_texture && !_texture->isLocked(), "Only unlocked owned textures may be destroyed");
			auto iter = textures.find(_texture->getName());
			require(iter != textures.end() && iter->second.get() == _texture, "Texture must be owned by this renderer");
			textures.erase(iter);
			++destroyed;
		}
		MyGUI::ITexture* getTexture(const std::string& _name) override
		{
			auto iter = textures.find(_name);
			return iter == textures.end() ? SkinRenderManager::getTexture(_name) : iter->second.get();
		}
		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			require(
				_texture && getTexture(_texture->getName()) == _texture,
				"Canvas draw must reference a live texture");
			require(_count <= _buffer->getVertexCount(), "Canvas draw must fit its vertex buffer");
			const auto& vertices = static_cast<SkinVertexBuffer*>(_buffer)->vertices;
			for (size_t i = 0; i < _count; ++i)
				require(std::isfinite(vertices[i].u) && std::isfinite(vertices[i].v), "Canvas UVs must be finite");
			drawnVertices += _count;
		}
		std::map<std::string, std::unique_ptr<CanvasTexture>> textures;
		size_t created{0}, destroyed{0};
	};

	struct CanvasFixture
	{
		std::vector<MyGUI::Canvas::Event> updates;
		size_t preChanges{0};
		MyGUI::LogManager log;
		CanvasRenderer renderer;
		MyGUI::DummyDataManager data;
		MyGUI::Gui gui;
		MyGUI::Canvas* canvas;

		CanvasFixture()
		{
			renderer.initialise();
			data.initialise();
			renderer.setViewSize(800, 600);
			gui.initialise("");
			createInputLayer();
			MyGUI::xml::Document document;
			require(
				document.open(std::string(MYGUI_WIDGET_MEDIA_ROOT) + "/MyGUI_BlueWhiteSkins.xml"),
				"Cannot load canvas skin");
			MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", MyGUI::Version(1, 1));
			canvas = gui.createWidget<MyGUI::Canvas>(
				"WhiteSkin",
				MyGUI::IntCoord(20, 20, 65, 33),
				MyGUI::Align::Default,
				"Main");
			canvas->eventPreTextureChanges += MyGUI::newDelegate(this, &CanvasFixture::before);
			canvas->requestUpdateCanvas = MyGUI::newDelegate(this, &CanvasFixture::update);
		}
		~CanvasFixture()
		{
			gui.shutdown();
			data.shutdown();
			renderer.shutdown();
		}
		void before(MyGUI::Canvas* _canvas)
		{
			require(_canvas->isTextureCreated(), "Pre-change callback must still see the old texture");
			++preChanges;
		}
		void update(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
		{
			require(_canvas->isTextureCreated(), "Update callback must see a valid texture");
			updates.push_back(_event);
		}
		void frame()
		{
			gui.eventFrameStart(0.016f);
		}
		void finish()
		{
			gui.destroyWidget(canvas);
			canvas = nullptr;
			frame();
			renderer.render();
			require(
				renderer.textures.empty() && renderer.created == renderer.destroyed,
				"Canvas destruction must release all textures");
			require(renderer.drawnVertices == 0, "Canvas destruction must release all draw geometry");
		}
	};

}
