#ifndef __INSTANCE_H__
#define __INSTANCE_H__


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
	type& type::getInstance(void) {assert(msInstance); return (*msInstance);} \
	type::type() : mIsInitialise(false) {assert(!msInstance);msInstance=this;} \
	type::~type() {msInstance=0;}

#endif // __INSTANCE_H__
