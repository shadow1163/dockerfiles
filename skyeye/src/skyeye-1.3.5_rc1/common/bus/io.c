/*
    io.c - I/O read_write function for all the architecture.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "skyeye_config.h"
#include "skyeye_device.h"

/*
 *  7/17/2003     clean some routine.
 *                clean io_reset only to do special mach_io_reset function.
 *                wlm <wlm@student.dlut.edu.cn>
 */

/* some machine need less number. e.g. s3c2440 only need DIVISOR = 1 to boot*/
#define DIVISOR      (50)
static int prescale = DIVISOR;

void
io_reset (void * state)
{
	struct device_desc *dev;
	int i;
	skyeye_config_t* config = get_current_config();
	assert(config->mach != NULL);
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (dev->reset)
			dev->reset (dev);
	}
	if(config->mach->mach_io_reset)
		config->mach->mach_io_reset (state);
	else
		fprintf(stderr,"SKYEYE_ERR:mach_io_reset is NULL\n");
}

void
io_do_cycle (void * state)
{
	struct device_desc *dev;
	int i;
#if 0
#ifdef DBCT_TEST_SPEED
	state->instr_count++;
#endif	//DBCT_TEST_SPEED
#endif
	skyeye_config_t* config = get_current_config();
	prescale--;
	if (prescale < 0) {
		prescale = DIVISOR;
		for (i = 0; i < config->mach->dev_count; i++) {
			dev = config->mach->devices[i];
			if (dev->update)
				dev->update (dev);
		}
		if(config->mach->mach_io_do_cycle != NULL)
			config->mach->mach_io_do_cycle (state);
	}
}

static uint32_t
io_read_byte (uint32_t addr)
{
	struct device_desc *dev;
	uint32_t data;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->read_byte)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {

			if (dev->read_byte (dev, addr, (uint8 *) & data) !=
			    ADDR_NOHIT)
				return data & 0xff;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {

			if (dev->read_byte (dev, addr, (uint8 *) & data) !=
			    ADDR_NOHIT)
				return data & 0xff;
		}
	}
	return config->mach->mach_io_read_byte (NULL, addr);
}

static uint32_t
io_read_halfword (uint32_t addr)
{
	struct device_desc *dev;
	uint32_t data;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->read_halfword)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {
			if (dev->read_halfword (dev, addr, (uint16 *) & data) !=
			    ADDR_NOHIT)
				return data & 0xffff;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {
			if (dev->read_halfword (dev, addr, (uint16 *) & data) !=
			    ADDR_NOHIT)
				return data & 0xffff;
		}
	}
	return config->mach->mach_io_read_halfword (NULL, addr);
}

static uint32_t
io_read_word (uint32_t addr)
{
	struct device_desc *dev;
	uint32_t data;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->read_word)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {
			if (dev->read_word (dev, addr, (uint32 *) & data) !=
			    ADDR_NOHIT)
				return data;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {
			if (dev->read_word (dev, addr, (uint32 *) & data) !=
			    ADDR_NOHIT)
				return data;
		}
	}
	return config->mach->mach_io_read_word (NULL, addr);
}

static void
io_write_byte (uint32_t addr, uint32_t data)
{
	struct device_desc *dev;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->write_byte)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {
			if (dev->write_byte (dev, addr, (uint8) data) !=
			    ADDR_NOHIT)
				return;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {
			if (dev->write_byte (dev, addr, (uint8) data) !=
			    ADDR_NOHIT)
				return;
		}
	}
	config->mach->mach_io_write_byte (NULL, addr, data);
}

static void
io_write_halfword (uint32_t addr, uint32_t data)
{
	struct device_desc *dev;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->write_halfword)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {
			if (dev->write_halfword (dev, addr, (uint16) data) !=
			    ADDR_NOHIT)
				return;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {
			if (dev->write_halfword (dev, addr, (uint16) data) !=
			    ADDR_NOHIT)
				return;
		}
	}
	config->mach->mach_io_write_halfword (NULL, addr, data);
}

static void
io_write_word ( uint32_t addr, uint32_t data)
{
	struct device_desc *dev;
	int i;
	skyeye_config_t* config = get_current_config();
	for (i = 0; i < config->mach->dev_count; i++) {
		dev = config->mach->devices[i];
		if (!dev->write_word)
			continue;
		/* if we specify size=0, we don't check 
		 * whether "addr" is in the range of address space of device.
		 * */
		if (dev->size == 0) {
			if (dev->write_word (dev, addr, data) != ADDR_NOHIT)
				return;
		}
		else if ((addr >= dev->base)
			 && (addr < (dev->base + dev->size))) {
			if (dev->write_word (dev, addr, data) != ADDR_NOHIT)
				return;
		}
	}
	config->mach->mach_io_write_word (NULL, addr, data);
}
int io_read(short size, uint32_t addr, uint32_t * value){
	void * state;
	switch(size){
		case 8:
			*value = io_read_byte (addr);
			break;
		case 16:
			*value = io_read_halfword(addr);
			break;
		case 32:
			*value = io_read_word(addr);
			break;
		default:
			return -1;
	}
	return 0;
}

int io_write(short size, uint32_t addr, uint32_t value){
	void * state;
	switch(size){
		case 8:
                        io_write_byte (addr, value);
                        break;
                case 16:
                        io_write_halfword(addr, value);
                        break;
                case 32:
                        io_write_word(addr, value);
                        break;
                default:
                        return -1;

	}
	return 0;
}
