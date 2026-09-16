#include "SkinTestContext.h"
#include "MyGUI_SubSkin.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_LayerNode.h"
#include <iostream>

namespace
{

	using unittest::require;

	class Renderer : public unittest::SkinRenderManager
	{
	public:
		explicit Renderer(MyGUI::VertexColourType _format) :
			mFormat(_format),
			info(unittest::SkinRenderManager::getInfo())
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

		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			unittest::SkinRenderManager::doRender(_buffer, _texture, _count);
			const auto& source = static_cast<unittest::SkinVertexBuffer*>(_buffer)->vertices;
			vertices.insert(vertices.end(), source.begin(), source.begin() + _count);
		}

	private:
		MyGUI::VertexColourType mFormat;

	public:
		MyGUI::RenderTargetInfo info;
		std::vector<MyGUI::Vertex> vertices;
	};

	class ParentRectangle : public MyGUI::ICroppedRectangle
	{
	public:
		ParentRectangle()
		{
			setSize({100, 80});
			mAbsolutePosition = {100, 60};
		}

		void setAbsolutePosition(const MyGUI::IntPoint& _position)
		{
			mAbsolutePosition = _position;
		}

		void setMargin(const MyGUI::IntRect& _margin)
		{
			mMargin = _margin;
		}
	};

	class Fixture
	{
	public:
		explicit Fixture(MyGUI::VertexColourType _format = MyGUI::VertexColourType::ColourARGB) :
			renderer(_format)
		{
			skin._setCroppedParent(&parent);
			skin.setAlign(MyGUI::Align::Default);
			setCoord({10, 20, 40, 30});
			skin._setUVSet({0.2f, 0.1f, 0.8f, 0.9f});
			attach();
		}

		~Fixture()
		{
			if (mAttached)
				skin.destroyDrawItem();
		}

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
			// As in Widget, alignment initializes the current rectangle before updating its clipping.
			skin._setAlign(parent.getSize());
		}

		void draw()
		{
			renderer.vertices.clear();
			// Exercise invalidation: a forced rebuild would conceal stale geometry after a setter.
			node.renderToTarget(&renderer, false);
		}

		void expectQuad(const MyGUI::IntCoord& _coord, const MyGUI::FloatRect& _uv, MyGUI::uint32 _colour = 0xFFFFFFFF)
		{
			draw();
			require(renderer.vertices.size() == 6, "A visible SubSkin must emit two triangles");
			const bool right[] = {false, true, false, false, true, true};
			const bool bottom[] = {false, false, true, true, false, true};
			for (size_t index = 0; index < 6; ++index)
			{
				const auto& vertex = renderer.vertices[index];
				// Convert back to screen pixels to check the public geometry contract.
				const auto& info = renderer.info;
				const float x = ((vertex.x + 1) / 2 - info.hOffset) / info.pixScaleX + info.leftOffset;
				const float y = ((1 - vertex.y) / 2 - info.vOffset) / info.pixScaleY + info.topOffset;
				require(
					std::abs(x - (right[index] ? _coord.right() : _coord.left)) < 0.001f &&
						std::abs(y - (bottom[index] ? _coord.bottom() : _coord.top)) < 0.001f,
					"SubSkin triangle positions or winding differ from the expected rectangle");
				require(
					std::abs(vertex.u - (right[index] ? _uv.right : _uv.left)) < 0.00001f &&
						std::abs(vertex.v - (bottom[index] ? _uv.bottom : _uv.top)) < 0.00001f,
					"SubSkin UVs differ from the expected texture rectangle");
				require(vertex.z == info.maximumDepth, "SubSkin must use the layer node depth");
				require(vertex.colour == _colour, "SubSkin packed colour or alpha is incorrect");
			}
		}

		Renderer renderer;
		ParentRectangle parent;
		MyGUI::LayerNode node{nullptr};
		MyGUI::SubSkin skin;

	private:
		bool mAttached{false};
	};

	void testGeometryAndViewCorrection()
	{
		Fixture test;
		test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f});
		test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f});
		test.renderer.info.leftOffset = 17;
		test.renderer.info.topOffset = 23;
		test.renderer.info.hOffset = 0.5f / 800;
		test.renderer.info.vOffset = 0.5f / 600;
		test.parent.setAbsolutePosition({200, 150});
		test.skin._correctView();
		test.expectQuad({210, 170, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f});
	}

	void testClippingAndUVChanges()
	{
		Fixture test;
		test.setCoord({-20, -10, 160, 100});
		test.expectQuad({100, 60, 100, 80}, {0.275f, 0.18f, 0.65f, 0.82f});
		test.skin._setUVSet({0, 0.2f, 1, 0.7f});
		test.expectQuad({100, 60, 100, 80}, {0.125f, 0.25f, 0.75f, 0.65f});
		// Reapplying the UV rectangle must keep the clipped mapping intact.
		test.skin._setUVSet({0, 0.2f, 1, 0.7f});
		test.expectQuad({100, 60, 100, 80}, {0.125f, 0.25f, 0.75f, 0.65f});
		test.parent.setMargin({10, 5, 20, 15});
		test.skin._updateView();
		test.expectQuad({110, 65, 70, 60}, {0.1875f, 0.275f, 0.625f, 0.575f});
		test.parent.setMargin({0, 0, 0, 0});
		test.parent.setSize({200, 120});
		test.setCoord({10, 10, 160, 100});
		test.expectQuad({110, 70, 160, 100}, {0, 0.2f, 1, 0.7f});
		// Flipped UVs are valid and must retain their orientation when clipped.
		test.skin._setUVSet({1, 1, 0, 0});
		test.setCoord({-40, -20, 160, 100});
		test.expectQuad({100, 60, 120, 80}, {0.75f, 0.8f, 0, 0});
	}

	void testEmptyAndHiddenViews()
	{
		Fixture test;
		const MyGUI::IntCoord empty[] = {
			{-50, 10, 40, 30},
			{110, 10, 40, 30},
			{10, -40, 40, 30},
			{10, 90, 40, 30},
			{-40, 10, 40, 30},
			{100, 10, 40, 30},
			{10, -30, 40, 30},
			{10, 80, 40, 30},
			{10, 20, 0, 30},
			{10, 20, 40, 0}};
		for (const auto& coord : empty)
		{
			test.setCoord(coord);
			test.draw();
			require(test.renderer.vertices.empty(), "Outside, edge-touching and zero-size skins must not draw");
			test.setCoord({10, 20, 40, 30});
			test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f});
		}
		for (int repeat = 0; repeat < 2; ++repeat)
		{
			test.skin.setVisible(false);
			test.draw();
			require(
				test.renderer.vertices.empty(),
				"Hidden skins must not draw, including repeated visibility changes");
		}
		test.skin.setVisible(true);
		test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f});
	}

	void testAlignment()
	{
		struct AlignmentCase
		{
			MyGUI::Align align;
			MyGUI::IntCoord expected;
		};
		const AlignmentCase cases[] = {
			{MyGUI::Align::Default, {10, 20, 40, 30}},
			{MyGUI::Align::Right | MyGUI::Align::Bottom, {50, 40, 40, 30}},
			{MyGUI::Align::Center, {50, 35, 40, 30}},
			{MyGUI::Align::Stretch, {10, 20, 80, 50}},
			{MyGUI::Align::Right | MyGUI::Align::VStretch, {50, 20, 40, 50}},
			{MyGUI::Align::HStretch | MyGUI::Align::Bottom, {10, 40, 80, 30}}};
		for (const auto& item : cases)
		{
			Fixture test;
			test.draw();
			test.skin.setAlign(item.align);
			test.parent.setSize({140, 100});
			test.skin._setAlign({100, 80});
			require(test.skin.getCoord() == item.expected, "SubSkin alignment must follow parent resizing");
			test.expectQuad(
				{100 + item.expected.left, 60 + item.expected.top, item.expected.width, item.expected.height},
				{0.2f, 0.1f, 0.8f, 0.9f});
		}
	}

	void testColourAndAlpha()
	{
		for (auto format : {MyGUI::VertexColourType::ColourARGB, MyGUI::VertexColourType::ColourABGR})
		{
			Fixture test(format);
			test.draw();
			test.skin._setColour(MyGUI::Colour(1, 0.5f, 0.25f, 0));
			const MyGUI::uint32 rgb = format == MyGUI::VertexColourType::ColourARGB ? 0xFF7F3F : 0x3F7FFF;
			test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f}, 0xFF000000 | rgb);
			for (float alpha : {0.5f, 0.0f, 1.0f})
			{
				test.skin.setAlpha(alpha);
				const auto packedAlpha = static_cast<MyGUI::uint32>(alpha * 255) << 24;
				test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f}, packedAlpha | rgb);
				test.skin._setColour(MyGUI::Colour(1, 0.5f, 0.25f, 0.75f));
				test.expectQuad({110, 80, 40, 30}, {0.2f, 0.1f, 0.8f, 0.9f}, packedAlpha | rgb);
			}
		}
	}

	void testStateAndDrawItemLifetime()
	{
		Fixture test;
		MyGUI::xml::Document document;
		auto resource = document.createRoot("Resource");
		resource->addAttribute("texture", "MyGUI_BlueWhiteSkins.png");
		auto stateNode = resource->createChild("BasisSkin")->createChild("State");
		auto* texture = test.renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		stateNode->addAttribute("offset", MyGUI::IntCoord(0, 0, texture->getWidth() / 2, texture->getHeight() / 2));
		MyGUI::SubSkinStateInfo state;
		static_cast<MyGUI::IStateInfo&>(state).deserialization(stateNode, MyGUI::Version(1, 0));
		test.draw();
		test.skin.setStateData(&state);
		test.expectQuad({110, 80, 40, 30}, {0, 0, 0.5f, 0.5f});
		test.setCoord({-20, -10, 160, 100});
		test.skin._setUVSet({0, 0, 1, 1});
		test.draw();
		test.skin.setStateData(&state);
		test.expectQuad({100, 60, 100, 80}, {0.0625f, 0.05f, 0.375f, 0.45f});
		for (int repeat = 0; repeat < 2; ++repeat)
		{
			test.detach();
			test.draw();
			require(test.renderer.vertices.empty(), "Removing a draw item must remove its geometry");
			test.skin.setAlpha(0.5f);
			test.skin._setColour(MyGUI::Colour::Red);
			test.skin.setVisible(false);
			test.skin.setVisible(true);
			test.skin._correctView();
			test.setCoord({10, 20, 40, 30});
			test.skin.setStateData(&state);
			test.attach();
			test.expectQuad({110, 80, 40, 30}, {0, 0, 0.5f, 0.5f}, 0x7FFF0000);
		}
	}

}

int main()
{
	try
	{
		testGeometryAndViewCorrection();
		testClippingAndUVChanges();
		testEmptyAndHiddenViews();
		testAlignment();
		testColourAndAlpha();
		testStateAndDrawItemLifetime();
		std::cout << "SubSkin tests passed" << std::endl;
		return 0;
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		return 1;
	}
}
