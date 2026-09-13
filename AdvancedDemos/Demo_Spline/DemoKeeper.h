/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;

	private:
		void setupResources() override;

		void makeBezier(const std::vector<MyGUI::FloatPoint>& _points, size_t _pointsNumber);
		void updateSpline();

		void notifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyChangeQuality(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyChangeStroke(MyGUI::ScrollBar* _sender, size_t _position);

	private:
		MyGUI::Widget* mClient{nullptr};
		std::vector<MyGUI::FloatPoint> mLinePoints;
		MyGUI::PolygonalSkin* mPolygonalSkin{nullptr};
		MyGUI::TextBox* mQualityText{nullptr};
		MyGUI::TextBox* mStrokeText{nullptr};
		MyGUI::IntPoint mOffset;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
