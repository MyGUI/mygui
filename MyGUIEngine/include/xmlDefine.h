#ifndef _XMLDEFINE_H_
#define _XMLDEFINE_H_

namespace xml
{

	typedef enum NodeType	 {
		XML_NODE_TYPE_REMARK, // коментарий
		XML_NODE_TYPE_INFO, // информационный блок
		XML_NODE_TYPE_NORMAL, // обыкновенный блок
	};

	namespace errors
	{

		typedef enum ErrorTypes {
			XML_ERROR_NONE,
			XML_ERROR_OPEN_FILE,
			XML_ERROR_CREATE_FILE,
			XML_ERROR_BODY_NON_CORRECT,
			XML_ERROR_NON_CLOSE_ALL_TAGS,
			XML_ERROR_DOCUMENT_IS_EMPTY,
			XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG,
			XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT,
			XML_ERROR_INFO_IS_EXIST,
			XML_ERROR_ROOT_IS_EXIST,
			XML_ERROR_ATTRIBUTE_NON_CORRECT,
			XML_ERROR_COUNT
		};

	} // namespace errors

} // namespace xml

#endif
