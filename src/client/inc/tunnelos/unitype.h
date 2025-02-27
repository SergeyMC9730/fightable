#pragma once

#include <stdint.h>

#define UNITYPE_TYPESTRING 's'
#define UNITYPE_TYPEINT    'i'
#define UNITYPE_TYPELONG   'l'
#define UNITYPE_TYPESHORT  'h'
#define UNITYPE_TYPEBYTE   'y'
#define UNITYPE_TYPEFLOAT  'f'
#define UNITYPE_TYPEDOUBLE 'd'
#define UNITYPE_TYPEFUN    'c'
#define UNITYPE_TYPEBASE   'b'
#define UNITYPE_TYPENULL   'n'

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct unitype_t unitype_t;
	struct unitype_t {
		void* p;
		unitype_t* next;
		unitype_t* prev;
		const char* name;
		char type;
		unsigned int p_sz;
	};
	typedef struct {
		unitype_t* (*func)(unitype_t* args);
		const char* name;
	} unifunc_t;

	/*
	* Every object will have placeholder name such as "short", "string", "null" and etc. You need to replace them with own names or you will get into troubles while searching objects using __uni_find.
	* Variable Template looks like this: "siy" => string, integer and byte
	*
	* String variable contains array of 256 characters
	*/
	unitype_t* __uni_create(const char* variable_template);
	// Internal function
	unitype_t* __uni_createbase(char* c, char t);

	// Delete variable
	void __uni_delete(unitype_t* args, unsigned char delete_children);
	// Find variable
	unitype_t* __uni_find(unitype_t* args, const char* name);
	// Add variable
	void __uni_add(unitype_t* args, unitype_t* entry);
	// Find last entry
	unitype_t* __uni_last(unitype_t* args);
	// Find first entry
	unitype_t* __uni_first(unitype_t* args);
#pragma pack(pop)

#ifdef __cplusplus
}
#endif