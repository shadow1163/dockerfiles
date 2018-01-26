/*
        loader_file.c - load a file to memory. 
        Copyright (C) 2003-2007 Skyeye Develop Group
        for help please send mail to <skyeye-developer@lists.sf.linuxforum.net>

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
/*
 * 06/22/2009   Michael.Kang  <blackfin.kang@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "skyeye_log.h"
#include "skyeye_loader.h"
#include "bank_defs.h"
#include "skyeye_ram.h"

/**
* @brief load a block of data to the memory
*
* @param src
* @param len
* @param load_addr
*
* @return 
*/
exception_t load_data(void* src, size_t len, generic_address_t load_addr){
	char* p = src;
	generic_address_t addr = load_addr;
	int i = 0;
	while(i < len){
		//skyeye_log(Debug_log, __FUNCTION__, "data=0x%x,addr=0x%x\n", *(p + i), addr);
		bus_write(8, addr, *(p + i));
		addr++;
		i++;
	}
	return No_exp;
}

/**
* @brief Load a file to guest memory
*
* @param filename
* @param load_addr
*
* @return 
*/
exception_t load_file(const char* filename, generic_address_t load_addr){
	FILE* f;
	uint8 data;
	int nread = 0;
	generic_address_t addr = load_addr;
	unsigned long host_addr = get_dma_addr(addr);
	skyeye_log(Debug_log, __FUNCTION__, "addr=0x%x, host_addr=0x%lx.\n", addr, (unsigned long) host_addr);
	struct stat stat_buf;
	if(stat(filename, &stat_buf) != 0){
		skyeye_log(Error_log, __FUNCTION__, "Can not stat file %s.\n", filename);
		return File_open_exp;
	}
	off_t size = stat_buf.st_size;
	if(size >= (get_bank_size(addr) - addr)){
		skyeye_log(Error_log, __FUNCTION__, "File %s too big.\n", filename);
		return Excess_range_exp;
	}
	f = fopen(filename, "rb");
	if(f == NULL){
		skyeye_log(Error_log, __FUNCTION__, "Can not open file %s.\n", filename);
		return File_open_exp;
	}

	nread = fread((uint8_t *)host_addr, 1, size, f);
	if(nread != size){
		skyeye_log(Error_log, __FUNCTION__, "Can not stat file %s.\n", filename);
		fclose(f);
		return File_open_exp;
	}
	#if 0
	/* read a char and write it to the memory */
	while(nread = fread(&data, 1, 1, f)){
		#if 0
		if(bus_write(8, addr, data) != 0){
			/* error handler for address error */
			fclose(f);
			return Excess_range_exp;
		}
		#endif
		*(uint8_t *)host_addr = data;
		host_addr++;
		addr++;
	}
	#endif
	skyeye_log(Info_log, __FUNCTION__, "Load the file %s to the memory 0x%x\n", filename, load_addr);
	fclose(f);
#if 0 
		if (mb[bank].filename
		    && (f = fopen (mb[bank].filename, "rb"))) {
			if (fread
			    (global_memory.rom[bank], 1, mb[bank].len,
			     f) <= 0) {
				perror ("fread");
				fprintf (stderr,
					 "Failed to load '%s'\n",
					 mb[bank].filename);
				skyeye_exit (-1);
			}
			fclose (f);

			p = (char *) global_memory.rom[bank];
			s = 0;
			while (s < global_memory.rom_size[bank]) {
				generic_arch_t* arch_instance = get_arch_instance(NULL);
				if (arch_instance->endianess == Big_endian)	/*big enddian? */
					swap = ((uint32_t) p[3]) |
						(((uint32_t) p[2]) <<
						 8) | (((uint32_t)
							p[1]) << 16) |
						(((uint32_t) p[0]) <<
						 24);
				else
					swap = ((uint32_t) p[0]) |
						(((uint32_t) p[1]) <<
						 8) | (((uint32_t)
							p[2]) << 16) |
						(((uint32_t) p[3]) <<
						 24);
				*(uint32_t *) p = swap;
				p += 4;
				s += 4;
			}

				/*ywc 2004-03-30 */
				//printf("Loaded ROM %s\n", mb[bank].filename);
			if (mb[bank].type == MEMTYPE_FLASH) {
				printf ("Loaded FLASH %s\n",
					mb[bank].filename);
			}
			else if (mb[bank].type == MEMTYPE_RAM) {
				printf ("Loaded RAM   %s\n",
					mb[bank].filename);
			}
			else if (mb[bank].type == MEMTYPE_ROM) {
				printf ("Loaded ROM   %s\n",
					mb[bank].filename);
			}

		}
		else if (mb[bank].filename[0] != '\0') {
			perror (mb[bank].filename);
			fprintf (stderr,
				 "bank %d, Couldn't open boot ROM %s - execution will "
				 "commence with the debuger.\n", bank,
				 mb[bank].filename);
		}
#endif

	return No_exp;
}
