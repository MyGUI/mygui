/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#include <list>

namespace delegates
{
	#ifndef COMBINE
		#define COMBINE(a,b)       COMBINE1(a,b)
	#endif
	#define COMBINE1(a,b)      a##b

	#define I_DELEGATE         COMBINE(IDelegate, SUFFIX)
	#define C_STATIC_DELEGATE  COMBINE(CStaticDelegate, SUFFIX)
	#define C_METHOD_DELEGATE  COMBINE(CMethodDelegate, SUFFIX)
	#ifndef C_DELEGATE
		#define C_DELEGATE         COMBINE(CDelegate, SUFFIX)
	#endif

	#ifndef C_MULTI_DELEGATE
		#define C_MULTI_DELEGATE         COMBINE(CMultiDelegate, SUFFIX)
	#endif

	// базовый класс всех делегатов
	template<TEMPLATE_PARAMS>
	class I_DELEGATE
	{
	public:
		I_DELEGATE<TEMPLATE_ARGS>(bool _static) :
			mStatic(_static)
		{
		}

		virtual ~I_DELEGATE() {}
		virtual void invoke(PARAMS) = 0;
		virtual bool compare(I_DELEGATE<TEMPLATE_ARGS>* _delegate) = 0;
		virtual bool compare(DelegateUnlink * _unlink) { return false; }
		inline bool isStatic() { return mStatic; }

	protected:
		bool mStatic;
	};

	// делегат для статической функции
	template<TEMPLATE_PARAMS>
	class C_STATIC_DELEGATE : public I_DELEGATE<TEMPLATE_ARGS>
	{
	public:
		typedef void (*Func)(PARAMS);

		C_STATIC_DELEGATE(Func _func) : 
			I_DELEGATE<TEMPLATE_ARGS>(true),
			mFunc(_func)
		{
		}

		virtual void invoke(PARAMS)
		{
			mFunc(ARGS);
		}

		virtual bool compare(I_DELEGATE<TEMPLATE_ARGS>* _delegate)
		{
			if (!_delegate || !_delegate->isStatic()) return false;
			C_STATIC_DELEGATE<TEMPLATE_ARGS>* cast =
				static_cast<C_STATIC_DELEGATE<TEMPLATE_ARGS>*>(_delegate);
			if (cast->mFunc != mFunc) return false;
			return true;
		}

	private:
		Func mFunc;
	};

	// делегат для метода класса
	template<class T, TEMPLATE_PARAMS>
	class C_METHOD_DELEGATE : public I_DELEGATE<TEMPLATE_ARGS>
	{
	public:
		typedef void (T::*Method)(PARAMS);

		C_METHOD_DELEGATE(DelegateUnlink * _unlink, T * _object, Method _method) :
			I_DELEGATE<TEMPLATE_ARGS>(false),
			mUnlink(_unlink),
			mObject(_object),
			mMethod(_method)
		{
		}

		virtual void invoke(PARAMS)
		{
			(mObject->*mMethod)(ARGS);
		}

		virtual bool compare(I_DELEGATE<TEMPLATE_ARGS>* _delegate)
		{
			if (!_delegate || _delegate->isStatic()) return false;
			C_METHOD_DELEGATE<T, TEMPLATE_ARGS>* cast =
				static_cast<C_METHOD_DELEGATE<T, TEMPLATE_ARGS>*>(_delegate);
			if ( cast->mObject != mObject || cast->mMethod != mMethod ) return false;
			return true;
		}

		virtual bool compare(DelegateUnlink * _unlink)
		{
			return mUnlink == _unlink;
		}

	private:
		DelegateUnlink *mUnlink;
		T * mObject;
		Method mMethod;
	};


	// шаблон для создания делегата статической функции
	// параметры : указатель на функцию
	// пример : newDelegate(funk_name);
	template<TEMPLATE_PARAMS>
	I_DELEGATE<TEMPLATE_ARGS>* newDelegate(void (*_func)(PARAMS))
	{
		return new C_STATIC_DELEGATE<TEMPLATE_ARGS>(_func);
	}

