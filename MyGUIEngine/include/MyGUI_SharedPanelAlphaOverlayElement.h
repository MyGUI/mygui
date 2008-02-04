/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__
#define __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_OneOverlayDataInfo.h"

#include <OgreOverlayElement.h>
#include <OgreRenderSystem.h>

namespace MyGUI
{

	using namespace Ogre;

	class _MyGUIExport SharedPanelAlphaOverlayElement : public Ogre::OverlayContainer
	{

    public:
        /** Constructor. */
        SharedPanelAlphaOverlayElement(const String& name);
        virtual ~SharedPanelAlphaOverlayElement();

        /** Initialise */
        virtual void initialise(void);

        /** Sets whether this panel is transparent (used only as a grouping level), or 
            if it is actually renderred.
        */
        void setTransparent(bool isTransparent);

        /** Returns whether this panel is transparent. */
        bool isTransparent(void) const;

        /** See OverlayElement. */
        virtual const Ogre::String& getTypeName(void) const;
        /** See Renderable. */
        void getRenderOperation(Ogre::RenderOperation& op);
        /** Overridden from OverlayElement */
        void setMaterialName(const Ogre::String& matName);
        /** Overridden from OverlayContainer */
        void _updateRenderQueue(Ogre::RenderQueue* queue);

		/** Overridden from OverlayElement */
        void setMetricsMode(Ogre::GuiMetricsMode gmm);

    protected:
        // Flag indicating if this panel should be visual or just group things
        bool mTransparent;

        RenderOperation mRenderOp;

        /// internal method for setting up geometry, called by OverlayElement::update
        virtual void updatePositionGeometry(void);

		/// Inherited function
		virtual void updateTextureGeometry(void);

		/// Inherited function
        virtual void _update(void);
		/// Inherited function
        virtual void _updateFromParent(void);
        /** Overridden from OverlayElement */
		virtual void _notifyViewport();

        /// Method for setting up base parameters for this class
        void addBaseParameters(void);

	public:

		inline void setOverlay(Ogre::Overlay * _overlay)
		{
			mOverlay = _overlay;
		}

		void setColour(Ogre::uint32 _colour)
		{
			mColour = _colour;
			mGeomPositionsOutOfDate = true;
		}

		inline void setCountSharedOverlay(size_t _count)
		{
			mVectorSharedOverlay.resize(_count);
			mGeomPositionsOutOfDate = true;
		}

		inline void setPositionInfo(float _left, float _top, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.left = _left;
			info.top = _top;

			// если это главный оверлей, то дублируем
			if (_id == 0) Ogre::OverlayContainer::setPosition(_left, _top);

			mGeomPositionsOutOfDate = true;
		}

		inline void setPositionInfo(float _left, float _top, float _width, float _height, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.left = _left;
			info.top = _top;
			info.width = _width;
			info.height = _height;

			// если это главный оверлей, то дублируем
			if (_id == 0) Ogre::OverlayContainer::setPosition(_left, _top);

			mGeomPositionsOutOfDate = true;
		}

		inline void setDimensionInfo(float _width, float _height, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.width = _width;
			info.height = _height;
			mGeomPositionsOutOfDate = true;
		}

		inline void setTransparentInfo(bool _transparent, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.transparent = _transparent;
			mGeomPositionsOutOfDate = true;
		}

		inline void setUVInfo(float _u1, float _v1, float _u2, float _v2, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.u1 = _u1;
			info.v1 = _v1;
			info.u2 = _u2;
			info.v2 = _v2;
			mGeomPositionsOutOfDate = true;
		}

		inline bool isTransparentInfo(size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id, "index out of range");
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			return info.transparent;
		}

	protected:
		enum {
			OVERLAY_POSITION_BINDING = 0,
			OVERLAY_POSITION_HIDE = -50000
		};

	protected:
		// цвет вершины
		Ogre::uint32 mColour;
		// вектор всех наших саб оверлеев
		VectorSharedOverlay mVectorSharedOverlay;

		static String msTypeName;


	}; // class _MyGUIExport SharedPanelAlphaOverlayElement : public Ogre::OverlayContainer

} // namespace MyGUI

#endif // __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__
