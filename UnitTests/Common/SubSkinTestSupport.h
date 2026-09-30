#ifndef MYGUI_UNITTEST_SUB_SKIN_TEST_SUPPORT_H_
#define MYGUI_UNITTEST_SUB_SKIN_TEST_SUPPORT_H_

#include "SkinTestContext.h"
#include "TestRunner.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_SubSkin.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_PolygonalSkin.h"
#include "MyGUI_TileRect.h"
#include <string>

namespace unittest::subskin
{

	// Keep overflow reproducers safe: logical capacity is distinct from guard storage.
	// Extra vertices let bounded overflow regressions fail an assertion without corrupting memory.
	class GuardedBuffer : public MyGUI::IVertexBuffer
	{
	public:
		void setVertexCount(size_t _count) override
		{
			mCount = _count;
			vertices.resize(_count + 128);
		}
		size_t getVertexCount() const override
		{
			return mCount;
		}
		MyGUI::Vertex* lock() override
		{
			for (size_t i = mCount; i < vertices.size(); ++i)
				vertices[i].set(12345, 12345, 12345, 12345, 12345, 0x12345678);
			return vertices.data();
		}
		void unlock() override
		{
			for (size_t i = mCount; i < vertices.size(); ++i)
			{
				const auto& v = vertices[i];
				require(
					v.x == 12345 && v.y == 12345 && v.z == 12345 && v.u == 12345 && v.v == 12345 &&
						v.colour == 0x12345678,
					"Geometry wrote beyond its reserved vertex capacity");
			}
		}
		std::vector<MyGUI::Vertex> vertices;

	private:
		size_t mCount{0};
	};

