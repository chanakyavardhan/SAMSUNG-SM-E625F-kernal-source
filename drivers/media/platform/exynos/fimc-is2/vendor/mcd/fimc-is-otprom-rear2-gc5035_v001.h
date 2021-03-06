#ifndef FIMC_IS_OTPROM_REAR2_GC5035_V001_H
#define FIMC_IS_OTPROM_REAR2_GC5035_V001_H

/*
 * Reference File
 * M62_Rear_Cal_Map_V022_20201105_for_LSI.xlsx
 */

#define GC5035_OTP_PAGE_ADDR                         0xfe
#define GC5035_OTP_MODE_ADDR                         0xf3
#define GC5035_OTP_BUSY_ADDR                         0x6f
#define GC5035_OTP_PAGE                              0x02
#define GC5035_OTP_ACCESS_ADDR_HIGH                  0x69
#define GC5035_OTP_ACCESS_ADDR_LOW                   0x6a
#define GC5035_OTP_READ_ADDR                         0x6c

#define GC5035_BANK_SELECT_ADDR                      0x1000
#define GC5035_OTP_START_ADDR_BANK1                  0x1080
#define GC5035_OTP_START_ADDR_BANK2                  0x1480
#define GC5035_OTP_START_ADDR_BANK3                  0x1880
#define GC5035_OTP_START_ADDR_BANK4                  0x1C80
#define GC5035_OTP_USED_CAL_SIZE                     ((0x13FF - 0x1080 + 0x1) / 8)

#define FIMC_IS_REAR2_MAX_CAL_SIZE                   (8 * 1024)
#define REAR2_HEADER_CHECKSUM_LEN                    ((0x13DF - 0x1080 + 0x1) / 8)

static const u32 sensor_mode_read_initial_setting_gc5035[] = {
	0xfa, 0x10, 0x0,
	0xf5, 0xe9, 0x0,
	0xfe, 0x02, 0x0,
	0x67, 0xc0, 0x0,
	0x59, 0x3f, 0x0,
	0x55, 0x80, 0x0,
	0x65, 0x80, 0x0,
	0x66, 0x03, 0x0,
};

static const u32 sensor_global_gc5035[] = {
	0xfc, 0x01, 0x0,
	0xf4, 0x40, 0x0,
	0xf5, 0xe9, 0x0,
	0xf6, 0x14, 0x0,
	0xf8, 0x44, 0x0,
	0xf9, 0x82, 0x0,
	0xfa, 0x00, 0x0,
	0xfc, 0x81, 0x0,
	0xfe, 0x00, 0x0,
	0x36, 0x01, 0x0,
	0xd3, 0x87, 0x0,
	0x36, 0x00, 0x0,
	0x33, 0x00, 0x0,
	0xfe, 0x03, 0x0,
	0x01, 0xe7, 0x0,
	0xf7, 0x01, 0x0,
	0xfc, 0x8f, 0x0,
	0xfc, 0x8f, 0x0,
	0xfc, 0x8e, 0x0,
	0xfe, 0x00, 0x0,
	0xee, 0x30, 0x0,
	0x87, 0x18, 0x0,
	0xfe, 0x01, 0x0,
	0x8c, 0x90, 0x0,
	0xfe, 0x00, 0x0,
};

static const u32 sensor_mode_read_initial_setting_gc5035_size =
	sizeof(sensor_mode_read_initial_setting_gc5035) / sizeof( sensor_mode_read_initial_setting_gc5035[0]);

static const u32 sensor_global_gc5035_size =
	sizeof(sensor_global_gc5035) / sizeof( sensor_global_gc5035[0]);
#endif /* FIMC_IS_OTPROM_REAR2_GC5035_V001_H */
