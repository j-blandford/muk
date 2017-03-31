#include <stddef.h>
#include <std.hpp>
#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/timer.hpp>
#include <kernel/drivers/ata.hpp>

const uint16_t device_offsets[2][2] = {
	{ ATA_0_MASTER, ATA_0_SLAVE },
	{ ATA_1_MASTER, ATA_1_SLAVE }
};

bool ATA::wait(int mask, int waitForState) {
	uint8_t state;
	uint16_t base_offset = device_offsets[bus][drive];
	uint64_t time = 0;

	while(true) {
		state = inportb(base_offset + ATA_STATUS);

		if((state&mask) == waitForState) 
			return true;

		if(time > 10)
			return false;

		sleep(1);
		time++;
	}

	return false;
}

bool ATA::prepare(int command, size_t num_blocks, int offset) {
	uint16_t base_offset = device_offsets[bus][drive];

	int flags = ATA_F_ECC | ATA_F_LBA | ATA_F_512_SEC;
	if(drive == 1) flags |= ATA_F_SLAVE;

	flags |= (offset >> 24) & 0xFF;

	this->wait(ATA_STATUS_BSY | ATA_STATUS_RDY, ATA_STATUS_RDY);

	outportb(base_offset + ATA_CTRL, 0);
	outportb(base_offset + ATA_COUNT, num_blocks);
	outportb(base_offset + ATA_SECTOR, offset);
	outportb(base_offset + ATA_CYL_LO, (offset >> 8) & 0xFF);
	outportb(base_offset + ATA_CYL_HI, (offset >> 16) & 0xFF);
	outportb(base_offset + ATA_FDH, flags);
	outportb(base_offset + ATA_COMMAND, command);

	return true;  
}

bool ATA::read(uint16_t** buffer, size_t num_blocks, int offset) {
	uint16_t buffer_short[num_blocks * ATA_BLOCKSIZE] = {0};

	this->prepare(ATA_COMMAND_READ, num_blocks, offset);

	for(size_t block = 0; block < num_blocks; block++) {
		this->wait(ATA_STATUS_DRQ, ATA_STATUS_DRQ);

		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			uint16_t readValue = inports(device_offsets[bus][drive] + ATA_DATA);

			buffer_short[i + block*ATA_BLOCKSIZE] = readValue;
		}
	}

	*buffer = buffer_short;

	return true;
}

bool ATA::read(uint8_t** buffer, size_t num_blocks, int offset) {
	uint16_t buffer_short[num_blocks * ATA_BLOCKSIZE] = {0};
	uint8_t buffer_byte[num_blocks * ATA_BLOCKSIZE * 2] = {0};

	this->prepare(ATA_COMMAND_READ, num_blocks, offset);

	for(size_t block = 0; block < num_blocks; block++) {

		this->wait(ATA_STATUS_DRQ, ATA_STATUS_DRQ);

		uint16_t readValue;

		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			readValue = inports(device_offsets[bus][drive] + ATA_DATA);

			buffer_short[i+block*ATA_BLOCKSIZE] = readValue;
		}

		// Splits u16_t into 2*u8_t
		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			buffer_byte[i*2] = (int)(buffer_short[i]);
			buffer_byte[i*2+1] = (int)((buffer_short[i] >> 8) & 0xFF);
		}

		// for(int i = 0; i < 6; i++) {
		//     terminal_printf("%x ", (int)(buffer_short[i] & 0xFF));
		//     terminal_printf("%x ", (int)((buffer_short[i] >> 8) & 0xFF));
		// }

	}

	*buffer = buffer_byte;

	return true;
}