	class Renderer : public SkinRenderManager
	{
	public:
		explicit Renderer(MyGUI::VertexColourType _format) :
			info(SkinRenderManager::getInfo()),
			mFormat(_format)
		{
			info.maximumDepth = 0.25f;
		}
		MyGUI::VertexColourType getVertexFormat() const override
		{
			return mFormat;
		}
		const MyGUI::RenderTargetInfo& getInfo() const override
		{
			return info;
		}
		MyGUI::IVertexBuffer* createVertexBuffer() override
		{
			return new GuardedBuffer();
		}
		void destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer) override
		{
			delete _buffer;
		}
		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			require(_texture == getTexture("MyGUI_BlueWhiteSkins.png"), "Unexpected skin texture");
			require(_count <= _buffer->getVertexCount() && _count % 3 == 0, "Invalid triangle count");
			const auto& source = static_cast<GuardedBuffer*>(_buffer)->vertices;
			for (size_t i = 0; i < _count; ++i)
			{
				const auto& v = source[i];
				require(
					std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z) && std::isfinite(v.u) &&
						std::isfinite(v.v),
					"Geometry contains non-finite positions or UVs");
				require(v.z == info.maximumDepth, "Geometry must use the layer node depth");
				vertices.push_back(v);
			}
		}
		MyGUI::RenderTargetInfo info;
		std::vector<MyGUI::Vertex> vertices;

	private:
		MyGUI::VertexColourType mFormat;
	};

	class Rectangle : public MyGUI::ICroppedRectangle
	{
	public:
		void setAbsolutePosition(const MyGUI::IntPoint& _position)
		{
			mAbsolutePosition = _position;
		}
		void setMargin(const MyGUI::IntRect& _margin)
		{
			mMargin = _margin;
		}
	};

	inline void near(float _actual, float _expected, const char* _message, float _tolerance = 0.002f)
	{
		require(
			std::abs(_actual - _expected) < _tolerance,
			std::string(_message) + ": expected " + std::to_string(_expected) + ", got " + std::to_string(_actual));
	}

	inline void initialiseSkin(MyGUI::SubSkin&, Renderer&)
	{
		// SubSkin and MainSkin need only coordinates and UVs.
	}

	inline void initialiseSkin(MyGUI::PolygonalSkin& _skin, Renderer&)
	{
		_skin.setWidth(10);
		_skin.setPoints({{10, 50}, {90, 50}});
	}

	inline void initialiseSkin(MyGUI::TileRect& _skin, Renderer& _renderer)
	{
		MyGUI::xml::Document document;
		auto resource = document.createRoot("Resource");
		resource->addAttribute("texture", "MyGUI_BlueWhiteSkins.png");
		auto node = resource->createChild("BasisSkin")->createChild("State");
		auto* texture = _renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		node->addAttribute("offset", MyGUI::IntCoord(0, 0, texture->getWidth(), texture->getHeight()));
		auto size = node->createChild("Property");
		size->addAttribute("key", "TileSize");
		size->addAttribute("value", MyGUI::IntSize(30, 20));
		MyGUI::TileRectStateInfo state;
		static_cast<MyGUI::IStateInfo&>(state).deserialization(node, MyGUI::Version(1, 0));
		_skin.setStateData(&state);
	}

	template<typename Skin>
	class Fixture
	{
	public:
		explicit Fixture(MyGUI::VertexColourType _format = MyGUI::VertexColourType::ColourARGB) :
			renderer(_format)
		{
			parent.setCoord({0, 0, 100, 100});
			parent.setAbsolutePosition({100, 60});
			skin._setCroppedParent(&parent);
			initialiseSkin(skin, renderer);
			setCoord({0, 0, 100, 100});
			skin._setUVSet({0.2f, 0.1f, 0.8f, 0.9f});
			attach();
		}
		~Fixture()
		{
			if (mAttached)
				skin.destroyDrawItem();
		}
		Fixture(const Fixture&) = delete;
		Fixture& operator=(const Fixture&) = delete;
		void attach()
		{
			skin.createDrawItem(renderer.getTexture("MyGUI_BlueWhiteSkins.png"), &node);
			mAttached = true;
		}
		void detach()
		{
			skin.destroyDrawItem();
			mAttached = false;
		}
		void setCoord(const MyGUI::IntCoord& _coord)
		{
			skin.setCoord(_coord);
			skin._setAlign(parent.getSize());
		}
		void draw(bool _update = false)
		{
			renderer.vertices.clear();
			// Ordinary draws must exercise invalidation; only target changes force a rebuild.
			node.renderToTarget(&renderer, _update);
		}
		MyGUI::FloatPoint local(const MyGUI::Vertex& _vertex) const
		{
			const auto& info = renderer.info;
			return {
				((_vertex.x + 1) / 2 - info.hOffset) / info.pixScaleX + info.leftOffset - parent.getAbsoluteLeft(),
				((1 - _vertex.y) / 2 - info.vOffset) / info.pixScaleY + info.topOffset - parent.getAbsoluteTop()};
		}
		// Ignore zero-area padding; tests do not prescribe triangulation or a fixed vertex count.
		std::vector<MyGUI::Vertex> surface() const
		{
			std::vector<MyGUI::Vertex> result;
			const auto& vertices = renderer.vertices;
			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				auto a = local(vertices[i]), b = local(vertices[i + 1]), c = local(vertices[i + 2]);
				const float cross = (b.left - a.left) * (c.top - a.top) - (b.top - a.top) * (c.left - a.left);
				if (std::abs(cross) > 0.001f)
					result.insert(result.end(), vertices.begin() + i, vertices.begin() + i + 3);
			}
			return result;
		}
		float area() const
		{
			float result = 0;
			const auto vertices = surface();
			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				auto a = local(vertices[i]), b = local(vertices[i + 1]), c = local(vertices[i + 2]);
				result += std::abs((b.left - a.left) * (c.top - a.top) - (b.top - a.top) * (c.left - a.left)) / 2;
			}
			return result;
		}
		void expectBounds(const MyGUI::FloatRect& _bounds) const
		{
			const auto vertices = surface();
			require(!vertices.empty(), "Expected visible geometry");
			auto first = local(vertices.front());
			MyGUI::FloatRect bounds(first.left, first.top, first.left, first.top);
			for (const auto& v : vertices)
			{
				auto p = local(v);
				bounds.left = std::min(bounds.left, p.left);
				bounds.right = std::max(bounds.right, p.left);
				bounds.top = std::min(bounds.top, p.top);
				bounds.bottom = std::max(bounds.bottom, p.top);
			}
			near(bounds.left, _bounds.left, "Unexpected left bound");
			near(bounds.right, _bounds.right, "Unexpected right bound");
			near(bounds.top, _bounds.top, "Unexpected top bound");
			near(bounds.bottom, _bounds.bottom, "Unexpected bottom bound");
		}
		Renderer renderer;
		Rectangle parent;
		MyGUI::LayerNode node{nullptr};
		Skin skin;

	private:
		bool mAttached{false};
	};

	template<typename Skin>
	void testAppearanceAndLifetime()
	{
		for (auto format : {MyGUI::VertexColourType::ColourARGB, MyGUI::VertexColourType::ColourABGR})
		{
			Fixture<Skin> test(format);
			test.draw();
			const auto original = test.surface();
			require(!original.empty(), "Default skin must draw");
			auto expectAppearance = [&](MyGUI::uint32 _colour)
			{
				test.draw();
				const auto vertices = test.surface();
				require(vertices.size() == original.size(), "Appearance changes must preserve geometry");
				for (size_t i = 0; i < vertices.size(); ++i)
				{
					const auto& vertex = vertices[i];
					const auto& before = original[i];
					require(
						vertex.x == before.x && vertex.y == before.y && vertex.z == before.z && vertex.u == before.u &&
							vertex.v == before.v,
						"Appearance changes must preserve positions and UVs");
					require(vertex.colour == _colour, "Unexpected packed colour or alpha");
				}
			};
			test.skin._setColour(MyGUI::Colour(1, 0.5f, 0.25f, 0));
			const MyGUI::uint32 rgb = format == MyGUI::VertexColourType::ColourARGB ? 0xFF7F3F : 0x3F7FFF;
			expectAppearance(0xFF000000 | rgb);
			for (float alpha : {0.5f, 0.0f, 1.0f})
			{
				test.skin.setAlpha(alpha);
				const auto packedAlpha = static_cast<MyGUI::uint32>(alpha * 255) << 24;
				expectAppearance(packedAlpha | rgb);
				test.skin._setColour(MyGUI::Colour(1, 0.5f, 0.25f, 0.75f));
				expectAppearance(packedAlpha | rgb);
			}
			for (int repeat = 0; repeat < 2; ++repeat)
			{
				test.skin.setVisible(false);
				test.draw();
				require(test.renderer.vertices.empty(), "Hidden skin must not draw");
			}
			test.skin.setVisible(true);
			expectAppearance(0xFF000000 | rgb);
			test.detach();
			test.draw();
			require(test.renderer.vertices.empty(), "Detached skin must not draw");
			test.skin._setColour(MyGUI::Colour::White);
			test.attach();
			expectAppearance(0xFFFFFFFF);
		}
	}

	template<typename Skin>
	void testViewCorrection()
	{
		Fixture<Skin> test;
		test.draw();
		const auto before = test.surface();
		require(!before.empty(), "Initial surface must be visible");
		test.parent.setAbsolutePosition({250, 130});
		test.skin._correctView();
		test.draw();
		const auto after = test.surface();
		require(after.size() == before.size(), "Moving parent changed topology");
		for (size_t i = 0; i < before.size(); ++i)
		{
			near(after[i].x - before[i].x, 300 * test.renderer.info.pixScaleX, "Parent X move was not applied");
			near(after[i].y - before[i].y, -140 * test.renderer.info.pixScaleY, "Parent Y move was not applied");
		}
	}

	template<typename Skin>
	void testTargetChange()
	{
		Fixture<Skin> test;
		test.draw();
		const auto before = test.surface();
		require(!before.empty(), "Initial surface must be visible");
		test.renderer.info.pixScaleX *= 2;
		test.renderer.info.pixScaleY *= 2;
		test.draw(true);
		const auto after = test.surface();
		require(after.size() == before.size(), "Target change altered topology");
		for (size_t i = 0; i < before.size(); ++i)
		{
			near(after[i].x, (before[i].x + 1) * 2 - 1, "Target scale update was ignored");
			near(after[i].y, 1 - (1 - before[i].y) * 2, "Target scale update was ignored");
		}
	}

	template<typename Skin>
	void testTargetOrigin()
	{
		Fixture<Skin> test;
		test.draw();
		const auto before = test.surface();
		require(!before.empty(), "Initial surface must be visible");
		test.renderer.info.setOffset(17, 23);
		test.skin._correctView();
		test.draw();
		const auto after = test.surface();
		require(after.size() == before.size(), "Target offset altered topology");
		for (size_t i = 0; i < before.size(); ++i)
		{
			near(after[i].x, before[i].x - 34 * test.renderer.info.pixScaleX, "Target left offset was ignored");
			near(after[i].y, before[i].y + 46 * test.renderer.info.pixScaleY, "Target top offset was ignored");
		}
	}

	template<typename Skin>
	void addCommonTests(std::vector<TestCase>& _tests)
	{
		const std::string name{Skin::getClassTypeName()};
		_tests.insert(
			_tests.end(),
			{
				{name + ".AppearanceAndLifetime", testAppearanceAndLifetime<Skin>},
				{name + ".ViewCorrection", testViewCorrection<Skin>},
				{name + ".TargetChange", testTargetChange<Skin>},
				{name + ".TargetOrigin", testTargetOrigin<Skin>},
			});
	}

}
#endif
