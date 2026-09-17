#include "SkinTest.h"
#include "MyGUI_TileRect.h"
#include "MyGUI_CommonStateInfo.h"

namespace
{

	using unittest::rectangularskin::ExpectedQuad;
	using Fixture = unittest::rectangularskin::Fixture<MyGUI::TileRect>;

	void setTiles(Fixture& _test, const MyGUI::IntSize& _size, bool _horizontal = true, bool _vertical = true)
	{
		MyGUI::xml::Document document;
		auto resource = document.createRoot("Resource");
		resource->addAttribute("texture", "MyGUI_BlueWhiteSkins.png");
		auto node = resource->createChild("BasisSkin")->createChild("State");
		auto* texture = _test.renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		node->addAttribute("offset", MyGUI::IntCoord(0, 0, texture->getWidth(), texture->getHeight()));
		auto size = node->createChild("Property");
		size->addAttribute("key", "TileSize");
		size->addAttribute("value", _size);
		auto horizontal = node->createChild("Property");
		horizontal->addAttribute("key", "TileH");
		horizontal->addAttribute("value", _horizontal);
		auto vertical = node->createChild("Property");
		vertical->addAttribute("key", "TileV");
		vertical->addAttribute("value", _vertical);
		MyGUI::TileRectStateInfo state;
		static_cast<MyGUI::IStateInfo&>(state).deserialization(node, MyGUI::Version(1, 0));
		_test.skin.setStateData(&state);
		// Widget applies alignment after selecting its initial skin state.
		_test.skin._setAlign(_test.parent.getSize());
	}

	void testFullAndPartialTiles()
	{
		Fixture test;
		setTiles(test, {20, 15});
		test.expectQuads({
			{{110, 80, 20, 15}, {0, 0, 1, 1}},
			{{130, 80, 20, 15}, {0, 0, 1, 1}},
			{{110, 95, 20, 15}, {0, 0, 1, 1}},
			{{130, 95, 20, 15}, {0, 0, 1, 1}},
		});
		test.setCoord({10, 20, 45, 35});
		test.expectQuads({
			{{110, 80, 20, 15}, {0, 0, 1, 1}},
			{{130, 80, 20, 15}, {0, 0, 1, 1}},
			{{150, 80, 5, 15}, {0, 0, 0.25f, 1}},
			{{110, 95, 20, 15}, {0, 0, 1, 1}},
			{{130, 95, 20, 15}, {0, 0, 1, 1}},
			{{150, 95, 5, 15}, {0, 0, 0.25f, 1}},
			{{110, 110, 20, 5}, {0, 0, 1, 1.0f / 3}},
			{{130, 110, 20, 5}, {0, 0, 1, 1.0f / 3}},
			{{150, 110, 5, 5}, {0, 0, 0.25f, 1.0f / 3}},
		});
	}

	void testClippingAndUVChanges()
	{
		Fixture test;
		setTiles(test, {20, 15});
		test.parent.setSize({30, 20});
		// The viewport excludes a complete row and column, and cuts every remaining edge.
		test.setCoord({-25, -18, 80, 60});
		test.expectQuads({
			{{100, 60, 15, 12}, {0.25f, 0.2f, 1, 1}},
			{{115, 60, 15, 12}, {0, 0.2f, 0.75f, 1}},
			{{100, 72, 15, 8}, {0.25f, 0, 1, 8.0f / 15}},
			{{115, 72, 15, 8}, {0, 0, 0.75f, 8.0f / 15}},
		});
		test.skin._setUVSet({0.2f, 0.1f, 0.8f, 0.9f});
		test.expectQuads({
			{{100, 60, 15, 12}, {0.35f, 0.26f, 0.8f, 0.9f}},
			{{115, 60, 15, 12}, {0.2f, 0.26f, 0.65f, 0.9f}},
			{{100, 72, 15, 8}, {0.35f, 0.1f, 0.8f, 0.1f + 0.8f * 8 / 15}},
			{{115, 72, 15, 8}, {0.2f, 0.1f, 0.65f, 0.1f + 0.8f * 8 / 15}},
		});
		test.skin._setUVSet({1, 1, 0, 0});
		test.expectQuads({
			{{100, 60, 15, 12}, {0.75f, 0.8f, 0, 0}},
			{{115, 60, 15, 12}, {1, 0.8f, 0.25f, 0}},
			{{100, 72, 15, 8}, {0.75f, 1, 0, 7.0f / 15}},
			{{115, 72, 15, 8}, {1, 1, 0.25f, 7.0f / 15}},
		});
		test.skin._setUVSet({0, 0, 1, 1});
		test.parent.setMargin({3, 2, 4, 3});
		test.skin._updateView();
		test.expectQuads({
			{{103, 62, 12, 10}, {0.4f, 1.0f / 3, 1, 1}},
			{{115, 62, 11, 10}, {0, 1.0f / 3, 0.55f, 1}},
			{{103, 72, 12, 5}, {0.4f, 0, 1, 1.0f / 3}},
			{{115, 72, 11, 5}, {0, 0, 0.55f, 1.0f / 3}},
		});
		test.parent.setMargin({0, 0, 0, 0});
		test.setCoord({5, 3, 10, 10});
		test.expectQuad({105, 63, 10, 10}, {0, 0, 0.5f, 2.0f / 3});
	}

