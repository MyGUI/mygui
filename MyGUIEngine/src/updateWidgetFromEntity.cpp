/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "updateWidgetFromEntity.h"
#include "MyGUI_Widget.h"

namespace helper
{

	void updateWidgetPosition(Ogre::Entity * _entity, Ogre::Camera * _camera, MyGUI::WidgetPtr _widget)
	{

		// get the projection of the object's AABB into screen space
		const Ogre::AxisAlignedBox& bbox = _entity->getWorldBoundingBox(true);
		Ogre::Matrix4 mat = _camera->getViewMatrix();

		const Ogre::Vector3* corners = bbox.getAllCorners();

		float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;

		// expand the screen-space bounding-box so that it completely encloses
		// the object's AABB
		for (int i=0; i<8; i++) {
			Ogre::Vector3 corner = corners[i];

			// multiply the AABB corner vertex by the view matrix to
			// get a camera-space vertex
			corner = mat * corner;

			// make 2D relative/normalized coords from the view-space vertex
			// by dividing out the Z (depth) factor -- this is an approximation
			float x = corner.x / corner.z + 0.5;
			float y = corner.y / corner.z + 0.5;

			if (x < min_x) min_x = x;
			if (x > max_x) max_x = x;
			if (y < min_y) min_y = y;
			if (y > max_y) max_y = y;
		}

		Ogre::Viewport * view = _camera->getViewport();

		// корректируем координаты для нормально восприятия
		// точка отсчета левый верхний угол
		float tmp = min_x;
		min_x = (1 - max_x);
		max_x = (1 - tmp);

		// середина ентити - половина длинны виджета
		int x = (int)((min_x + ((max_x - min_x) * 0.5)) * view->getActualWidth());
		x -= (_widget->getWidth() / 2);

		// верх энтити - высота виджета
		int y = (int)(min_y * view->getActualHeight());
		y -= _widget->getHeight();

		_widget->setPosition(x, y);

	}

} // namespace helper
