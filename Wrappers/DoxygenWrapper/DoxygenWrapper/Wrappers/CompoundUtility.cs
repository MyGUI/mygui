using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Compounds.Types;
using DoxygenWrapper.Wrappers.Compounds;

namespace DoxygenWrapper.Wrappers
{
	public class CompoundUtility
	{
		// возвращает тип без typedef ов
		public static CompoundType GetUnwrappedType(CompoundType _type)
		{
			CompoundType type = _type;
			while (type.BaseType is CompoundTypedef)
				type = ((CompoundTypedef)type.BaseType).CompoundType;
			return type;
		}

		public static CompoundType GetEventType(CompoundVariable _variable)
		{
			CompoundType type = GetUnwrappedType(_variable.CompoundType);
			if (type.TypeName.StartsWith("MyGUI::EventPair") ||
				type.TypeName.StartsWith("MyGUI::EventPair3to4"))
			{
				if (type.TemplateTypes.Count == 2)
				{
					type = GetUnwrappedType(type.TemplateTypes[1]);
				}
			}

			return type;
		}

		public static bool IsVariableEvent(CompoundVariable _variable)
		{
			CompoundType eventType = GetEventType(_variable);
			return eventType.TypeName.StartsWith("MyGUI::delegates::CDelegate") ||
				eventType.TypeName.StartsWith("MyGUI::delegates::CMultiDelegate");
		}

		public static bool IsEventMultiDelegate(CompoundType _type)
		{
			return _type.TypeName.StartsWith("MyGUI::delegates::CMultiDelegate");
		}
	}
}
