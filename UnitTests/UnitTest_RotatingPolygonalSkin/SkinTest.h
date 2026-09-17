#ifndef MYGUI_UNITTEST_ROTATING_POLYGONAL_SKIN_TEST_H_
#define MYGUI_UNITTEST_ROTATING_POLYGONAL_SKIN_TEST_H_

#include "SkinTestContext.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_CommonStateInfo.h"
#include <string>
#include <type_traits>

namespace unittest::customskin
{

	struct TestCase
	{
		std::string name;
		void (*run)();
	};
	using Tests = std::vector<TestCase>;
	void addRotatingSkinTests(Tests& _tests);
	void addPolygonalSkinTests(Tests& _tests);
	void addGeometryUtilityTests(Tests& _tests);

	// Keep overflow reproducers safe: logical capacity is distinct from guard storage.
	// These bounded cases emit at most 60 vertices; 128 guard vertices accommodate them.
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
			reservedVertices = _buffer->getVertexCount();
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
		size_t reservedVertices{0};

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
			setCoord({0, 0, 100, 100});
			skin._setUVSet({0.2f, 0.1f, 0.8f, 0.9f});
			if constexpr (std::is_same_v<Skin, MyGUI::PolygonalSkin>)
			{
				skin.setWidth(10);
				skin.setPoints({{10, 50}, {90, 50}});
			}
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
		Rectangle clip;
		Rectangle parent;
		MyGUI::LayerNode node{nullptr};
		Skin skin;

	private:
		bool mAttached{false};
	};

	template<typename Skin>
	void applyHalfTextureState(Skin& _skin, Renderer& _renderer)
	{
		MyGUI::xml::Document document;
		auto resource = document.createRoot("Resource");
		resource->addAttribute("texture", "MyGUI_BlueWhiteSkins.png");
		auto node = resource->createChild("BasisSkin")->createChild("State");
		auto* texture = _renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		node->addAttribute("offset", MyGUI::IntCoord(0, 0, texture->getWidth() / 2, texture->getHeight() / 2));
		using State = std::conditional_t<
			std::is_same_v<Skin, MyGUI::RotatingSkin>,
			MyGUI::RotatingSkinStateInfo,
			MyGUI::SubSkinStateInfo>;
		if constexpr (std::is_same_v<Skin, MyGUI::RotatingSkin>)
		{
			auto angle = node->createChild("Property");
			angle->addAttribute("key", "Angle");
			angle->addAttribute("value", "0.37");
			auto center = node->createChild("Property");
			center->addAttribute("key", "Center");
			center->addAttribute("value", "20 10");
		}
		State state;
		static_cast<MyGUI::IStateInfo&>(state).deserialization(node, MyGUI::Version(1, 0));
		_skin.setStateData(&state);
	}

	template<typename Skin>
	void testAppearanceAndLifetime()
	{
		for (auto format : {MyGUI::VertexColourType::ColourARGB, MyGUI::VertexColourType::ColourABGR})
		{
			Fixture<Skin> test(format);
			test.draw();
			const float area = test.area();
			require(area > 0, "Default skin must draw");
			test.skin.setAlpha(0.5f);
			test.skin._setColour(MyGUI::Colour::Red);
			test.draw();
			for (const auto& v : test.surface())
				require(
					v.colour == (format == MyGUI::VertexColourType::ColourARGB ? 0x7FFF0000U : 0x7F0000FFU),
					"Colour changes must preserve alpha and use native channel order");
			test.skin.setAlpha(1);
			test.draw();
			for (const auto& v : test.surface())
				require((v.colour >> 24) == 255, "Alpha change was not rendered");
			for (int repeat = 0; repeat < 2; ++repeat)
			{
				test.skin.setVisible(false);
				test.draw();
				require(test.renderer.vertices.empty(), "Hidden skin must not draw");
			}
			test.skin.setVisible(true);
			test.draw();
			near(test.area(), area, "Showing skin must restore geometry", 0.1f);
			test.detach();
			test.draw();
			require(test.renderer.vertices.empty(), "Detached skin must not draw");
			test.skin._setColour(MyGUI::Colour::White);
			test.attach();
			test.draw();
			near(test.area(), area, "Reattaching skin must restore geometry", 0.1f);
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

}
#endif
