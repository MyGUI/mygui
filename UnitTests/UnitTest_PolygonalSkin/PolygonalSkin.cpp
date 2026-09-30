#include "SkinTest.h"

namespace unittest::customskin
{
	namespace
	{

		using Test = Fixture<MyGUI::PolygonalSkin>;

		void checkLineUV(
			const Test& _test,
			MyGUI::FloatPoint _start,
			MyGUI::FloatPoint _end,
			float _width,
			const MyGUI::FloatRect& _uv)
		{
			auto d = _end - _start;
			const float length = std::hypot(d.left, d.top);
			require(!_test.surface().empty(), "Expected a visible surface for UV checks");
			for (const auto& v : _test.surface())
			{
				auto p = _test.local(v) - _start;
				const float along = (p.left * d.left + p.top * d.top) / (length * length);
				const float across = (p.left * -d.top + p.top * d.left) / length;
				near(v.u, _uv.left + along * (_uv.right - _uv.left), "Line U must follow distance along the path");
				near(
					v.v,
					(_uv.top + _uv.bottom) / 2 - across / _width * (_uv.bottom - _uv.top),
					"Line V must follow stroke width");
			}
		}

		void testLinesAndWidth()
		{
			Test test;
			for (const auto& end : {MyGUI::FloatPoint(90, 10), MyGUI::FloatPoint(10, 90), MyGUI::FloatPoint(90, 90)})
			{
				test.skin.setPoints({{10, 10}, end});
				for (float width : {2.0f, 10.0f})
				{
					test.skin.setWidth(width);
					test.draw();
					near(
						test.area(),
						width * std::hypot(end.left - 10, end.top - 10),
						"Line area does not match length times width",
						0.1f);
					checkLineUV(test, {10, 10}, end, width, {0.2f, 0.1f, 0.8f, 0.9f});
				}
			}
		}

		void testClippingAndUV()
		{
			Test test;
			test.skin.setPoints({{-20, 50}, {120, 50}});
			test.draw();
			test.expectBounds({0, 45, 100, 55});
			near(test.area(), 1000, "Clipped line area is incorrect", 0.1f);
			checkLineUV(test, {-20, 50}, {120, 50}, 10, {0.2f, 0.1f, 0.8f, 0.9f});
			test.skin._setUVSet({1, 1, 0, 0});
			test.draw();
			checkLineUV(test, {-20, 50}, {120, 50}, 10, {1, 1, 0, 0});
			test.parent.setMargin({10, 0, 20, 0});
			test.skin._updateView();
			test.draw();
			test.expectBounds({10, 45, 80, 55});
			near(test.area(), 700, "Parent margins must clip line geometry", 0.1f);
			checkLineUV(test, {-20, 50}, {120, 50}, 10, {1, 1, 0, 0});
		}

		void testJoinsAndDuplicates()
		{
			Test test;
			test.skin.setPoints({{10, 20}, {10, 20}, {50, 20}, {50, 20}, {50, 60}});
			test.draw();
			near(test.area(), 800, "Right-angle join must preserve stroke area", 0.1f);
			test.expectBounds({10, 15, 55, 60});
			test.skin.setPoints({{10, 50}, {50, 50}, {90, 50}});
			test.draw();
			near(test.area(), 800, "Collinear joins must remain a continuous line", 0.1f);
			checkLineUV(test, {10, 50}, {90, 50}, 10, {0.2f, 0.1f, 0.8f, 0.9f});
		}

		void testStroke()
		{
			Test test;
			test.skin.setPoints({{10, 50}, {30, 50}, {50, 50}, {70, 50}, {90, 50}});
			test.skin.setStroke(1);
			test.draw();
			near(test.area(), 400, "Stroke step one must draw alternating segments", 0.1f);
			checkLineUV(test, {10, 50}, {90, 50}, 10, {0.2f, 0.1f, 0.8f, 0.9f});
			test.skin.setStroke(0);
			test.draw();
			near(test.area(), 800, "Disabling stroke must restore the full path", 0.1f);
		}

