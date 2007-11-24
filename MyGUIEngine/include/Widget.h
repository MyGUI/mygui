#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Prerequest.h"
#include "BasisWidget.h"
#include "WidgetSkinInfo.h"
#include "LayerItemInfo.h"
#include "WidgetUserData.h"
#include "WidgetEvent.h"
#include "WidgetFactory.h"

namespace MyGUI
{

	class _MyGUIExport Widget : public BasisWidget , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// дл€ вызова закрытых деструкторов
		friend WidgetManager;
		// дл€ вызова закрытого конструктора
		friend factory::WidgetFactory;

	protected:
		// все создание только через фабрику
		Widget(int _x, int _y, int _cx, int _cy, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// создаем дочку
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, Align _align, const Ogre::String & _name = "");

		virtual void move(int _x, int _y);
		virtual void move(int _x, int _y, int _cx, int _cy);
		virtual void size(int _cx, int _cy);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		const Ogre::ColourValue & getColour() {return m_color;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(Align _align);

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};
	
		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// закрываем метод базового класса
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

	protected:
		void update(); // обновлени€ себ€ и детей

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		inline const Ogre::String & getName() {return m_name;};

		// присоедин€емс€ к отцу
		void attach(BasisWidgetPtr _basis, bool _child);

		// уд€л€ет только негодных батюшке государю
		void destroyWidget(WidgetPtr & _widget);
		// удал€ет всех детей
		void destroyWidget();

		// наследуетс€ и переопредел€етс€ дл€ избежани€ динамического преобразовани€
		bool isWidget() {return true;}

		// вспомогательный метод дл€ распарсивани€ сабвиджетофф
		WidgetPtr parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size);

	public:
		// возвращает указатель на айтем в этой точке попадание в виджет (наследуетьс€ от LayerItemInfo)
		LayerItemInfoPtr findItem(int _x, int _y);

		inline bool isNeedKeyFocus() {return m_needKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {m_needKeyFocus = _need;}
		inline bool isEnable() {return m_enable;}

	protected:

		// создаем и добавл€ем саб скин виджету
		BasisWidgetPtr addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material);

	public:
		// закрытый деструктор
		virtual ~Widget();

	protected:

		// список всех стейтов
		const MapWidgetStateInfo & m_stateInfo;
		// показывает скрывает все сабскины
		void visible(bool _visible);

		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// вектор всех детей сабскинов
		VectorBasisWidgetPtr m_subSkinChild;

		// указатель на окно текста
		BasisWidgetPtr m_text;

		// доступен ли на виджет
		bool m_enable;
		// скрыты ли все сабскины при выходе за границу
		bool m_visible;
		// прозрачность нашего оверле€
		float m_alpha;
		// цвет текста
		Ogre::ColourValue m_color;
		// им€ виджета
		Ogre::String m_name;

	}; // class Widget

} // namespace MyGUI


#endif // __WIDGET_H__