#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_GeometryUtility.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>

namespace
{

	using unittest::require;

	void near(float _actual, float _expected, const char* _message, float _tolerance = 0.002f)
	{
		require(
			std::abs(_actual - _expected) < _tolerance,
			std::string(_message) + ": expected " + std::to_string(_expected) + ", got " + std::to_string(_actual));
	}

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
				near(polygonArea(clipped), reverse ? -item.area : item.area, "Clipping changed area or winding", 0.05f);
				for (const auto& p : clipped)
					require(
						std::isfinite(p.left) && std::isfinite(p.top) && p.left >= 0 && p.left <= 100 && p.top >= 0 &&
							p.top <= 100,
						"Clipped vertex must be finite and inside the crop rectangle");
				auto twice = MyGUI::geometry_utility::cropPolygon(clipped.data(), clipped.size(), rect);
				near(polygonArea(twice), polygonArea(clipped), "Repeated clipping changed the surface", 0.05f);
			}
		}
	}

	void testRenderTargetConversion()
	{
		using namespace MyGUI::geometry_utility;
		MyGUI::RenderTargetInfo info;
		info.pixScaleX = 1.0f / 128;
		info.pixScaleY = 1.0f / 64;
		info.hOffset = 0.125f;
		info.vOffset = -0.25f;
		info.setOffset(8, 4);
		const MyGUI::IntPoint origin(40, 20);
		const MyGUI::FloatPoint local(16, 8);
		const MyGUI::FloatPoint absolute(56, 28);
		const MyGUI::FloatPoint expected(0, 0.75f);

		require(toRenderTarget(local, info, origin) == expected, "Local point must include origin and target offsets");
		require(toRenderTarget(absolute, info) == expected, "Default origin must accept absolute pixels");
		require(local == MyGUI::FloatPoint(16, 8), "Value conversion must preserve its input");
		require(fromRenderTarget(expected, info, origin) == local, "Inverse must return local pixels");
		require(fromRenderTarget(expected, info) == absolute, "Default inverse must return absolute pixels");

		const MyGUI::FloatRect rect(16, 8, 48, 24);
		const auto converted = toRenderTarget(rect, info, origin);
		require(
			converted == MyGUI::FloatRect(0, 0.75f, 0.5f, 0.25f),
			"Rectangle conversion must preserve edge identities across the Y flip");

		require(
			toRenderTarget(MyGUI::IntCoord(16, 8, 32, 16), info, origin) == converted,
			"Integer rectangle conversion must use width and height with parent and target offsets");
		require(
			toRenderTarget(MyGUI::IntCoord(-16, -8, 0, 0), info, origin) ==
				MyGUI::FloatRect(-0.5f, 1.25f, -0.5f, 1.25f),
			"Empty integer rectangles must preserve negative positions");

		MyGUI::FloatPoint points[] = {local, {48, 24}};
		toRenderTargetInPlace(points, info, origin);
		require(
			points[0] == expected && points[1] == MyGUI::FloatPoint(0.5f, 0.25f),
			"Array conversion must convert every corner");
		std::array<MyGUI::FloatPoint, 2> absolutePoints = {{absolute, {88, 44}}};
		toRenderTargetInPlace(absolutePoints, info);
		require(
			absolutePoints[0] == points[0] && absolutePoints[1] == points[1],
			"std::array conversion must support the default origin");

		MyGUI::VectorFloatPoint buffer = {local, {48, 24}};
		toRenderTargetInPlace(buffer.data(), 1, info, origin);
		require(
			buffer[0] == expected && buffer[1] == MyGUI::FloatPoint(48, 24),
			"Buffer conversion must only change the requested prefix");
		toRenderTargetInPlace(nullptr, 0, info);

		info.hOffset = info.vOffset = 0;
		info.setOffset(0, 0);
		require(
			toRenderTarget(MyGUI::FloatRect(0, 0, 128, 64), info) == MyGUI::FloatRect(-1, 1, 1, -1),
			"Target edges must map to normalized render coordinates");
		require(
			toRenderTarget(MyGUI::IntCoord(0, 0, 128, 64), info) == MyGUI::FloatRect(-1, 1, 1, -1),
			"Integer target edges must map to normalized render coordinates");
		info.pixScaleX = 1.0f / 1920;
		info.pixScaleY = 1.0f / 1080;
		info.setOffset(13, -7);
		for (const MyGUI::IntCoord coord :
			 {MyGUI::IntCoord(11, 19, 76, 38), MyGUI::IntCoord(-25, -18, 80, 60), MyGUI::IntCoord(901, 507, 0, 0)})
		{
			const MyGUI::FloatRect edges(
				(float)coord.left,
				(float)coord.top,
				(float)coord.right(),
				(float)coord.bottom());
			require(
				toRenderTarget(coord, info, origin) == toRenderTarget(edges, info, origin),
				"Integer rectangles must preserve floating-point rounding to avoid changing rotated clipping");
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

int main()
{
	return unittest::runTests({
		{"GeometryUtility.Clipping", testClipping},
		{"GeometryUtility.AffineUV", testAffineUV},
		{"GeometryUtility.RenderTargetConversion", testRenderTargetConversion},
	});
}