	void testTargetAndParentOffsets()
	{
		Fixture test;
		setTiles(test, {20, 15});
		test.setCoord({-5, -3, 40, 30});
		test.draw();
		test.renderer.info.setOffset(17, 23);
		test.renderer.info.pixScaleX = 1.0f / 1024;
		test.renderer.info.pixScaleY = 1.0f / 768;
		test.renderer.info.hOffset = 0.5f / 1024;
		test.renderer.info.vOffset = 0.5f / 768;
		test.parent.setAbsolutePosition({200, 150});
		test.skin._correctView();
		const std::vector<ExpectedQuad> expected = {
			{{200, 150, 15, 12}, {0.25f, 0.2f, 1, 1}},
			{{215, 150, 20, 12}, {0, 0.2f, 1, 1}},
			{{200, 162, 15, 15}, {0.25f, 0, 1, 1}},
			{{215, 162, 20, 15}, {0, 0, 1, 1}},
		};
		test.expectQuads(expected);
		test.expectQuads(expected);
	}

	void testTilingAxes()
	{
		Fixture test;
		test.setCoord({10, 20, 35, 20});
		setTiles(test, {20, 15}, true, false);
		test.expectQuads({
			{{110, 80, 20, 20}, {0, 0, 1, 1}},
			{{130, 80, 15, 20}, {0, 0, 0.75f, 1}},
		});
		setTiles(test, {20, 15}, false, true);
		test.expectQuads({
			{{110, 80, 35, 15}, {0, 0, 1, 1}},
			{{110, 95, 35, 5}, {0, 0, 1, 1.0f / 3}},
		});
		setTiles(test, {20, 15}, false, false);
		test.expectQuad({110, 80, 35, 20}, {0, 0, 1, 1});
		test.skin.setAlign(MyGUI::Align::Stretch);
		test.parent.setSize({110, 90});
		test.skin._setAlign({100, 80});
		test.expectQuad({110, 80, 45, 30}, {0, 0, 1, 1});
	}

	void testResizeAndBufferGrowth()
	{
		Fixture test;
		setTiles(test, {20, 15});
		test.draw();
		test.parent.setSize({200, 200});
		test.setCoord({10, 20, 100, 90});
		std::vector<ExpectedQuad> expected;
		// Thirty whole tiles exceed the initial sixteen-tile reservation.
		for (int row = 0; row < 6; ++row)
			for (int column = 0; column < 5; ++column)
				expected.push_back({{110 + column * 20, 80 + row * 15, 20, 15}, {0, 0, 1, 1}});
		test.expectQuads(expected);
		test.setCoord({10, 20, 15, 10});
		test.expectQuad({110, 80, 15, 10}, {0, 0, 0.75f, 2.0f / 3});
		setTiles(test, {10, 10});
		test.expectQuads({
			{{110, 80, 10, 10}, {0, 0, 1, 1}},
			{{120, 80, 5, 10}, {0, 0, 0.5f, 1}},
		});
	}

	void testEmptyAndHiddenViews()
	{
		Fixture test;
		test.expectQuads({}); // No tile size has been selected yet.
		setTiles(test, {20, 15});
		for (const MyGUI::IntCoord coord :
			 {MyGUI::IntCoord{-50, 10, 40, 30},
			  {100, 10, 40, 30},
			  {10, -30, 40, 30},
			  {10, 80, 40, 30},
			  {10, 20, 0, 30},
			  {10, 20, 40, 0}})
		{
			test.setCoord(coord);
			test.expectQuads({});
			test.setCoord({10, 20, 10, 10});
			test.expectQuad({110, 80, 10, 10}, {0, 0, 0.5f, 2.0f / 3});
		}
		test.skin.setVisible(false);
		test.expectQuads({});
		test.skin.setVisible(true);
		test.expectQuad({110, 80, 10, 10}, {0, 0, 0.5f, 2.0f / 3});
		setTiles(test, {0, 15});
		test.expectQuads({});
		setTiles(test, {20, 0});
		test.expectQuads({});
		setTiles(test, {-20, 15});
		test.expectQuads({});
		setTiles(test, {20, -15});
		test.expectQuads({});
		setTiles(test, {20, 15});
		test.expectQuad({110, 80, 10, 10}, {0, 0, 0.5f, 2.0f / 3});
	}

}

void unittest::rectangularskin::addTileRectTests(Tests& _tests)
{
	_tests.insert(
		_tests.end(),
		{{"TileRect.FullAndPartialTiles", testFullAndPartialTiles},
		 {"TileRect.ClippingAndUVChanges", testClippingAndUVChanges},
		 {"TileRect.TargetAndParentOffsets", testTargetAndParentOffsets},
		 {"TileRect.TilingAxes", testTilingAxes},
		 {"TileRect.ResizeAndBufferGrowth", testResizeAndBufferGrowth},
		 {"TileRect.EmptyAndHiddenViews", testEmptyAndHiddenViews}});
}
