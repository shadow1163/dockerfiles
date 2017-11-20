#ifndef __MEMORY_RAM_H__
#define __MEOMRY_RAM_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef DBCT
#include "tb.h"
#endif

#define DRAM_BITS       (23)    /* 8MB of DRAM */
#define ROM_BANKS       16
#define ROM_BITS        (28)    /* 0x10000000 each bank */

typedef struct mem_state_t
{
        uint32_t *dram;
	/* The pointer on the host */
        uint32_t *rom[ROM_BANKS];
        unsigned int rom_size[ROM_BANKS];
#ifdef DBCT
//teawater add for arm2x86 2004.12.04-------------------------------------------
        uint8_t *tbp[ROM_BANKS];        //translate block pointer
        struct tb_s *tbt[ROM_BANKS];    //translate block structure pointer
//AJ2D--------------------------------------------------------------------------
#endif

} mem_state_t;
#if 0
uint32_t mem_read_byte(void * state, uint32_t addr);
uint32_t mem_read_halfword(void * state, uint32_t addr);
uint32_t mem_read_word(void* state, uint32_t addr);
void mem_write_byte(void * state, uint32_t addr, uint32_t data);
void mem_write_halfword(void * state, uint32_t addr, uint32_t data);
void mem_write_word(void * state, uint32_t addr, uint32_t data);
#endif

int mem_read(short size, int offset, uint32_t * value);
int mem_write(short size, int offset, uint32_t value);
int warn_write(short size, int offset, uint32_t value);
unsigned long get_dma_addr(unsigned long guest_addr);
mem_state_t * get_global_memory();

#ifdef __cplusplus
}
#endif

#endif
