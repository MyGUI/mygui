/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#include "assert.h"
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
	#ifndef C_MAP_DELEGATE
		#define C_MAP_DELEGATE         COMBINE(CMapDelegate, SUFFIX)
	#endif
	#ifndef C_PROXY_MAP_DELEGATE
		#define C_PROXY_MAP_DELEGATE         COMBINE(CProxyMapDelegate, SUFFIX)
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
	private:
		C_STATIC_DELEGATE(C_STATIC_DELEGATE<TEMPLATE_PARAMS> const&) { }
		C_STATIC_DELEGATE<TEMPLATE_PARAMS> & operator = (C_STATIC_DELEGATE<TEMPLATE_PARAMS> const&) { return *this; }

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
	private:
		C_METHOD_DELEGATE(C_METHOD_DELEGATE<class T, TEMPLATE_PARAMS> const&) { }
		C_METHOD_DELEGATE<class T, TEMPLATE_PARAMS> & operator = (C_METHOD_DELEGATE<class T, TEMPLATE_PARAMS> const&) { return *this; }

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
	private:
		C_DELEGATE(C_DELEGATE<TEMPLATE_PARAMS> const&) { }
		C_DELEGATE<TEMPLATE_PARAMS> & operator = (C_DELEGATE<TEMPLATE_PARAMS> const&) { return *this; }

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
	private:
		C_MULTI_DELEGATE(C_MULTI_DELEGATE<TEMPLATE_PARAMS> const&) { }
		C_MULTI_DELEGATE<TEMPLATE_PARAMS> & operator = (C_MULTI_DELEGATE<TEMPLATE_PARAMS> const&) { return *this; }

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
			for (ListDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
				if (*iter) return false;
			}
			return true;
		}

		void clear()
		{
			for (ListDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
				if (*iter) {
					delete (*iter);
					(*iter) = 0;
				}
			}
		}

		void clear(DelegateUnlink * _unlink)
		{
			for (ListDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
				if ((*iter) && (*iter)->compare(_unlink)) {
					delete (*iter);
					(*iter) = 0;
				}
			}
		}

		C_MULTI_DELEGATE<TEMPLATE_ARGS>& operator+=(IDelegate* _delegate)
		{
			for (ListDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
				if ((*iter) && (*iter)->compare(_delegate)) {
					assert("dublicate delegate");
				}
			}
			mDelegates.push_back(_delegate);
			return *this;
		}

		C_MULTI_DELEGATE<TEMPLATE_ARGS>& operator-=(IDelegate* _delegate)
		{
			for (ListDelegate::iterator iter=mDelegates.begin(); iter!=mDelegates.end(); ++iter) {
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
			ListDelegate::iterator iter=mDelegates.begin();
			while (iter != mDelegates.end()) {
				if (0 == (*iter)) iter = mDelegates.erase(iter);
				else {
					(*iter)->invoke(ARGS);
					++iter;
				}
			};
		}

	private:
		ListDelegate mDelegates;
	};

	// шаблон класса мап делегата
	template<TEMPLATE_PARAMS>
	class C_MAP_DELEGATE
	{
	private:
		C_MAP_DELEGATE(C_MAP_DELEGATE<TEMPLATE_PARAMS> const&) { }
		C_MAP_DELEGATE<TEMPLATE_PARAMS> & operator = (C_MAP_DELEGATE<TEMPLATE_PARAMS> const&) { return *this; }

	private:
		//---------------------------------------------------------------
		// шаблон класс прокси, для мап делегата
		template<TEMPLATE_PARAMS>
		class C_PROXY_MAP_DELEGATE
		{
		public:
			template<TEMPLATE_PARAMS> friend class C_MAP_DELEGATE;
			typedef I_DELEGATE<TEMPLATE_ARGS> IDelegate;

		private:
			C_PROXY_MAP_DELEGATE() { }
			C_PROXY_MAP_DELEGATE(C_PROXY_MAP_DELEGATE<TEMPLATE_PARAMS> const&) { }
			C_PROXY_MAP_DELEGATE<TEMPLATE_PARAMS> & operator = (C_PROXY_MAP_DELEGATE<TEMPLATE_PARAMS> const&) { return *this; }

			C_PROXY_MAP_DELEGATE(const std::string & _key, void * _owner)
				: m_key(_key)
			{
				m_owner = (C_MAP_DELEGATE<TEMPLATE_PARAMS> *)_owner;
			}

		public:
			C_PROXY_MAP_DELEGATE<TEMPLATE_ARGS>& operator = (typename IDelegate* _delegate)
			{
				return *this;
			}

		private:
			std::string m_key;
			C_MAP_DELEGATE<TEMPLATE_PARAMS> * m_owner;
		};
		//---------------------------------------------------------------

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

		void operator()(const std::string & _key, PARAMS)
		{
			MapDelegate::iterator iter=mDelegates.find(_key);
			if (iter != mDelegates.end()) {
				(*iter).second->invoke(ARGS);
			}
		}

	private:
		MapDelegate mDelegates;
	};

} // namespace delegates
