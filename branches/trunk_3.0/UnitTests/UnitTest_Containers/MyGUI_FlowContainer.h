/*!
	@file
	@author		Alexander Ptakhin
	@date		04/2009
	@module
*/
#ifndef __MYGUI_FLOW_CONTAINER_H__
#define __MYGUI_FLOW_CONTAINER_H__

#include "MyGUI_Container.h"
#include "MyGUI_Window.h"

/*

BUGS:
:-)

TODO
min/max

*/

namespace MyGUI
{

	// Internal
	class WidgetParamWrap_Width{};

	// Internal
	class WidgetParamWrap_Height{};

	/// MyGUI Internal
	template<typename T>
	struct WidgetParamWrap
	{
		//const SizeDescription& desc;
		const Dimension& size;
		const Dimension& minSize;
		const Dimension& maxSize;

		const SizeParam& dim() const { MYGUI_EXCEPT("Wrong templation!"); }
		const SizeParam& minDim() const { MYGUI_EXCEPT("Wrong templation!"); }
		const SizeParam& maxDim() const { MYGUI_EXCEPT("Wrong templation!"); } 

		WidgetParamWrap(const Dimension& _size, const Dimension& _minSize, const Dimension& _maxSize)
			:	size(_size), minSize(_minSize), maxSize(_maxSize) {}
	};

	class FlowContainer;
	typedef FlowContainer* FlowContainerPtr;

	enum 
	{
		HORZ,
		VERT,
	};

	/** Flow container. Data stores from left to right. 
	Modes:
	Break lines:
	Wider: If there is no place for widget, set it next row.
	*/
	class /*MYGUI_EXPORT*/ FlowContainer : public Container
	{
		MYGUI_RTTI_DERIVED( FlowContainer )
	public:

		class WidgetInfo : Container::BaseWidgetInfo
		{
		protected:
			WidgetInfo(Widget* _widget);

			IntSize mCurrentSize;

		protected:

			void setCurrentSize(const IntSize& _size) { mCurrentSize = _size; }

		public:
			/// Notem it's not your proprty but it's real current sizes of widget.
			IntSize getCurrentSize() const { return mCurrentSize; }

			bool lineBreak;
			//SizeDescription sizeDesc;
			bool autoLineBreak;

			Dimension size;
			Dimension minSize;
			Dimension maxSize;

		protected:
			bool mWasLoaded;

			void _load();

			friend class FlowContainer;
		};

	public:

		/// Ovarlap
		void _destroyChildWidget(Widget* _widget);

		WidgetInfo* getWidgetInfo(Widget* _widget);

		typedef std::list<WidgetInfo> ListWidgetInfo;
		typedef std::list<WidgetInfo>::iterator ListWidgetInfoIter;

		enum WidgetTag
		{
			WT_ALL = (int)-1,
			WT_SPACER = MYGUI_FLAG(1),
			WT_FREE = MYGUI_FLAG(2),
			WT_PARENT = MYGUI_FLAG(3),
			WT_NOT_SPACER = MYGUI_FLAG(4),
		};

		struct SizeData
		{
			float freeCoeff;
			float parentCoeff;
			float spacersCoeff;

			/// MinimizeBits
			int state;

			SizeData() :
				parentCoeff(0.0f),
				freeCoeff(0.0f),
				spacersCoeff(0.0f),
				state(WT_ALL)
			{
			}
		};

	protected:
		struct RowData : public SizeData
		{
			IntPoint pos;
			ListWidgetInfoIter first;
			ListWidgetInfoIter last; // last non-space 
			ListWidgetInfoIter lineBreak;// which breaks
			ListWidgetInfoIter next;
			bool autoLineBreak;
			IntSize size;
			int maxWidth;

			RowData() : autoLineBreak(false), maxWidth(0) {}
		};

		struct RowInput
		{
			IntPoint cur;
			ListWidgetInfoIter from;
			int maxWidth;
		};

	public:
		bool isCoeff(float _coeff) const { return fabs(_coeff)> 0.00001; }

		bool isSpacer(/*const */Widget* _widget) const;

	protected:
		bool getRowData(const RowInput& _in, RowData& _result);

		void placeWidgets(const RowData& _data);

		struct CalcRow
		{
			IntSize size;
			ListWidgetInfoIter last;
		};

	
		float calcFlWidthSum(const RowData& _data, FloatMode _mode);

		bool calcPxWidthSum(const RowData& _data, CalcRow& _calcRow, int _widgetTags);

		float calcFlHeightSum(FloatMode _mode);

		/* Policies? What policies? :) */
		template<class T>
		int _getWidgetPxDimension(const SizeData& _sizeData, const T& _size, const WidgetInfo& _info);

		template<class T>
		float _getWidgetFlDimension(const SizeData& _sizeData, const T& _size, const WidgetInfo& _info);

		int getWidgetPxWidth(const RowData& _data, const WidgetInfo& _info);

		int getWidgetPxHeight(const RowData& _data, const WidgetInfo& _info);
	
		float getWidgetFlWidth(const RowData& _data, const WidgetInfo& _info);

		float getWidgetFlHeight(const RowData& _data, const WidgetInfo& _info);

		IntSize getWidgetPxSize(const RowData& _data, const WidgetInfo& _info);

		template<class T>
		bool _isWidgetComply(int _widgetTags, const T& _size, const WidgetInfo& _info) const;

		bool isWidgetWidthComply(int _widgetTags, const WidgetInfo& _info) const;

		bool isWidgetHeightComply(int _widgetTags, const WidgetInfo& _info) const;

		// TODO:
		//IntSize getWidgetMinSize(const WidgetInfo& _info) const;

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, const std::string& _name);

	public:

		void add(Widget* _widget);

		void remove(Widget* _widget);

		void updateWidgetInfo(Widget* _widget);

		void updateWidgetInfo(WidgetInfo& _widgetInfo);

		// Bad hack
		void updateAllWidgetInfos();

		void update();
	
	protected:
		
		ListWidgetInfo mWidgetsInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_FLOW_CONTAINER_H__
