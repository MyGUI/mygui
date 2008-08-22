/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

// шаблон класс прокси, для мап делегата
/*template<typename T>
class CProxyMapDelegate
{
public:
	template<TEMPLATE_PARAMS> friend class C_MAP_DELEGATE;
	typedef IDelegate<T> IDelegate;

private:
	CProxyMapDelegate() { }
	CProxyMapDelegate(CProxyMapDelegate<T> const&) { }
	CProxyMapDelegate<T> & operator = (CProxyMapDelegate<T> const&) { return *this; }

	CProxyMapDelegate(const std::string & _key, void * _owner)
		: m_key(_key)
	{
		m_owner = (C_MAP_DELEGATE<TEMPLATE_PARAMS> *)_owner;
	}

public:
	CProxyMapDelegate<T>& operator = (typename IDelegate* _delegate)
	{
		return *this;
	}

private:
	std::string m_key;
	C_MAP_DELEGATE<TEMPLATE_PARAMS> * m_owner;
};

// шаблон класса мап делегата
template<typename T>
class MapDelegate
{

public:
	typedef typename C_PROXY_MAP_DELEGATE<TEMPLATE_ARGS>::IDelegate IDelegate;
	typedef std::map<std::string, typename C_PROXY_MAP_DELEGATE<TEMPLATE_ARGS>::IDelegate *> MapDelegate;

	C_MAP_DELEGATE()
	{
	}

	~C_MAP_DELEGATE()
	{
		clear();
	}

	bool empty()
	{
		return mDelegates.empty();
	}

	bool exist(const std::string & _key)
	{
		return mDelegates.find(_key) != mDelegates.end();
	}

	void clear()
	{
		for (MapDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
			delete (*iter).second;
		}
		mDelegates.clear();
	}

	void clear(const std::string & _key)
	{
		MapDelegate::iterator iter=mDelegates.find(_key);
		if (iter != mDelegates.end()) {
			delete iter->second;
			mDelegates.erase(iter);
		}
	}

	void clear(DelegateUnlink * _unlink)
	{
		for (MapDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end();) {
			if ((*iter).second->compare(_unlink)) {
				delete (*iter).second;
				iter = mDelegates.erase(iter);
			}
			else {
				++iter;
			}
		}
	}

	typename C_PROXY_MAP_DELEGATE<TEMPLATE_ARGS> operator [] (const std::string & _key)
	{
		return C_PROXY_MAP_DELEGATE<TEMPLATE_ARGS>(_key, this);
	}

	/*void assign(const std::string & _key, void * _delegate)
	{
		IDelegate * cast = (IDelegate *)_delegate;
		
	}*/

	/*void operator()(const std::string & _key, PARAMS)
	{
		MapDelegate::iterator iter=mDelegates.find(_key);
		if (iter != mDelegates.end()) {
			(*iter).second->invoke(ARGS);
		}
	}

private:
	MapDelegate mDelegates;
};*/

void DemoKeeper::start()
{

	/*delegates::delegates::CMapDelegate1<int> map;
	map["test"] = delegates::delegates::newDelegate(test);*/


	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(57, 53);

	StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), ALIGN_DEFAULT, "Main");

	image->setImageTexture("skin_fl_AZ_new.png");
	image->addItemInfo("0.5 | 57 53 | 0 0");
	image->addItemInfo("0.5 | 57 53 | 0 53 | 0 106");
	image->addItemInfo("0.5 | 57 53 | 0 159 | 0 212");
	image->addItemNames("normal active select");
	image->setItemSelect("active");

}

void DemoKeeper::end()
{
}

