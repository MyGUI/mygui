#include "TextRendering.h"
#include "BehaviourTestSupport.h"
#include "SkinTestContext.h"
#include "MyGUI_EditText.h"
#include "MyGUI_TextureUtility.h"

namespace
{

	using unittest::require;

	class TextRenderer : public unittest::SkinRenderManager
	{
	public:
		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			SkinRenderManager::doRender(_buffer, _texture, _count);
			const auto& data = static_cast<unittest::SkinVertexBuffer*>(_buffer)->vertices;
			vertices.insert(vertices.end(), data.begin(), data.begin() + _count);
		}
		std::vector<MyGUI::Vertex> vertices;
	};

	struct TextFixture
	{
		TextFixture()
		{
			renderer.initialise();
			data.initialise();
			renderer.setViewSize(800, 600);
			gui.initialise("");
			unittest::loadResources("UnitTest_Rendering/TextSkin.xml");
			unittest::createInputLayer();
			widget = gui.createWidget<MyGUI::TextBox>(
				"RenderTestText",
				MyGUI::IntCoord(100, 100, 100, 60),
				MyGUI::Align::Default,
				"Main");
			widget->setFontName("RenderTestFont");
			widget->setFontHeight(20);
			widget->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Top);
			widget->setTextColour(MyGUI::Colour::Red);
			text = widget->getSubWidgetText()->castType<MyGUI::EditText>();
		}
		~TextFixture()
		{
			gui.shutdown();
			data.shutdown();
			renderer.shutdown();
		}
		void render(size_t _quads)
		{
			renderer.vertices.clear();
			renderer.render();
			require(renderer.vertices.size() == _quads * 6, "Text must submit exactly the expected visible quads");
		}
		void quad(size_t _index, MyGUI::FloatRect _pixels, MyGUI::FloatRect _atlasPixels, MyGUI::uint32 _colour)
		{
			const auto* atlas = renderer.getTexture("MyGUI_BlueWhiteSkins.png");
			const int corners[6]{0, 1, 2, 1, 3, 2};
			for (size_t i = 0; i < 6; ++i)
			{
				const auto& v = renderer.vertices.at(_index * 6 + i);
				const bool right = corners[i] & 1, bottom = corners[i] & 2;
				const float x = (v.x + 1) * 400, y = (1 - v.y) * 300;
				require(
					std::abs(x - (right ? _pixels.right : _pixels.left)) < 0.001f &&
						std::abs(y - (bottom ? _pixels.bottom : _pixels.top)) < 0.001f,
					"Glyph triangles must use the expected pixel corners");
				require(
					std::abs(v.u * atlas->getWidth() - (right ? _atlasPixels.right : _atlasPixels.left)) < 0.001f &&
						std::abs(v.v * atlas->getHeight() - (bottom ? _atlasPixels.bottom : _atlasPixels.top)) < 0.001f,
					"Glyph UVs must track the visible part of the atlas rectangle");
				require(v.colour == _colour, "Glyph vertices must carry the expected colour");
			}
		}
		MyGUI::uint32 colour(const MyGUI::Colour& _colour)
		{
			return MyGUI::texture_utility::toNativeColour(_colour, renderer.getVertexFormat());
		}
		MyGUI::LogManager log;
		TextRenderer renderer;
		MyGUI::DummyDataManager data;
		MyGUI::Gui gui;
		MyGUI::TextBox* widget;
		MyGUI::EditText* text;
	};

}

void testTextGeometry()
{
	TextFixture f;
	f.widget->setCaption("A#00FF00B");
	f.render(2);
	f.quad(0, {100, 100, 110, 120}, {16, 32, 26, 52}, f.colour(MyGUI::Colour::Red));
	f.quad(1, {110, 100, 120, 120}, {32, 32, 42, 52}, f.colour(MyGUI::Colour::Green));
	f.widget->setCaption("A");
	f.text->setShadow(true);
	f.text->setShadowColour(MyGUI::Colour::Blue);
	f.render(2);
	f.quad(0, {101, 101, 111, 121}, {16, 32, 26, 52}, f.colour(MyGUI::Colour::Blue));
	f.quad(1, {100, 100, 110, 120}, {16, 32, 26, 52}, f.colour(MyGUI::Colour::Red));
	f.text->setShadow(false);
	f.widget->setCaption("?");
	f.render(1);
	f.quad(0, {100, 100, 110, 120}, {48, 32, 58, 52}, f.colour(MyGUI::Colour::Red));
	f.widget->setVisible(false);
	f.render(0);
	f.widget->setVisible(true);
	f.widget->setCaption("");
	f.render(0);
}

void testTextClipping()
{
	TextFixture f;
	f.widget->setCaption("A");
	const auto red = f.colour(MyGUI::Colour::Red);
	f.text->setViewOffset({5, 0});
	f.render(1);
	f.quad(0, {100, 100, 105, 120}, {21, 32, 26, 52}, red);
	f.text->setViewOffset({0, 5});
	f.render(1);
	f.quad(0, {100, 100, 110, 115}, {16, 37, 26, 52}, red);
	f.text->setViewOffset({0, 0});
	f.widget->setSize(5, 60);
	f.render(1);
	f.quad(0, {100, 100, 105, 120}, {16, 32, 21, 52}, red);
	f.widget->setSize(100, 10);
	f.render(1);
	f.quad(0, {100, 100, 110, 110}, {16, 32, 26, 42}, red);
	f.widget->setSize(5, 10);
	f.text->setViewOffset({2, 3});
	f.render(1);
	f.quad(0, {100, 100, 105, 110}, {18, 35, 23, 45}, red);
	for (const auto offset :
		 {MyGUI::IntPoint(20, 0), MyGUI::IntPoint(-20, 0), MyGUI::IntPoint(0, 30), MyGUI::IntPoint(0, -30)})
	{
		f.text->setViewOffset(offset);
		f.render(0);
	}
}

void testTextSelectionAndCursor()
{
	TextFixture f;
	f.widget->setCaption("AB\nAB");
	f.text->setTextSelection(1, 4);
	f.text->setInvertSelected(false);
	f.render(6);
	f.quad(0, {110, 100, 120, 120}, {80, 32, 81, 33}, f.colour(MyGUI::Colour::White));
	f.quad(3, {100, 120, 110, 140}, {80, 32, 81, 33}, f.colour(MyGUI::Colour::White));
	f.quad(2, {110, 100, 120, 120}, {32, 32, 42, 52}, f.colour(MyGUI::Colour::Red));
	f.text->setInvertSelected(true);
	f.render(6);
	const MyGUI::Colour cyan(0, 1, 1);
	f.quad(0, {110, 100, 120, 120}, {80, 32, 81, 33}, f.colour(cyan));
	f.quad(2, {110, 100, 120, 120}, {32, 32, 42, 52}, f.colour(cyan));
	f.quad(1, {100, 100, 110, 120}, {16, 32, 26, 52}, f.colour(MyGUI::Colour::Red));
	f.text->setTextSelection(0, 0);
	f.text->setCursorPosition(4);
	f.text->setVisibleCursor(true);
	f.render(5);
	f.quad(4, {110, 120, 112, 140}, {64, 32, 66, 52}, f.colour(MyGUI::Colour::White));
	f.text->setVisibleCursor(false);
	f.render(4);
}
