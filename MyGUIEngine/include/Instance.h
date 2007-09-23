#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#define INSTANCE_HEADER(type) \
	private: \
	static type* msInstance; \
	public: \
	static type& getInstance(void); \
	static type* getInstancePtr(void);

#define INSTANCE_IMPLEMENT(type) \
	type* type::msInstance = 0; \
	type* type::getInstancePtr(void) {return msInstance;} \
	type& type::getInstance(void) {if (!msInstance)msInstance=new type();return (*msInstance);}

#endif // __INSTANCE_H__
