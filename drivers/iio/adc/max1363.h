 /*
  * Copyright (C) 2008-2011 Jonathan Cameron
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  */

#include <linux/kernel.h>

#ifndef _MAX1363_H_
#define  _MAX1363_H_

#define MAX1363_MAX_CHANNELS 25
#define MAX1363_SETUP_BYTE(a) ((a) | 0x80)

/* There is a fair bit more defined here than currently
 * used, but the intention is to support everything these
 * chips do in the long run */

/* see data sheets */
/* max1363 and max1236, max1237, max1238, max1239 */
#define MAX1363_SETUP_AIN3_IS_AIN3_REF_IS_VDD	0x00
#define MAX1363_SETUP_AIN3_IS_REF_EXT_TO_REF	0x20
#define MAX1363_SETUP_AIN3_IS_AIN3_REF_IS_INT	0x40
#define MAX1363_SETUP_AIN3_IS_REF_REF_IS_INT	0x60
#define MAX1363_SETUP_POWER_UP_INT_REF		0x10
#define MAX1363_SETUP_POWER_DOWN_INT_REF	0x00

/* think about including max11600 etc - more settings */
#define MAX1363_SETUP_EXT_CLOCK			0x08
#define MAX1363_SETUP_INT_CLOCK			0x00
#define MAX1363_SETUP_UNIPOLAR			0x00
#define MAX1363_SETUP_BIPOLAR			0x04
#define MAX1363_SETUP_RESET			0x00
#define MAX1363_SETUP_NORESET			0x02
/* max1363 only - though don't care on others.
 * For now monitor modes are not implemented as the relevant
 * line is not connected on my test board.
 * The definitions are here as I intend to add this soon.
 */
#define MAX1363_SETUP_MONITOR_SETUP		0x01

/* Specific to the max1363 */
#define MAX1363_MON_RESET_CHAN(a) (1 << ((a) + 4))
#define MAX1363_MON_INT_ENABLE			0x01

/* defined for readability reasons */
/* All chips */
#define MAX1363_CONFIG_BYTE(a) ((a))

#define MAX1363_CONFIG_SE			0x01
#define MAX1363_CONFIG_DE			0x00
#define MAX1363_CONFIG_SCAN_TO_CS		0x00
#define MAX1363_CONFIG_SCAN_SINGLE_8		0x20
#define MAX1363_CONFIG_SCAN_MONITOR_MODE	0x40
#define MAX1363_CONFIG_SCAN_SINGLE_1		0x60
/* max123{6-9} only */
#define MAX1236_SCAN_MID_TO_CHANNEL		0x40

/* max1363 only - merely part of channel selects or don't care for others*/
#define MAX1363_CONFIG_EN_MON_MODE_READ 0x18

#define MAX1363_CHANNEL_SEL(a) ((a) << 1)

/* max1363 strictly 0x06 - but doesn't matter */
#define MAX1363_CHANNEL_SEL_MASK		0x1E
#define MAX1363_SCAN_MASK			0x60
#define MAX1363_SE_DE_MASK			0x01

/**
 * struct max1363_mode - scan mode information
 * @conf:	The corresponding value of the configuration register
 * @modemask:	Bit mask corresponding to channels enabled in this mode
 */
struct max1363_mode {
	int8_t		conf;
	DECLARE_BITMAP(modemask, MAX1363_MAX_CHANNELS);
};

/* This must be maintained along side the max1363_mode_table in max1363_core */
enum max1363_modes {
	/* Single read of a single channel */
	_s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7, _s8, _s9, _s10, _s11,
	/* Differential single read */
	d0m1, d2m3, d4m5, d6m7, d8m9, d10m11,
	d1m0, d3m2, d5m4, d7m6, d9m8, d11m10,
	/* Scan to channel and mid to channel where overlapping */
	s0to1, s0to2, s2to3, s0to3, s0to4, s0to5, s0to6,
	s6to7, s0to7, s6to8, s0to8, s6to9,
	s0to9, s6to10, s0to10, s6to11, s0to11,
	/* Differential scan to channel and mid to channel where overlapping */
	d0m1to2m3, d0m1to4m5, d0m1to6m7, d6m7to8m9,
	d0m1to8m9, d6m7to10m11, d0m1to10m11, d1m0to3m2,
	d1m0to5m4, d1m0to7m6, d7m6to9m8, d1m0to9m8,
	d7m6to11m10, d1m0to11m10,
};

struct iio_info;
struct iio_chan_spec;

/**
 * struct max1363_chip_info - chip specifc information
 * @name:		indentification string for chip
 * @bits:		accuracy of the adc in bits
 * @int_vref_mv:	the internal reference voltage
 * @info:		iio core function callbacks structure
 * @mode_list:		array of available scan modes
 * @num_modes:		the number of scan modes available
 * @default_mode:	the scan mode in which the chip starts up
 * @channels:		channel specification
 * @num_channels:	number of channels
 */
struct max1363_chip_info {
	const struct iio_info		*info;
	struct iio_chan_spec		*channels;
	int				num_channels;
	const enum max1363_modes	*mode_list;
	enum max1363_modes		default_mode;
	u16				int_vref_mv;
	u8				num_modes;
	u8				bits;
};

struct regulator;
struct i2c_client;

/**
 * struct max1363_state - driver instance specific data
 * @client:		i2c_client
 * @setupbyte:		cache of current device setup byte
 * @configbyte:		cache of current device config byte
 * @chip_info:		chip model specific constants, available modes etc
 * @current_mode:	the scan mode of this chip
 * @requestedmask:	a valid requested set of channels
 * @reg:		supply regulator
 */
struct max1363_state {
	struct i2c_client		*client;
	u8				setupbyte;
	u8				configbyte;
	const struct max1363_chip_info	*chip_info;
	const struct max1363_mode	*current_mode;
	u32				requestedmask;
	struct regulator		*reg;
};

#endif /* _MAX1363_H_ */
