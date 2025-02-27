#include <tunnelos/unitype.h>
#include <stdlib.h>
#include <string.h>

unitype_t* __placeholder_unitype(unitype_t* args) {
	return 0;
}
unitype_t* __uni_createbase(char* c, char t) {
	unitype_t* res = (unitype_t*)malloc(sizeof(unitype_t));

	res->name = c;
	res->prev = NULL;
	res->next = NULL;
	res->type = t;
	res->p = NULL;
	res->p_sz = 0;

	size_t sz = 0;

	switch (t) {
	case UNITYPE_TYPESTRING: {
		sz = 256;
		break;
	}
	case UNITYPE_TYPEINT: {
		sz = sizeof(int);
		break;
	}
	case UNITYPE_TYPELONG: {
		sz = sizeof(long);
		break;
	}
	case UNITYPE_TYPESHORT: {
		sz = sizeof(short);
		break;
	}
	case UNITYPE_TYPEBYTE: {
		sz = sizeof(uint8_t);
		break;
	}
	case UNITYPE_TYPEFLOAT: {
		sz = sizeof(float);
		break;
	}
	case UNITYPE_TYPEDOUBLE: {
		sz = sizeof(double);
		break;
	}
	case UNITYPE_TYPEFUN: {
		unifunc_t* fun = (unifunc_t*)malloc(sizeof(unifunc_t));
		fun->func = __placeholder_unitype;
		fun->name = "Placeholder function. Replace with your own.";
		res->p = fun;
		break;
	}
	default: {}
	}

	if (sz != 0) {
		res->p = malloc(sz);
		memset(res->p, 0, sz);
	}

	res->p_sz = sz;

	return res;
}

unitype_t* __uni_create(const char* mask) {
	if (!mask) return NULL;

	unitype_t* res = (unitype_t*)malloc(sizeof(unitype_t));
	memset(res, 0, sizeof(unitype_t));
	res->name = "base";
	res->type = UNITYPE_TYPEBASE;
	res->p = res;

	// Parse mask
	if (mask) {
		int len = strlen(mask);
		int i = 0;
		while (i < len) {
			unitype_t* e = __uni_createbase(" ", mask[i]);
			__uni_add(res, e);
			i++;
		}
	}

	return res;
}

void __uni_delete(unitype_t* args, unsigned char delete_children) {
	if (!args) return;

	if (delete_children) {
		unitype_t* u = __uni_last(args);

		while (u != args && u != NULL) {
			unitype_t* t = u->prev;

			__uni_delete(u, 0);

			u = t;
		}
	}

	free(args->p);
	free(args);
}
unitype_t* __uni_find(unitype_t* args, const char* name) {
	if (!name) return NULL;

	unitype_t* u = args;

	while (u != NULL) {
		if (name != NULL && !strcmp(u->name, name)) {
			return u;
		}
		else {
			u = u->next;
		}
	}
	return NULL;
}

unitype_t* __uni_last(unitype_t* args) {
	if (!args) return NULL;

	unitype_t* up = args;
	unitype_t* u = up->next;

	while (u != NULL) {
		up = u;
		u = u->next;
	}

	return up;
}

unitype_t* __uni_first(unitype_t* args) {
	unitype_t* up = args;
	unitype_t* u = up->prev;

	while (u != NULL) {
		up = u;
		u = u->prev;
	}

	return up;
}

void __uni_add(unitype_t* args, unitype_t* entry) {
	if (!args || !entry) return;

	unitype_t* last = __uni_last(args);
	last->next = entry;
	entry->prev = last;
	entry->next = NULL;
}