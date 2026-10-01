#ifndef MYGUI_UNITTEST_FONT_TEST_CONTEXT_H_
#define MYGUI_UNITTEST_FONT_TEST_CONTEXT_H_

#include "TestSupport.h"
#include "MyGUI_DataFileStream.h"
#include "MyGUI_ResourceTrueTypeFont.h"
#include <fstream>
#include <map>
#include <memory>
#include <sstream>

namespace unittest
{

	inline void loadFontTestResources(std::string_view _xml, MyGUI::Version _version = MyGUI::Version(1, 2))
	{
		std::istringstream stream{std::string(_xml)};
		MyGUI::xml::Document document;
		require(document.open(stream), "Font test XML must parse");
		MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", _version);
	}

	inline MyGUI::TextBox* createFontTextBox(MyGUI::Gui& _gui, const MyGUI::IFont& _font)
	{
		loadFontTestResources(R"(<MyGUI type="Resource">
			<Resource type="ResourceSkin" name="FontTestText" size="800 600">
				<BasisSkin type="SimpleText" offset="0 0 800 600" align="Stretch"/>
			</Resource>
		</MyGUI>)");
		auto* text = _gui.createWidget<MyGUI::TextBox>(
			"FontTestText",
			MyGUI::IntCoord(0, 0, 800, 600),
			MyGUI::Align::Default,
			"");
		text->setFontName(_font.getResourceName());
		text->setFontHeight(0);
		text->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		return text;
	}

	// Store the bytes MyGUI uploads, without depending on a graphics driver.
	class FontTexture : public MyGUI::ITexture
	{
	public:
		explicit FontTexture(const std::string& _name) :
			mName(_name)
		{
		}
		const std::string& getName() const override
		{
			return mName;
		}
		int getWidth() const override
		{
			return mWidth;
		}
		int getHeight() const override
		{
			return mHeight;
		}
		MyGUI::PixelFormat getFormat() const override
		{
			return mFormat;
		}
		MyGUI::TextureUsage getUsage() const override
		{
			return mUsage;
		}
		size_t getNumElemBytes() const override
		{
			return mFormat == MyGUI::PixelFormat::L8A8 ? 2 : 4;
		}
		bool isLocked() const override
		{
			return mLocked;
		}
		void destroy() override
		{
			pixels.clear();
		}
		void setShader(const std::string& _shader) override
		{
			shader = _shader;
		}
		void setInvalidateListener(MyGUI::ITextureInvalidateListener* _listener) override
		{
			listener = _listener;
		}
		void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format) override
		{
			require(_width > 0 && _height > 0, "Font atlas must have positive dimensions");
			require(
				_format == MyGUI::PixelFormat::L8A8 || _format == MyGUI::PixelFormat::R8G8B8A8,
				"Unexpected font atlas format");
			mWidth = _width;
			mHeight = _height;
			mUsage = _usage;
			mFormat = _format;
			pixels.assign(size_t(_width) * _height * getNumElemBytes(), 0xCD);
		}
		void* lock(MyGUI::TextureUsage) override
		{
			require(!mLocked, "Font atlas must not be locked twice");
			mLocked = true;
			return pixels.data();
		}
		void unlock() override
		{
			require(mLocked, "Font atlas must be locked before upload completes");
			mLocked = false;
		}
		void loadFromFile(const std::string&) override
		{
			require(false, "Expected a generated atlas");
		}
		void saveToFile(const std::string&) override
		{
			require(false, "Tests do not save atlases");
		}

		std::vector<MyGUI::uint8> pixels;
		std::string shader;
		MyGUI::ITextureInvalidateListener* listener{nullptr};

	private:
		std::string mName;
		int mWidth{0};
		int mHeight{0};
		MyGUI::PixelFormat mFormat;
		MyGUI::TextureUsage mUsage;
		bool mLocked{false};
	};

	class FontRenderManager : public MyGUI::DummyRenderManager
	{
	public:
		MyGUI::ITexture* createTexture(const std::string& _name) override
		{
			auto texture = std::make_unique<FontTexture>(_name);
			auto* result = texture.get();
			require(textures.emplace(_name, std::move(texture)).second, "Font texture names must be unique");
			++created;
			return result;
		}
		void destroyTexture(MyGUI::ITexture* _texture) override
		{
			const auto name = _texture->getName();
			require(textures.erase(name) == 1, "Font must release an owned texture");
			++destroyed;
		}
		MyGUI::ITexture* getTexture(const std::string& _name) override
		{
			auto it = textures.find(_name);
			return it == textures.end() ? nullptr : it->second.get();
		}
		bool isFormatSupported(MyGUI::PixelFormat _format, MyGUI::TextureUsage) override
		{
			return _format == MyGUI::PixelFormat::R8G8B8A8 ||
				(supportsLuminanceAlpha && _format == MyGUI::PixelFormat::L8A8);
		}

		bool supportsLuminanceAlpha{true};
		int created{0};
		int destroyed{0};
		std::map<std::string, std::unique_ptr<FontTexture>> textures;
	};

	class FontDataManager : public MyGUI::DummyDataManager
	{
	public:
		MyGUI::IDataStream* getData(const std::string& _name) const override
		{
			if (_name != "DejaVuSans.ttf")
				return nullptr;
			auto stream = std::make_unique<std::ifstream>(getMediaPath(_name), std::ios::binary);
			require(stream->good(), "Cannot open bundled DejaVuSans.ttf");
			return new MyGUI::DataFileStream(std::move(stream));
		}
	};

	class FontTestContext
	{
	private:
		MyGUI::LogManager mLog;

	public:
		FontRenderManager renderer;
		FontDataManager data;
		MyGUI::Gui gui;

		FontTestContext()
		{
			renderer.initialise();
			renderer.setViewSize(800, 600);
			gui.initialise("");
		}
		~FontTestContext()
		{
			gui.shutdown();
			renderer.shutdown();
		}
		FontTestContext(const FontTestContext&) = delete;
		FontTestContext& operator=(const FontTestContext&) = delete;
	};

}

#endif // MYGUI_UNITTEST_FONT_TEST_CONTEXT_H_
