/*!
	@file
	@author		George Evmenov
	@date		01/2011
*/

namespace delegates
{

	#define MYGUI_COMBINE(a, b)									MYGUI_COMBINE1(a, b)
	#define MYGUI_COMBINE1(a, b)								a##b

	#define MYGUI_I_DELEGATE									MYGUI_COMBINE(IDelegate, MYGUI_SUFFIX)

	#define MYGUI_FUNCTOR_DELEGATE					MYGUI_COMBINE(FunctorDelegate, MYGUI_SUFFIX)

	// Functor delegate.
	MYGUI_TEMPLATE MYGUI_TEMPLATE_PARAMS
	class MYGUI_FUNCTOR_DELEGATE : public  MYGUI_I_DELEGATE MYGUI_TEMPLATE_ARGS
	{
	public:
		MYGUI_FUNCTOR_DELEGATE(const boost::function <void (MYGUI_PARAMS)> &_functor) : mFunctor(_functor) { }

		virtual bool isType( const std::type_info& _type)
		{
			return typeid( MYGUI_FUNCTOR_DELEGATE MYGUI_TEMPLATE_ARGS ) == _type;
		}

		virtual void invoke( MYGUI_PARAMS )
		{
			mFunctor( MYGUI_ARGS );
		}

		virtual bool compare(  MYGUI_I_DELEGATE MYGUI_TEMPLATE_ARGS* _delegate) const
		{
			return false;
		}

		virtual bool compare(IDelegateUnlink* _unlink) const
		{
			return false;
		}

	private:
		boost::function <void (MYGUI_PARAMS)> mFunctor;
	};


} // namespace delegates

MYGUI_TEMPLATE MYGUI_TEMPLATE_PARAMS
inline  delegates::MYGUI_I_DELEGATE MYGUI_TEMPLATE_ARGS* newDelegate(const boost::function <void (MYGUI_PARAMS)> & _functor)
{
	return new delegates::MYGUI_FUNCTOR_DELEGATE MYGUI_TEMPLATE_ARGS  (_functor);
}

#undef MYGUI_COMBINE
#undef MYGUI_COMBINE1

#undef MYGUI_I_DELEGATE

#undef MYGUI_FUNCTOR_DELEGATE

#undef MYGUI_SUFFIX
#undef MYGUI_TEMPLATE
#undef MYGUI_TEMPLATE_PARAMS
#undef MYGUI_TEMPLATE_ARGS
#undef MYGUI_T_TEMPLATE_PARAMS
#undef MYGUI_T_TEMPLATE_ARGS
#undef MYGUI_PARAMS
#undef MYGUI_ARGS
#undef MYGUI_TYPENAME
