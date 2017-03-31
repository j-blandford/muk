#pragma once

#include <stddef.h>
#include <stdint.h>

#include <std.hpp>


#define ATA_BLOCKSIZE 512

// In/output offsets
#define ATA_0_MASTER	0x1F0
#define ATA_0_SLAVE	    0x170
#define ATA_1_MASTER	0x1E8
#define ATA_1_SLAVE     0x168

#define ATA_CTRL				0x206
#define ATA_CTRL_RST			0x04
#define ATA_CTRL_DISABLE_INT	0x02

// ATA registers
#define ATA_DATA	0   /* data register */
#define ATA_ERROR	1   /* error register */
#define ATA_COUNT	2   /* sectors to transfer */
#define ATA_SECTOR	3   /* sector number */
#define ATA_CYL_LO	4   /* low byte of cylinder number */
#define ATA_CYL_HI	5   /* high byte of cylinder number */
#define ATA_FDH		6   /* flags, drive and head */
#define ATA_STATUS	7
#define ATA_COMMAND	7

// ATA Flags when sending data
#define ATA_F_SLAVE		0x10        // slave drive
#define ATA_F_512_SEC	0x20	// 512byte sectors
#define ATA_F_LBA		0x40        // linear addressing
#define ATA_F_ECC		0x80		// error correcting

// ATA Status codes
#define ATA_STATUS_DRQ	0x08    // request data
#define ATA_STATUS_BSY	0x80    //busy
#define ATA_STATUS_RDY	0x40    // ready for commands

#define ATA_COMMAND_IDLE		0x00
#define ATA_COMMAND_READ		0x20    /* read data */
#define ATA_COMMAND_WRITE		0x30    /* write data */
#define ATA_COMMAND_IDENTIFY	0xec

class ATA {
	int bus;
	int drive;
public: 

	ATA(int bus, int drive) : bus(bus), drive(drive) { }
	~ATA() { }

	// programmed i/o mode (all ATA devices support this)
	// static uint16_t * readPIO(int bus, int drive, int size);
	// static void writePIO(int bus, int drive, uint16_t * buffer, int size);
	// static bool initializeDevice(int bus, int drive);
	// static void resetATA(int bus, int drive);

	bool prepare(int command, size_t num_blocks, int offset); // prepares the device for a command's data packet
	bool read(uint16_t** buffer, size_t num_blocks, int offset);
	bool read(uint8_t** buffer, size_t num_blocks, int offset);
	bool wait(int mask, int waitForState);

	// static void findATA();
	// static std::vector<Filesystems::DirectoryEntry> getDirectory(int deviceIndex, size_t sectorIndex);
	// static std::vector<Filesystems::DirectoryEntry> getDirectoryPath(int deviceIndex, char * path);
	// // todo: ATAPI
	// // todo: SCSI (maybe...)
};