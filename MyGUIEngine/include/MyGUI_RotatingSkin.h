/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_ROTATING_SKIN_H__
#define __MYGUI_ROTATING_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class MYGUI_EXPORT RotatingSkin :
		public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED( RotatingSkin )

	public:
		RotatingSkin();
		virtual ~RotatingSkin();

		/** Set angle of rotation in radians */
		void setAngle(float _angle);
		/** Get angle of rotation in radians */
		float getAngle() const;

		/** Set center of rotation
			@param _center Center point.
		*/
		void setCenter(const IntPoint& _center);
		/** Get center of rotation */
		IntPoint getCenter(bool _local = true) const;

		virtual void setAlpha(float _alpha);

		virtual void setVisible(bool _visible);

		virtual void setStateData(IStateInfo* _data);

		virtual void createDrawItem(ITexture* _texture, ILayerNode* _node);
		virtual void destroyDrawItem();

		// метод для отрисовки себя
		virtual void doRender();

		/*internal:*/
		virtual void _updateView();
		virtual void _correctView();

		virtual void _setAlign(const IntSize& _oldsize);

		virtual void _setUVSet(const FloatRect& _rect);
		virtual void _setColour(const Colour& _value);

	protected:
		void _rebuildGeometry();

	private:
		bool mGeometryOutdated;

		float mAngle;
		IntPoint mCenterPos;

		enum {RECT_VERTICIES_COUNT = 4, GEOMETRY_VERTICIES_TOTAL_COUNT = 8};
		FloatPoint mResultVerticiesPos[GEOMETRY_VERTICIES_TOTAL_COUNT];
		FloatPoint mResultVerticiesUV[GEOMETRY_VERTICIES_TOTAL_COUNT];

		bool mEmptyView;

		VertexColourType mVertexFormat;
		uint32 mCurrentColour;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode;
		RenderItem* mRenderItem;
	};

} // namespace MyGUI

#endif // __MYGUI_ROTATING_SKIN_H__
