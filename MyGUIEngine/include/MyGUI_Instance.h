/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_INSTANCE_H__
#define __MYGUI_INSTANCE_H__

#define MYGUI_INSTANCE_HEADER(type) \
	private: \
	static type* msInstance; \
	bool mIsInitialise; \
	public: \
	type();\
	~type();\
	static type& getInstance(void); \
	static type* getInstancePtr(void);


#define MYGUI_INSTANCE_IMPLEMENT(type) \
	type* type::msInstance = 0; \
	type* type::getInstancePtr(void) {return msInstance;} \
	type& type::getInstance(void) {MYGUI_ASSERT(0 != msInstance, "instance " << #type << " was not created");return (*msInstance);} \
	type::type() : mIsInitialise(false) {MYGUI_ASSERT(0 == msInstance, "instance " << #type << " is exsist");msInstance=this;} \
	type::~type() {msInstance=0;} \
	const std::string INSTANCE_TYPE_NAME(#type);

#endif // __MYGUI_INSTANCE_H__
