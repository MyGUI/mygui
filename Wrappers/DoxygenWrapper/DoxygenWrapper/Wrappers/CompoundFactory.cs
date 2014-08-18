using System;
using System.Collections.Generic;
using DoxygenWrapper.Wrappers.Compounds;

namespace DoxygenWrapper.Wrappers
{
	public class CompoundFactory
	{
		public void RegisterFactory(string _type, Type _instance)
		{
			if (!CheckBaseType(_instance, typeof(Compound)))
				throw new ArgumentException(string.Format("Base type of {0} != {1}", _instance, typeof(Compound)));

			if (mFactory.ContainsKey(_type))
				throw new ArgumentException(string.Format("{0} already registred", _type));

			mFactory[_type] = _instance;
		}

		private bool CheckBaseType(Type _derived, Type _base)
		{
			if (_derived == typeof(Object))
				return false;
			if (_derived == _base)
				return true;
			return CheckBaseType(_derived.BaseType, _base);
		}

		public Compound CreateCompound(string _type)
		{
			if (!mFactory.ContainsKey(_type))
				return null;

			object compound = Activator.CreateInstance(mFactory[_type]);
			return (Compound)compound;
		}

		Dictionary<string, Type> mFactory = new Dictionary<string, Type>();
	}
}
