
#include <windows.h>

#include "DefaultLayer.h"
#include "Widget.h"

using namespace MyGUI;

int APIENTRY WinMain(HINSTANCE hThisInst, HINSTANCE hLastInst, LPSTR lpCmdLine, int nCmdShow)
{

	// создаем один виджет, и ему создаем одно дитя
	// каждый виджет, будет содержать по два квада
	// у каждого виджета своя текстура
	Widget* widget1 = new Widget("texture1");
	Widget* widget2 = widget1->createChild("texture2");





	// создает тестовый леер, в котором будут наши айтемы
	DefaultLayer* layer = new DefaultLayer("Deafult");
	// создаем один рутовый нод
	ILayerNode* node1 = layer->createItemNode(0);
	// присоединяем к нему рутовый виджет
	node1->attachLayerItem(widget1);




	// а теперь рисуем леер
	// леер, передаст событие о рисовании, всем рутовым нодам
	// рутовые ноды, нарисуют свои рендер айтемы
	layer->doRender(true);


}