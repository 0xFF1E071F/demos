#ifndef USYNC_H
#define USYNC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SYNC_PLAYER

#include "sync-data.h"

extern float usync_values[SYNC_TRACK_COUNT];

/* tiny api */
#define usync_init() 0
void usync_update(float t);
#define usync_get_val(x) usync_values[ SYNC_TRACK_##x ]
#define usync_export()

#else /* !defined(SYNC_PLAYER) */

#include "sync.h"

extern struct sync_device *usync_dev;
extern float usync_time;

int usync_init(void);
void usync_update(float t);
#define usync_get_val(track) sync_get_val(sync_get_track(usync_dev, #track), usync_time)
void usync_export(const char *path);

/* implement these yourself */
extern struct sync_cb usync_cb;
extern void *usync_cb_data;

#endif /* !defined(SYNC_PLAYER) */

#ifdef __cplusplus
}
#endif

#endif /* !defined(USYNC_H) */
