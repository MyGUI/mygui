
#include <windows.h>

#include "DefaultLayer.h"
#include "Widget.h"

using namespace MyGUI;

int APIENTRY WinMain(HINSTANCE hThisInst, HINSTANCE hLastInst, LPSTR lpCmdLine, int nCmdShow)
{

	// ������� ���� ������, � ��� ������� ���� ����
	// ������ ������, ����� ��������� �� ��� �����
	// � ������� ������� ���� ��������
	Widget* widget1 = new Widget("texture1");
	Widget* widget2 = widget1->createChild("texture2");





	// ������� �������� ����, � ������� ����� ���� ������
	DefaultLayer* layer = new DefaultLayer("Deafult");
	// ������� ���� ������� ���
	ILayerNode* node1 = layer->createItemNode(0);
	// ������������ � ���� ������� ������
	node1->attachLayerItem(widget1);




	// � ������ ������ ����
	// ����, �������� ������� � ���������, ���� ������� �����
	// ������� ����, �������� ���� ������ ������
	layer->doRender(true);


}