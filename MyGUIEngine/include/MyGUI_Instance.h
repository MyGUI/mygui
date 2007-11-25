/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_INSTANCE_H__
#define __MYGUI_INSTANCE_H__

#define INSTANCE_HEADER(type) \
	private: \
	static type* msInstance; \
	bool mIsInitialise; \
	public: \
	type();\
	~type();\
	static type& getInstance(void); \
	static type* getInstancePtr(void);


#define INSTANCE_IMPLEMENT(type) \
	type* type::msInstance = 0; \
	type* type::getInstancePtr(void) {return msInstance;} \
	type& type::getInstance(void) {MYGUI_ASSERT(null != msInstance);return (*msInstance);} \
	type::type() : mIsInitialise(false) {MYGUI_ASSERT(null == msInstance);msInstance=this;} \
	type::~type() {msInstance=0;} \
	const std::string INSTANCE_TYPE_NAME(#type);

#endif // __MYGUI_INSTANCE_H__
