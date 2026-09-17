#include "SkinTest.h"

namespace unittest::customskin
{
	namespace
	{

		using Test = Fixture<MyGUI::RotatingSkin>;
		constexpr float pi = 3.14159265358979323846f;

		void checkUV(const Test& _test, float _angle, MyGUI::FloatPoint _center, const MyGUI::FloatRect& _uv)
		{
			require(!_test.surface().empty(), "Expected a visible surface for UV checks");
			for (const auto& v : _test.surface())
			{
				auto p = _test.local(v) - _center;
				// Recover the unrotated pixel position from the submitted surface.
				const float x = _center.left + p.left * std::cos(_angle) + p.top * std::sin(_angle);
				const float y = _center.top - p.left * std::sin(_angle) + p.top * std::cos(_angle);
				near(
					v.u,
					_uv.left + x / _test.skin.getWidth() * (_uv.right - _uv.left),
					"Rotated U mapping is incorrect");
				near(
					v.v,
					_uv.top + y / _test.skin.getHeight() * (_uv.bottom - _uv.top),
					"Rotated V mapping is incorrect");
			}
		}

		void testAnglesAndCenter()
		{
			Test test;
			test.setCoord({0, 0, 40, 20});
			for (const auto& center : {MyGUI::IntPoint(0, 0), MyGUI::IntPoint(20, 10), MyGUI::IntPoint(-10, 30)})
			{
				test.skin.setCenter(center);
				require(test.skin.getCenter() == center, "Rotation center getter differs from setter");
				require(
					test.skin.getCenter(false) == center + test.parent.getAbsolutePosition(),
					"Absolute center is incorrect");
				for (float angle : {0.0f, pi / 2, pi, -pi / 2, 0.37f, 2 * pi})
				{
					test.skin.setAngle(angle);
					near(test.skin.getAngle(), angle, "Angle getter differs from setter");
					test.draw();
					near(test.area(), 800, "Rotation must preserve rectangle area", 0.1f);
					checkUV(test, angle, MyGUI::FloatPoint(center.left, center.top), {0.2f, 0.1f, 0.8f, 0.9f});
				}
			}
			test.skin.setCenter({0, 0});
			test.skin.setAngle(pi / 2);
			test.draw();
			test.expectBounds({-20, 0, 0, 40});
		}

		void testUVChanges()
		{
			Test test;
			test.skin.setCenter({50, 50});
			test.skin.setAngle(0.37f);
			test.draw();
			test.skin._setUVSet({1, 0.8f, 0, 0.2f});
			test.draw();
			checkUV(test, 0.37f, {50, 50}, {1, 0.8f, 0, 0.2f});
		}

		void testClipping()
		{
			Test test;
			test.clip.setCoord({0, 0, 60, 80});
			test.parent._setCroppedParent(&test.clip);
			test.skin.setCenter({50, 50});
			test.draw();
			require(test.renderer.vertices.size() == 6, "A clipped rectangle must emit only two triangles");
			require(test.renderer.reservedVertices == 18, "The rotating skin must reserve its maximum capacity");
			test.expectBounds({0, 0, 60, 80});
			near(test.area(), 4800, "Clipped rectangle area is incorrect", 0.1f);
			checkUV(test, 0, {50, 50}, {0.2f, 0.1f, 0.8f, 0.9f});
			// A square rotated by 45 degrees intersects all four sides in an octagon.
			test.clip.setSize({100, 100});
			test.skin.setAngle(pi / 4);
			test.draw();
			require(test.renderer.vertices.size() == 18, "A clipped octagon must emit six triangles");
			require(test.renderer.reservedVertices == 18, "Clipping must retain the same buffer capacity");
			test.expectBounds({0, 0, 100, 100});
			const float corner = 100 - 50 * std::sqrt(2.0f);
			near(test.area(), 10000 - 2 * corner * corner, "Octagonal clipping area is incorrect", 0.1f);
			checkUV(test, pi / 4, {50, 50}, {0.2f, 0.1f, 0.8f, 0.9f});
			// Parent margins must change both positions and interpolated UVs.
			test.clip.setMargin({10, 20, 30, 40});
			test.skin.setAngle(0);
			test.skin._updateView();
			test.draw();
			test.expectBounds({10, 20, 70, 60});
			near(test.area(), 2400, "Ancestor margins were ignored", 0.1f);
			checkUV(test, 0, {50, 50}, {0.2f, 0.1f, 0.8f, 0.9f});
		}

		void testEmptyClipAndRecovery()
		{
			Test test;
			test.clip.setCoord({0, 0, 100, 100});
			test.parent._setCroppedParent(&test.clip);
			test.draw();
			require(test.area() > 0, "Initial rectangle must be visible");
			test.parent.setPosition({200, 200});
			test.skin._updateView();
			test.draw();
			require(test.renderer.vertices.empty(), "Fully clipped rectangle must emit no vertices");
			test.parent.setPosition({0, 0});
			test.skin._updateView();
			test.draw();
			near(test.area(), 10000, "Geometry must recover after an empty clip", 0.1f);
			require(test.renderer.vertices.size() == 6, "Recovery must emit only two triangles");
			require(test.renderer.reservedVertices == 18, "Recovery must retain the same buffer capacity");
			test.setCoord({0, 0, 0, 100});
			test.draw();
			require(test.renderer.vertices.empty(), "Zero-width skin must not draw");
		}

		void testOffset()
		{
			Test test;
			test.setCoord({20, 30, 40, 20});
			test.draw();
			test.expectBounds({20, 30, 60, 50});
		}

		void testState()
		{
			Test test;
			test.draw();
			applyHalfTextureState(test.skin, test.renderer);
			test.draw();
			near(test.skin.getAngle(), 0.37f, "State must update rotation angle");
			require(test.skin.getCenter() == MyGUI::IntPoint(20, 10), "State must update rotation center");
			near(test.area(), 10000, "State must preserve rectangle area", 0.1f);
			checkUV(test, 0.37f, {20, 10}, {0, 0, 0.5f, 0.5f});
		}

		void testAlignment()
		{
			Test test;
			test.skin.setAlign(MyGUI::Align::Stretch);
			test.parent.setSize({140, 120});
			test.skin._setAlign({100, 100});
			test.draw();
			test.expectBounds({0, 0, 140, 120});
		}

	}

	void addRotatingSkinTests(Tests& _tests)
	{
		_tests.insert(
			_tests.end(),
			{{"RotatingSkin.AnglesAndCenter", testAnglesAndCenter},
			 {"RotatingSkin.UVChanges", testUVChanges},
			 {"RotatingSkin.Clipping", testClipping},
			 {"RotatingSkin.EmptyClipAndRecovery", testEmptyClipAndRecovery},
			 {"RotatingSkin.Offset", testOffset},
			 {"RotatingSkin.Alignment", testAlignment},
			 {"RotatingSkin.State", testState},
			 {"RotatingSkin.AppearanceAndLifetime", testAppearanceAndLifetime<MyGUI::RotatingSkin>},
			 {"RotatingSkin.ViewCorrection", testViewCorrection<MyGUI::RotatingSkin>},
			 {"RotatingSkin.TargetChange", testTargetChange<MyGUI::RotatingSkin>},
			 {"RotatingSkin.TargetOrigin", testTargetOrigin<MyGUI::RotatingSkin>}});
	}

}
