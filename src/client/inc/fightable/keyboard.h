#pragma once

#define TRACKED_INPUTS 8

//#ifdef __cplusplus
//extern "C" {
//#endif

struct fkeyboard_frame {
	unsigned char down[TRACKED_INPUTS];
	unsigned char pressed[TRACKED_INPUTS];
	unsigned char released[TRACKED_INPUTS];
	unsigned char up[TRACKED_INPUTS];
};

struct fkeyboard_mgr {
	struct fkeyboard_frame current;
	struct fkeyboard_frame previous;

	int registered_inputs[TRACKED_INPUTS];
};

void _fKeyboardUpdate(struct fkeyboard_mgr* mgr);

unsigned char _fKeyboardDown(struct fkeyboard_mgr* mgr, int key);
unsigned char _fKeyboardUp(struct fkeyboard_mgr* mgr, int key);
unsigned char _fKeyboardReleased(struct fkeyboard_mgr* mgr, int key);
unsigned char _fKeyboardPressed(struct fkeyboard_mgr* mgr, int key);

void _fKeyboardRequestKey(struct fkeyboard_mgr* mgr, int key);

void _fKeyboardRegister(struct fkeyboard_mgr* mgr, int key);

//#ifdef __cplusplus
//}
//#endif