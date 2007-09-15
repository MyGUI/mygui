//#ifndef _DELEGATEIMPLEMENT_H_
//#define _DELEGATEIMPLEMENT_H_

#include "Prerequest.h"


namespace widget
{

	#define COMBINE(a,b)       COMBINE1(a,b)
	#define COMBINE1(a,b)      a##b

	#define I_DELEGATE         COMBINE(IDelegate, SUFFIX)
	#define C_STATIC_DELEGATE  COMBINE(CStaticDelegate, SUFFIX)
	#define C_METHOD_DELEGATE  COMBINE(CMethodDelegate, SUFFIX)
	#define C_DELEGATE         COMBINE(CDelegate, SUFFIX)

	// базовый класс всех делегатов
	template<TEMPLATE_PARAMS>
	class I_DELEGATE
	{
	public:
		virtual ~I_DELEGATE() {}
		virtual void Invoke(PARAMS) = 0;
	};

	// делегат для статической функции
	template<TEMPLATE_PARAMS>
	class C_STATIC_DELEGATE : public I_DELEGATE<TEMPLATE_ARGS>
	{
	public:
		typedef void (*PFunc)(PARAMS);
		C_STATIC_DELEGATE(PFunc pFunc) { m_pFunc = pFunc; }
		virtual void Invoke(PARAMS) { m_pFunc(ARGS); }

	private:
		PFunc m_pFunc;
	};

	// делегат для метода класса
	template<class TObj, TEMPLATE_PARAMS>
	class C_METHOD_DELEGATE : public I_DELEGATE<TEMPLATE_ARGS>
	{
	public:
		typedef void (TObj::*PMethod)(PARAMS);
		C_METHOD_DELEGATE(TObj* pObj, PMethod pMethod)
		{
			m_pObj = pObj;
			m_pMethod = pMethod;
		}
		virtual void Invoke(PARAMS) { (m_pObj->*m_pMethod)(ARGS); }

	private:
		TObj *m_pObj;
		PMethod m_pMethod;
	};


	// шаблон для создания делегата статической функции
	// параметры : указатель на функцию
	// пример : newDelegate(funk_name);
	template<TEMPLATE_PARAMS>
	I_DELEGATE<TEMPLATE_ARGS>* newDelegate(void (*pFunc)(PARAMS))
	{
		return new C_STATIC_DELEGATE<TEMPLATE_ARGS>(pFunc);
	}

	// шаблон для создания делегата метода класса
	// параметры : указатель на объект класса и указатель на метод класса
	// пример : newDelegate(&object_name, &class_name::method_name);
	template <class TObj, TEMPLATE_PARAMS>
	I_DELEGATE<TEMPLATE_ARGS>* newDelegate(TObj* pObj, void (TObj::*pMethod)(PARAMS))
	{
		return new C_METHOD_DELEGATE<TObj, TEMPLATE_ARGS> (pObj, pMethod);
	}


	// шаблон класса делегата
	template<TEMPLATE_PARAMS>
	class C_DELEGATE
	{
	public:
		typedef I_DELEGATE<TEMPLATE_ARGS> IDelegate;

		C_DELEGATE() : m_pDelegate (0) {}
		~C_DELEGATE() { if (m_pDelegate) delete m_pDelegate; }

		bool IsNull() { return (m_pDelegate == 0); }

		C_DELEGATE<TEMPLATE_ARGS>& operator=(IDelegate* pDelegate)
		{
			if (m_pDelegate) delete m_pDelegate;
			m_pDelegate = pDelegate;
			return *this;
		}

		void operator()(PARAMS)
		{
			if (m_pDelegate == 0) return;
			m_pDelegate->Invoke(ARGS);
		}

	private:
		IDelegate * m_pDelegate;
	};

} // namespace widget

//#endif
