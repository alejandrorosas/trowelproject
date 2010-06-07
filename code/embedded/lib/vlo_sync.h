#ifndef _VLO_SYNC_H_
#define _VLO_SYNC_H_

typedef int (*vlo_cb_t)(uint16_t freq);

void vlo_get_frequency(vlo_cb_t cb);

#endif
