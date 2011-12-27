/*
 * Copyright (c) 2011 - Mauro Carvalho Chehab <mchehab@redhat.com>
 */

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "dvb_frontend.h"

#define ARRAY_SIZE(x)	(sizeof(x)/sizeof((x)[0]))

#define MAX_DELIVERY_SYSTEMS	20

/*
 * Implement the stats properties as if they were defined via S2API style.
 * This simplifies the addition of newer stats, and helps to port the
 * code to v5 style, if such change gets merged upstream.
 */

#define DTV_MAX_STATS 5
#define DTV_STATUS			(DTV_MAX_COMMAND + 100)
#define DTV_BER				(DTV_MAX_COMMAND + 101)
#define DTV_SIGNAL_STRENGTH		(DTV_MAX_COMMAND + 102)
#define DTV_SNR				(DTV_MAX_COMMAND + 103)
#define DTV_UNCORRECTED_BLOCKS		(DTV_MAX_COMMAND + 104)

struct dvb_v5_stats {
	struct dtv_property		prop[DTV_MAX_STATS];
};

struct dvb_v5_fe_parms {
	int				fd;
	char				*fname;
	unsigned			verbose;
	struct dvb_frontend_info	info;
	uint32_t			version;
	fe_delivery_system_t		current_sys;
	int				num_systems;
	fe_delivery_system_t		systems[MAX_DELIVERY_SYSTEMS];
	int				n_props;
	struct dtv_property		dvb_prop[DTV_MAX_COMMAND];
	int				legacy_fe;
	struct dvb_v5_stats		stats;
};

/* Open/close methods */

struct dvb_v5_fe_parms *dvb_fe_open(int adapter, int frontend,
				    unsigned verbose, unsigned use_legacy_call);
void dvb_fe_close(struct dvb_v5_fe_parms *parms);

/* Get/set delivery system parameters */

int dvb_fe_retrieve_parm(struct dvb_v5_fe_parms *parms,
			unsigned cmd, uint32_t *value);
int dvb_fe_store_parm(struct dvb_v5_fe_parms *parms,
		      unsigned cmd, uint32_t value);
int dvb_set_sys(struct dvb_v5_fe_parms *parms,
		   fe_delivery_system_t sys);
void dvb_fe_prt_parms(struct dvb_v5_fe_parms *parms);
int dvb_fe_set_parms(struct dvb_v5_fe_parms *parms);

/* Get statistics */

int dvb_fe_retrieve_stats(struct dvb_v5_fe_parms *parms,
			   unsigned cmd, uint32_t *value);
int dvb_fe_store_stats(struct dvb_v5_fe_parms *parms,
			unsigned cmd, uint32_t value);
int dvb_fe_get_stats(struct dvb_v5_fe_parms *parms);

/* Get both status statistics and dvb parameters */

int dvb_fe_get_event(struct dvb_v5_fe_parms *parms);

/* Other functions, associated to SEC/LNB/DISEqC */

/*
 * NOTE: It currently lacks support for two ioctl's:
 * FE_DISEQC_RESET_OVERLOAD	used only on av7110.
 * Spec says:
 *   If the bus has been automatically powered off due to power overload,
 *   this ioctl call restores the power to the bus. The call requires read/write
 *   access to the device. This call has no effect if the device is manually
 *   powered off. Not all DVB adapters support this ioctl.
 *
 * FE_DISHNETWORK_SEND_LEGACY_CMD is used on av7110, budget, gp8psk and stv0299
 * Spec says:
 *   WARNING: This is a very obscure legacy command, used only at stv0299
 *   driver. Should not be used on newer drivers.
 *   It provides a non-standard method for selecting Diseqc voltage on the
 *   frontend, for Dish Network legacy switches.
 *   As support for this ioctl were added in 2004, this means that such dishes
 *   were already legacy in 2004.
 *
 * So, it doesn't make much sense on implementing support for them.
 */

int dvb_fe_sec_voltage(struct dvb_v5_fe_parms *parms, int on, int v18);
int dvb_fe_sec_tone(struct dvb_v5_fe_parms *parms, int on);
int dvb_fe_lnb_high_voltage(struct dvb_v5_fe_parms *parms, int on);
int dvb_fe_diseqc_burst(struct dvb_v5_fe_parms *parms, int mini_a);
int dvb_fe_diseqc_cmd(struct dvb_v5_fe_parms *parms, unsigned len, char *buf);
int dvb_fe_diseqc_reply(struct dvb_v5_fe_parms *parms, unsigned *len, char *buf,
		       int timeout);
