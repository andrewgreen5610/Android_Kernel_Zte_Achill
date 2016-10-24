/* Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef DIAGUSB_H
#define DIAGUSB_H

#ifdef CONFIG_DIAG_OVER_USB
#include <linux/usb/usbdiag.h>
#endif
#include "diagchar.h"
#include "diag_mux.h"

#define DIAG_USB_LOCAL		0
#define DIAG_USB_LOCAL_LAST	1
#define DIAG_USB_BRIDGE_BASE	DIAG_USB_LOCAL_LAST
#define DIAG_USB_MDM		(DIAG_USB_BRIDGE_BASE)
#define DIAG_USB_MDM2		(DIAG_USB_BRIDGE_BASE + 1)
#define DIAG_USB_QSC		(DIAG_USB_BRIDGE_BASE + 2)
#define DIAG_USB_BRIDGE_LAST	(DIAG_USB_BRIDGE_BASE + 3)

#ifndef CONFIG_DIAGFWD_BRIDGE_CODE
#define NUM_DIAG_USB_DEV	DIAG_USB_LOCAL_LAST
#else
#define NUM_DIAG_USB_DEV	DIAG_USB_BRIDGE_LAST
#endif

#define DIAG_USB_NAME_SZ	24
#define DIAG_USB_GET_NAME(x)	(diag_usb[x].name)

#define DIAG_USB_MODE		0

/*begin-4 -TF project, encrypt the DiagPort -zhenghuan 2014/12/23*/
/*choose macro
    ZTE_FEATURE_TF_SECURITY_SYSTEM_HIGH
    ZTE_FEATURE_TF_SECURITY_SYSTEM */
#ifdef ZTE_FEATURE_TF_SECURITY_SYSTEM
#define DIAG_LOCK_ON
#endif

#ifdef ZTE_FEATURE_TF_SECURITY_SYSTEM
int is_diag_locked(void);
#endif
/*end-4 -TF project, encrypt the DiagPort -zhenghuan 2014/12/23*/

struct diag_usb_info {
	int id;
	int ctxt;
	char name[DIAG_USB_NAME_SZ];
	int connected;
	int enabled;
	int mempool;
	unsigned long read_cnt;
	unsigned long write_cnt;
	spinlock_t lock;
	struct usb_diag_ch *hdl;
	struct diag_mux_ops *ops;
	int read_pending;
	unsigned char *read_buf;
	struct diag_request *read_ptr;
	struct work_struct read_work;
	struct work_struct read_done_work;
	struct work_struct connect_work;
	struct work_struct disconnect_work;
	struct workqueue_struct *usb_wq;
};

#ifdef CONFIG_DIAG_OVER_USB
extern struct diag_usb_info diag_usb[NUM_DIAG_USB_DEV];
int diag_usb_register(int id, int ctxt, struct diag_mux_ops *ops);
int diag_usb_queue_read(int id);
int diag_usb_write(int id, unsigned char *buf, int len, int ctxt);
void diag_usb_connect_all(void);
void diag_usb_disconnect_all(void);
void diag_usb_exit(int id);
#else
int diag_usb_register(int id, int ctxt, struct diag_mux_ops *ops)
{
	return 0;
}
int diag_usb_queue_read(int id)
{
	return 0;
}
int diag_usb_write(int id, unsigned char *buf, int len, int ctxt)
{
	return 0;
}
void diag_usb_connect_all(void)
{
	return;
}
void diag_usb_disconnect_all(void)
{
	return;
}
void diag_usb_exit(int id)
{
	return;
}
#endif

#endif