	// шаблон для создания делегата метода класса
	// параметры : указатель на объект класса и указатель на метод класса
	// пример : newDelegate(&object_name, &class_name::method_name);
	template <class T, TEMPLATE_PARAMS>
	I_DELEGATE<TEMPLATE_ARGS>* newDelegate(T * _object, void (T::*_method)(PARAMS))
	{
		return new C_METHOD_DELEGATE<T, TEMPLATE_ARGS> (GetDelegateUnlink(_object), _object, _method);
	}

	// шаблон класса делегата
	template<TEMPLATE_PARAMS>
	class C_DELEGATE
	{
	public:
		typedef I_DELEGATE<TEMPLATE_ARGS> IDelegate;

		C_DELEGATE() :
			mDelegate(0)
		{
		}

		~C_DELEGATE()
		{
			clear();
		}

		bool empty()
		{
			return mDelegate == 0;
		}

		void clear()
		{
			if (mDelegate) {
				delete mDelegate;
				mDelegate = 0;
			}
		}

		C_DELEGATE<TEMPLATE_ARGS>& operator=(IDelegate* _delegate)
		{
			if (mDelegate) {
				delete mDelegate;
			}
			mDelegate = _delegate;
			return *this;
		}

		void operator()(PARAMS)
		{
			if (mDelegate == 0) return;
			mDelegate->invoke(ARGS);
		}

	private:
		IDelegate * mDelegate;
	};

	// шаблон класса мульти делегата
	template<TEMPLATE_PARAMS>
	class C_MULTI_DELEGATE
	{
	public:
		typedef I_DELEGATE<TEMPLATE_ARGS> IDelegate;
		typedef std::list<IDelegate *> ListDelegate;

		C_MULTI_DELEGATE()
		{
		}

		~C_MULTI_DELEGATE()
		{
			clear();
		}

		bool empty()
		{
			typename ListDelegate::iterator iter;
			for (iter=mListDelegates.begin(); iter!=mListDelegates.end(); ++iter) {
				if (*iter) return false;
			}
			return true;
		}

		void clear()
		{
			typename ListDelegate::iterator iter;
			for (iter=mListDelegates.begin(); iter!=mListDelegates.end(); ++iter) {
				if (*iter) {
					delete (*iter);
					(*iter) = 0;
				}
			}
		}

		void clear(DelegateUnlink * _unlink)
		{
			typename ListDelegate::iterator iter;
			for (iter=mListDelegates.begin(); iter!=mListDelegates.end(); ++iter) {
				if ((*iter) && (*iter)->compare(_unlink)) {
					delete (*iter);
					(*iter) = 0;
				}
			}
		}

		C_MULTI_DELEGATE<TEMPLATE_ARGS>& operator+=(IDelegate* _delegate)
		{
			typename ListDelegate::iterator iter;
			for (iter=mListDelegates.begin(); iter!=mListDelegates.end(); ++iter) {
				if ((*iter) && (*iter)->compare(_delegate)) {
                                  assert("dublicate delegate");
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		C_MULTI_DELEGATE<TEMPLATE_ARGS>& operator-=(IDelegate* _delegate)
		{
			typename ListDelegate::iterator iter;
			for (iter=mListDelegates.begin(); iter!=mListDelegates.end(); ++iter) {
				if ((*iter) && (*iter)->compare(_delegate)) {
					// проверяем на идентичность делегатов
					if ((*iter) != _delegate) delete (*iter);
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		void operator()(PARAMS)
		{
			typename ListDelegate::iterator iter=mListDelegates.begin();
			while (iter != mListDelegates.end()) {
				if (0 == (*iter)) iter = mListDelegates.erase(iter);
				else {
					(*iter)->invoke(ARGS);
					++iter;
				}
			};
		}

	private:
		ListDelegate mListDelegates;
	};

} // namespace delegates
