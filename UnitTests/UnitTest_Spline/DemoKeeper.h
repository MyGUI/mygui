/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();

	private:
		virtual void setupResources();

		void makeBezier(const std::vector<MyGUI::FloatPoint>& _points, size_t _pointsNumber);
		void updateSpline();

		void notifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyChangeQuality(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyChangeStroke(MyGUI::ScrollBar* _sender, size_t _position);

	private:
		MyGUI::Widget* mClient;
		std::vector<MyGUI::FloatPoint> mLinePoints;
		MyGUI::PolygonalSkin* mPolygonalSkin;
		MyGUI::TextBox* mQualityText;
		MyGUI::TextBox* mStrokeText;
		MyGUI::IntPoint mOffset;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
