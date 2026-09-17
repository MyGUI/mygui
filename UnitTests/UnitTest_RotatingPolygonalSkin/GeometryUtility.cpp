#include "SkinTest.h"
#include "MyGUI_GeometryUtility.h"

namespace unittest::customskin
{
	namespace
	{

		float polygonArea(const MyGUI::VectorFloatPoint& _points)
		{
			float area = 0;
			for (size_t i = 0; i < _points.size(); ++i)
			{
				const auto& a = _points[i];
				const auto& b = _points[(i + 1) % _points.size()];
				area += a.left * b.top - a.top * b.left;
			}
			return area / 2;
		}

		void testClipping()
		{
			const MyGUI::IntCoord rect(0, 0, 100, 100);
			struct Case
			{
				MyGUI::VectorFloatPoint points;
				float area;
			};
			const Case cases[] = {
				{{}, 0},
				{{{10, 10}, {90, 10}, {90, 90}, {10, 90}}, 6400},
				{{{-10, -10}, {110, -10}, {110, 110}, {-10, 110}}, 10000},
				{{{-20, 20}, {20, 20}, {20, 80}, {-20, 80}}, 1200},
				{{{80, 20}, {120, 20}, {120, 80}, {80, 80}}, 1200},
				{{{20, -20}, {80, -20}, {80, 20}, {20, 20}}, 1200},
				{{{20, 80}, {80, 80}, {80, 120}, {20, 120}}, 1200},
				{{{-20, -20}, {-10, -20}, {-10, -10}}, 0},
				{{{0, 0}, {-10, 10}, {10, 10}}, -50},
				{{{-10, 0}, {0, 0}, {0, 100}, {-10, 100}}, 0},
				{{{50, -50}, {150, 50}, {50, 150}, {-50, 50}}, 10000}};
			for (const auto& item : cases)
			{
				for (bool reverse : {false, true})
				{
					auto points = item.points;
					if (reverse)
						std::reverse(points.begin(), points.end());
					auto clipped = MyGUI::geometry_utility::cropPolygon(points.data(), points.size(), rect);
					near(
						polygonArea(clipped),
						reverse ? -item.area : item.area,
						"Clipping changed area or winding",
						0.05f);
					for (const auto& p : clipped)
						require(
							std::isfinite(p.left) && std::isfinite(p.top) && p.left >= 0 && p.left <= 100 &&
								p.top >= 0 && p.top <= 100,
							"Clipped vertex must be finite and inside the crop rectangle");
					auto twice = MyGUI::geometry_utility::cropPolygon(clipped.data(), clipped.size(), rect);
					near(polygonArea(twice), polygonArea(clipped), "Repeated clipping changed the surface", 0.05f);
				}
			}
		}

		void testAffineUV()
		{
			const MyGUI::FloatPoint origin(3, 5), x(11, 9), y(5, 11);
			for (const auto& weights :
				 {MyGUI::FloatPoint(0, 0),
				  MyGUI::FloatPoint(1, 0),
				  MyGUI::FloatPoint(0, 1),
				  MyGUI::FloatPoint(0.25f, 0.75f),
				  MyGUI::FloatPoint(-0.5f, 1.5f)})
			{
				MyGUI::FloatPoint p(
					origin.left + weights.left * 8 + weights.top * 2,
					origin.top + weights.left * 4 + weights.top * 6);
				auto position = MyGUI::geometry_utility::getPositionInsideRect(p, origin, x, y);
				// The callers pair corner2's UV vector with the first returned coefficient.
				near(position.left, weights.top, "Second basis coefficient is incorrect");
				near(position.top, weights.left, "First basis coefficient is incorrect");
				auto uv =
					MyGUI::geometry_utility::getUVFromPositionInsideRect(position, {0, 0.8f}, {0.6f, 0}, {0.2f, 0.1f});
				near(uv.left, 0.2f + weights.left * 0.6f, "Affine U interpolation is incorrect");
				near(uv.top, 0.1f + weights.top * 0.8f, "Affine V interpolation is incorrect");
			}
			auto degenerate = MyGUI::geometry_utility::getPositionInsideRect({1, 1}, {0, 0}, {1, 1}, {2, 2});
			require(
				std::isfinite(degenerate.left) && std::isfinite(degenerate.top),
				"Degenerate basis must not divide by zero");
		}

	}

	void addGeometryUtilityTests(Tests& _tests)
	{
		_tests.insert(
			_tests.end(),
			{{"GeometryUtility.Clipping", testClipping}, {"GeometryUtility.AffineUV", testAffineUV}});
	}

}
