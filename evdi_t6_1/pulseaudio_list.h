
#ifndef _PULSEAUDIO_LIST_H_
#define _PULSEAUDIO_LIST_H_
#include <pulse/pulseaudio.h>

typedef struct pa_devicelist {
	uint8_t initialized;
	char name[512];
	uint32_t index;
	char description[256];
} pa_devicelist_t;

void pa_state_cb(pa_context *c, void *userdata);
void pa_sinklist_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata);
void pa_sourcelist_cb(pa_context *c, const pa_source_info *l, int eol, void *userdata);
int pa_get_devicelist(pa_devicelist_t *input, pa_devicelist_t *output);


#endif
