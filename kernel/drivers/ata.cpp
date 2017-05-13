#include <kernel/drivers/ata.hpp>

#include <kernel/cpu.hpp>
#include <kernel/tty.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/scheduler.hpp>

const uint16_t device_offsets[2][2] = {
	{ ATA_0_MASTER, ATA_0_SLAVE },
	{ ATA_1_MASTER, ATA_1_SLAVE }
};

void ATA::initialise() {
	uint16_t base_offset = device_offsets[bus][drive];
	int flags = ATA_F_ECC | ATA_F_LBA | ATA_F_512_SEC;
	
	if(drive == 1) 
		flags |= ATA_F_SLAVE;

	uint8_t connected = inportb(base_offset + ATA_STATUS);
	if(connected == 0xFF) {
		// this device isn't connected/configured
		// somehow we need to escape and error
	} 
	else {
		// We have found an attached ATA device on this bus, let's grab some info on it :)
		this->reset();

		// send command ATA_COMMAND_IDENTIFY to the device to grab some of the information
		this->wait(ATA_STATUS_BSY, 0);

		outportb(base_offset + ATA_CTRL, 0);
		outportb(base_offset + ATA_COUNT, 0);
		outportb(base_offset + ATA_SECTOR, 0);
		outportb(base_offset + ATA_CYL_LO, 0);
		outportb(base_offset + ATA_CYL_HI, 0);
		outportb(base_offset + ATA_FDH, flags);
		outportb(base_offset + ATA_COMMAND, ATA_COMMAND_IDENTIFY);

		this->wait(ATA_STATUS_DRQ, ATA_STATUS_DRQ);

		uint16_t* buffer = this->readPIO(512);
	}
}

uint16_t* ATA::readPIO(int size) {
	Scheduler::lock();

	uint16_t* buffer = new uint16_t[size];
	
	for(int i = 0; i < size/2; i++) {
		buffer[i] = inports(device_offsets[bus][drive] + ATA_DATA);

		//sleep(1);
		//bcprintf("buffer[%d]=%x\n", i, buffer[i]);
		//MAGIC_BREAK;
	}

	// if((int)(buffer[27] >> 8) != 0)
 	// 	terminal_printf("[ATA] Device (%d,%d): ", bus, drive);

	// for(int i = 27; i < 47; i++) {
	// 	// uint16_t is actually TWO char's long!
	// 	terminal_printf("%c", (int)(buffer[i] >> 8));
	// 	terminal_printf("%c", (int)(buffer[i] & 0xFF));
	// }

	Scheduler::unlock();

	MAGIC_BREAK;

	return buffer;
}

// uint16_t * ATA::readPIO(int size) {
// 	uint16_t buffer[size/2] = {0};

// 	uint16_t buffer_byte[size] = {0};

// 	for(int i = 0; i < size; i++) {
// 		buffer_byte[i] = inports(device_offsets[bus][drive] + ATA_DATA);
// 	}

	
	
// 				bcprintf("CONNECTED!\n");
// 			MAGIC_BREAK;
	


// 	if((int)(buffer_byte[27] >> 8) != 0)
// 		terminal_printf("[ATA] Device (%d,%d): ", bus, drive);

// 	for(int i = 27; i < 47; i++) {
// 		// uint16_t is actually TWO char's long!
// 		terminal_printf("%c", (int)(buffer_byte[i] >> 8));
// 		terminal_printf("%c", (int)(buffer_byte[i] & 0xFF));
// 	}

// 	terminal_writestring("\n");
// 	return buffer;
// }

void ATA::writePIO(uint16_t * buffer, int size) {
	for(int i = size; i > 0; i-=2) {
		outports(device_offsets[bus][drive] + ATA_DATA, *buffer);
		*buffer++;
	}
}

void ATA::reset() {
	// allows us to send more commands after the ATA_STATUS command
	uint16_t base_offset = device_offsets[bus][drive];

	outportb(base_offset + ATA_CTRL, ATA_CTRL_RST);
	outportb(base_offset + ATA_CTRL, 0);
}

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

		sleep(10);
		time++;
	}

	return false;
}

bool ATA::prepare(int command, size_t num_blocks, int offset) {
	uint16_t base_offset = device_offsets[bus][drive];
	int flags = ATA_F_ECC | ATA_F_LBA | ATA_F_512_SEC;

	if(drive == 1) 
		flags |= ATA_F_SLAVE;

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

uint16_t* ATA::read_u16(size_t num_blocks, int offset) {
	uint16_t* buffer = new uint16_t[num_blocks * ATA_BLOCKSIZE / 2];

	this->prepare(ATA_COMMAND_READ, num_blocks, offset); // give the device a heads up

	for(size_t block = 0; block < num_blocks; block++) {
		this->wait(ATA_STATUS_DRQ, ATA_STATUS_DRQ); // wait for it to be ready (per block)

		size_t block_offset = block*ATA_BLOCKSIZE / 2;

		for(int i = 0; i < ATA_BLOCKSIZE / 2; i++) {
			buffer[block_offset + i] = inports(device_offsets[bus][drive] + ATA_DATA);
		}
	}

	return buffer;
}

uint8_t* ATA::read_u8(size_t num_blocks, int offset) {
	uint8_t* buffer = new uint8_t[num_blocks * ATA_BLOCKSIZE];
	uint16_t buffer_short[num_blocks * ATA_BLOCKSIZE / 2] = {0}; // temporary storage of shorts
	
	this->prepare(ATA_COMMAND_READ, num_blocks, offset); // give the device a heads up

	for(size_t block = 0; block < num_blocks; block++) {
		this->wait(ATA_STATUS_DRQ, ATA_STATUS_DRQ); // wait for it to be ready (per block)

		size_t block_offset = block*ATA_BLOCKSIZE;

		for(int i = 0; i < ATA_BLOCKSIZE / 2; i++) {
			buffer_short[block_offset + i] = inports(device_offsets[bus][drive] + ATA_DATA);
		}

		// Splits u16_t into 2*u8_t
		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			buffer[i*2] = (int)(buffer_short[i]);
			buffer[i*2 + 1] = (int)((buffer_short[i] >> 8) & 0xFF);
		}
	}

	return buffer;
}
