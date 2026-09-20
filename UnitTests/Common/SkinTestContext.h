#ifndef MYGUI_UNITTEST_SKIN_TEST_CONTEXT_H_
#define MYGUI_UNITTEST_SKIN_TEST_CONTEXT_H_

#include "TestSupport.h"
#include "MyGUI_IVertexBuffer.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>

namespace unittest
{

	// The headless test needs the real atlas dimensions for UV conversion, but never samples pixels.
	class AtlasTexture : public MyGUI::ITexture
	{
	public:
		AtlasTexture()
		{
			loadFromFile(mName);
		}

		const std::string& getName() const override
		{
			return mName;
		}
		int getWidth() const override
		{
			return mSize.width;
		}
		int getHeight() const override
		{
			return mSize.height;
		}
		MyGUI::PixelFormat getFormat() const override
		{
			return MyGUI::PixelFormat::R8G8B8A8;
		}
		MyGUI::TextureUsage getUsage() const override
		{
			return MyGUI::TextureUsage::Static;
		}
		size_t getNumElemBytes() const override
		{
			return 4;
		}
		bool isLocked() const override
		{
			return false;
		}
		void destroy() override
		{
		}
		void setShader(const std::string&) override
		{
		}
		void unlock() override
		{
		}

		void loadFromFile(const std::string& _filename) override
		{
			std::ifstream stream(std::string(MYGUI_WIDGET_MEDIA_ROOT) + "/" + _filename, std::ios::binary);
			std::array<unsigned char, 24> header{};
			require(
				static_cast<bool>(stream.read(reinterpret_cast<char*>(header.data()), header.size())),
				"Cannot read skin atlas PNG header");
			const std::array<unsigned char, 8> signature{137, 80, 78, 71, 13, 10, 26, 10};
			require(std::equal(signature.begin(), signature.end(), header.begin()), "Invalid skin atlas PNG signature");
			require(
				header[12] == 'I' && header[13] == 'H' && header[14] == 'D' && header[15] == 'R',
				"Missing skin atlas IHDR");
			auto dimension = [&](size_t _offset)
			{
				unsigned int value = 0;
				for (size_t index = _offset; index < _offset + 4; ++index)
					value = (value << 8) | header[index];
				require(value > 0 && value <= 16384, "Invalid skin atlas dimension");
				return static_cast<int>(value);
			};
			mSize = {dimension(16), dimension(20)};
		}

		void createManual(int, int, MyGUI::TextureUsage, MyGUI::PixelFormat) override
		{
			require(false, "This test only supports the existing skin atlas");
		}
		void saveToFile(const std::string&) override
		{
			require(false, "The headless atlas cannot be saved");
		}
		void* lock(MyGUI::TextureUsage) override
		{
			require(false, "The headless atlas does not expose pixels");
			return nullptr;
		}

	private:
		std::string mName{"MyGUI_BlueWhiteSkins.png"};
		MyGUI::IntSize mSize;
	};

	class SkinVertexBuffer : public MyGUI::IVertexBuffer
	{
	public:
		void setVertexCount(size_t _count) override
		{
			vertices.resize(_count);
		}
		size_t getVertexCount() const override
		{
			return vertices.size();
		}
		MyGUI::Vertex* lock() override
		{
			return vertices.data();
		}
		void unlock() override
		{
		}
		std::vector<MyGUI::Vertex> vertices;
	};

	class SkinRenderManager : public MyGUI::DummyRenderManager
	{
	public:
		SkinRenderManager()
		{
			mInfo.pixScaleX = 1.0f / 800.0f;
			mInfo.pixScaleY = 1.0f / 600.0f;
			mInfo.aspectCoef = 600.0f / 800.0f;
		}

		const MyGUI::RenderTargetInfo& getInfo() const override
		{
			return mInfo;
		}

		MyGUI::IVertexBuffer* createVertexBuffer() override
		{
			return new SkinVertexBuffer();
		}
		void destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer) override
		{
			delete _buffer;
		}
		MyGUI::ITexture* getTexture(const std::string& _name) override
		{
			return _name == mAtlas.getName() ? &mAtlas : nullptr;
		}

		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			require(_texture == &mAtlas, "Unexpected draw texture");
			require(_count <= _buffer->getVertexCount() && _count % 3 == 0, "Invalid draw vertex count");
			const auto& vertices = static_cast<SkinVertexBuffer*>(_buffer)->vertices;
			for (size_t index = 0; index < _count; ++index)
			{
				const auto& vertex = vertices[index];
				require(
					std::isfinite(vertex.x) && std::isfinite(vertex.y) && std::isfinite(vertex.z) &&
						std::isfinite(vertex.u) && std::isfinite(vertex.v),
					"Skin generated non-finite vertices");
				++alphaCounts[vertex.colour >> 24];
			}
			drawnVertices += _count;
		}

		void render()
		{
			drawnVertices = 0;
			alphaCounts.clear();
			MyGUI::LayerManager::getInstance().renderToTarget(this, true);
		}

		size_t drawnVertices{0};
		std::map<MyGUI::uint32, size_t> alphaCounts;

	private:
		AtlasTexture mAtlas;
		MyGUI::RenderTargetInfo mInfo;
	};

	// A headless context because DummyPlatform intentionally has no texture or vertex storage.
	class SkinTestContext
	{
	public:
		SkinTestContext()
		{
			mRenderer.initialise();
			mRenderer.setViewSize(800, 600);
			mGui.initialise("");
		}

		~SkinTestContext()
		{
			mGui.shutdown();
			mRenderer.shutdown();
		}

		SkinTestContext(const SkinTestContext&) = delete;
		SkinTestContext& operator=(const SkinTestContext&) = delete;

		void loadSkins()
		{
			loadMedia("MyGUI_CommonSkins.xml");
			loadMedia("MyGUI_BlueWhiteSkins.xml");
		}

		void loadMedia(const std::string& _filename)
		{
			MyGUI::xml::Document document;
			require(
				document.open(std::string(MYGUI_WIDGET_MEDIA_ROOT) + "/" + _filename),
				"Cannot load MyGUI media skins");
			MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", MyGUI::Version(1, 1));
		}

		SkinRenderManager& renderer()
		{
			return mRenderer;
		}

	private:
		MyGUI::LogManager mLog;
		SkinRenderManager mRenderer;
		MyGUI::DummyDataManager mData;
		MyGUI::Gui mGui;
	};

}

#endif // MYGUI_UNITTEST_SKIN_TEST_CONTEXT_H_