		void testEmptyAndRecovery()
		{
			Test test;
			test.skin.setPoints({});
			test.draw();
			require(test.renderer.vertices.empty(), "Initially empty path must not draw");
			test.skin.setPoints({{10, 50}, {90, 50}});
			test.draw();
			near(test.area(), 800, "Empty path must recover", 0.1f);
			test.skin.setPoints({{-20, -20}, {-10, -10}});
			test.draw();
			require(test.surface().empty(), "Outside path must be clipped");
			test.skin.setPoints({{10, 50}, {90, 50}});
			test.draw();
			near(test.area(), 800, "Clipped path must recover", 0.1f);
			test.setCoord({0, 0, 0, 100});
			test.draw();
			require(test.renderer.vertices.empty(), "Zero-width view must not draw");
		}

		void testState()
		{
			Test test;
			test.draw();
			applyHalfTextureState(test.skin, test.renderer);
			test.draw();
			near(test.area(), 800, "Texture state must preserve line geometry", 0.1f);
			checkLineUV(test, {10, 50}, {90, 50}, 10, {0, 0, 0.5f, 0.5f});
		}

		void testAlignment()
		{
			Test test;
			test.skin.setPoints({{-20, 50}, {160, 50}});
			test.draw();
			test.expectBounds({0, 45, 100, 55});
			test.skin.setAlign(MyGUI::Align::Stretch);
			test.parent.setSize({140, 120});
			test.skin._setAlign({100, 100});
			test.draw();
			test.expectBounds({0, 45, 140, 55});
			near(test.area(), 1400, "Alignment must update the line's clipping rectangle", 0.1f);
		}

		void testClear()
		{
			Test test;
			test.draw();
			require(test.area() > 0, "Initial path must draw");
			test.skin.setPoints({});
			test.draw();
			require(test.renderer.vertices.empty(), "Clearing points must invalidate the cached draw");
		}

		void testSinglePoint()
		{
			Test test;
			test.draw();
			test.skin.setPoints({{20, 20}});
			test.draw();
			require(test.renderer.vertices.empty(), "A single point must clear the cached draw");
		}

		void testDuplicateOnly()
		{
			Test test;
			test.draw();
			test.skin.setPoints({{20, 20}, {20, 20}});
			test.draw();
			require(test.renderer.vertices.empty(), "Duplicate-only points must clear previous geometry");
		}

		void testCapacity()
		{
			Test test;
			test.skin.setPoints({{-81.9606f, -49.323f}, {66.0494f, 59.6754f}, {-70.311f, -41.5809f}});
			test.skin.setWidth(122.247f);
			test.draw();
			require(test.area() > 0, "Clipped sharp join must remain visible");
		}

		void testReversal()
		{
			Test test;
			// Fixed ordinary coordinates exercise rounding at an exact 180-degree reversal.
			for (int x = 1; x <= 20; ++x)
				for (int y = 1; y <= 20; ++y)
				{
					test.skin.setPoints({{30, 30}, {30.0f + x, 30.0f + y}, {30, 30}});
					test.draw();
					require(test.area() > 0, "A reversing path must retain finite visible geometry");
				}
		}

	}

	void addPolygonalSkinTests(Tests& _tests)
	{
		_tests.insert(
			_tests.end(),
			{{"PolygonalSkin.LinesAndWidth", testLinesAndWidth},
			 {"PolygonalSkin.ClippingAndUV", testClippingAndUV},
			 {"PolygonalSkin.JoinsAndDuplicates", testJoinsAndDuplicates},
			 {"PolygonalSkin.Stroke", testStroke},
			 {"PolygonalSkin.EmptyAndRecovery", testEmptyAndRecovery},
			 {"PolygonalSkin.State", testState},
			 {"PolygonalSkin.Alignment", testAlignment},
			 {"PolygonalSkin.Clear", testClear},
			 {"PolygonalSkin.SinglePoint", testSinglePoint},
			 {"PolygonalSkin.DuplicateOnly", testDuplicateOnly},
			 {"PolygonalSkin.Capacity", testCapacity},
			 {"PolygonalSkin.Reversal", testReversal},
			 {"PolygonalSkin.AppearanceAndLifetime", testAppearanceAndLifetime<MyGUI::PolygonalSkin>},
			 {"PolygonalSkin.ViewCorrection", testViewCorrection<MyGUI::PolygonalSkin>},
			 {"PolygonalSkin.TargetChange", testTargetChange<MyGUI::PolygonalSkin>},
			 {"PolygonalSkin.TargetOrigin", testTargetOrigin<MyGUI::PolygonalSkin>}});
	}

}
