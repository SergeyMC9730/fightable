#include <fightable/keyboard.h>
#include <raylib.h>

void _fKeyboardUpdate(struct fkeyboard_mgr* mgr) {
	if (!mgr) return;

	mgr->previous = mgr->current;

#ifndef TARGET_ANDROID
	for (int i = 0; i < TRACKED_INPUTS; i++) {
		int ch = mgr->registered_inputs[i];
		if (ch <= 0) continue;

		mgr->current.down[i] = IsKeyDown(ch);
		mgr->current.pressed[i] = IsKeyPressed(ch);
		mgr->current.released[i] = IsKeyReleased(ch);
		mgr->current.up[i] = IsKeyReleased(ch);
	}
#else
	
#endif

	for (int i = 0; i < TRACKED_INPUTS; i++) {
		int ch = mgr->registered_inputs[i];
		if (ch <= 0) continue;

		if (mgr->current.pressed[i] == mgr->previous.pressed[i]) {

		}
	}
}

int _fKeyboardGetIdx(struct fkeyboard_mgr* mgr, int key) {
	if (!mgr) return -1;

	for (int i = 0; i < TRACKED_INPUTS; i++) {
		if (mgr->registered_inputs[i] == key) return i;
	}

	return -1;
}

unsigned char _fKeyboardDown(struct fkeyboard_mgr* mgr, int key) {
	int idx = _fKeyboardGetIdx(mgr, key);
	if (idx < 0) return 0;

	return mgr->current.down[idx];
}
unsigned char _fKeyboardUp(struct fkeyboard_mgr* mgr, int key) {
	int idx = _fKeyboardGetIdx(mgr, key);
	if (idx < 0) return 0;

	return mgr->current.up[idx];
}
unsigned char _fKeyboardReleased(struct fkeyboard_mgr* mgr, int key) {
	int idx = _fKeyboardGetIdx(mgr, key);
	if (idx < 0) return 0;

	return mgr->current.released[idx];
}
unsigned char _fKeyboardPressed(struct fkeyboard_mgr* mgr, int key) {
	int idx = _fKeyboardGetIdx(mgr, key);
	if (idx < 0) return 0;

	return mgr->current.pressed[idx];
}

void _fKeyboardRegister(struct fkeyboard_mgr* mgr, int key) {
	if (!mgr) return;

	for (int i = 0; i < TRACKED_INPUTS; i++) {
		if (mgr->registered_inputs[i] == 0) {
			mgr->registered_inputs[i] = key;
			return;
		}
	}
}

void _fKeyboardRequestKey(struct fkeyboard_mgr* mgr, int key) {
	int idx = _fKeyboardGetIdx(mgr, key);
	if (idx < 0) return;

	mgr->current.pressed[idx] = 1;
	if (mgr->current.pressed[idx] == mgr->previous.pressed[idx]) {
		mgr->current.pressed[idx] = 0;
		mgr->current.down[idx] = 1;
	}
}