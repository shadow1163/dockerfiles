/*
        skyeye_dyncom.h - dynamic compiled function header file
        Copyright (C) 2010 Skyeye Develop Group
        for help please send mail to <skyeye-developer@lists.gro.clinux.org>

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
 * Ported from libcpu source tree.
 *
 * 08/22/2010   Michael.Kang  <blackfin.kang@gmail.com>
 */

#ifndef __SKYEYE_DYNCOM_H__
#define __SKYEYE_DYNCOM_H__

#include "config.h"
#include "skyeye_platform.h"
#include "skyeye_types.h"
#include "dyncom/defines.h"
#include "portable/portable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <map>
#include <vector>

#include <skyeye_log.h>
#include <skyeye_mm.h>

namespace llvm {
	class BasicBlock;
	class ExecutionEngine;
	class Function;
	class Module;
	class PointerType;
	class StructType;
	class Value;
}

#include "dyncom_types.h"
#include "dyncom/fp_types.h"

using namespace llvm;
using namespace std;

typedef struct cpu cpu_t;

typedef void        (*fp_init)(struct cpu *cpu, struct cpu_archinfo *info, struct cpu_archrf *rf);
typedef void        (*fp_done)(struct cpu *cpu);
// @@@BEGIN_DEPRECATION
typedef StructType *(*fp_get_struct_reg)(struct cpu *cpu);
typedef addr_t      (*fp_get_pc)(struct cpu *cpu, void *regs);
typedef uint32      (*fp_get_instr_length)(struct cpu *cpu);
// @@@END_DEPRECATION
typedef void        (*fp_emit_decode_reg)(struct cpu *cpu, BasicBlock *bb);
typedef void        (*fp_spill_reg_state)(struct cpu *cpu, BasicBlock *bb);
typedef int         (*fp_tag_instr)(struct cpu *cpu, addr_t pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc);
typedef int         (*fp_disasm_instr)(struct cpu *cpu, addr_t pc, char *line, unsigned int max_line);
typedef Value      *(*fp_translate_cond)(struct cpu *cpu, addr_t pc, BasicBlock *bb);
typedef int         (*fp_translate_instr)(struct cpu *cpu, addr_t pc, BasicBlock *bb);
typedef int         (*fp_translate_loop_helper)(struct cpu *cpu, addr_t pc, BasicBlock *bb, BasicBlock *bb_ret, BasicBlock *bb_next, BasicBlock *bb_cond);
// @@@BEGIN_DEPRECATION
// idbg support
typedef uint64_t    (*fp_get_psr)(struct cpu *cpu, void *regs);
typedef int         (*fp_get_reg)(struct cpu *cpu, void *regs, unsigned reg_no, uint64_t *value);
typedef int         (*fp_get_fp_reg)(struct cpu *cpu, void *regs, unsigned reg_no, void *value);
// @@@END_DEPRECATION

typedef struct {
	fp_init init;
	fp_done done;
// @@@BEGIN_DEPRECATION
	fp_get_pc get_pc;
	fp_get_instr_length get_instr_length;
// @@@END_DEPRECATION
	fp_emit_decode_reg emit_decode_reg;
	fp_spill_reg_state spill_reg_state;
	fp_tag_instr tag_instr;
	fp_disasm_instr disasm_instr;
	fp_translate_cond translate_cond;
	fp_translate_instr translate_instr;
	fp_translate_loop_helper translate_loop_helper;
// @@@BEGIN_DEPRECATION
	// idbg support
	fp_get_psr get_psr;
	fp_get_reg get_reg;
	fp_get_fp_reg get_fp_reg;
// @@@END_DEPRECATION
} arch_func_t;

typedef bool_t			(*fp_is_inside_page)(cpu_t *cpu, addr_t addr);
typedef bool_t			(*fp_is_page_start)(cpu_t* cpu, addr_t addr);
typedef bool_t			(*fp_is_page_end)(cpu_t* cpu, addr_t addr);
typedef uint32_t 		(*fp_read_memory_t)(cpu_t *cpu, addr_t addr, uint32_t size, int ex_flag);
typedef void 			(*fp_write_memory_t)(cpu_t *cpu, addr_t addr, uint32_t value, uint32_t size, int ex_flag);
typedef uint32_t		(*fp_check_mm_t)(cpu_t *cpu, uint32_t addr, int count, uint32_t read_flag);
typedef int				(*fp_effective_to_physical)(struct cpu *cpu, uint32_t addr, uint32_t *result);
typedef struct {
	fp_is_inside_page is_inside_page;
	fp_is_page_start is_page_start;
	fp_is_page_end is_page_end;
	fp_read_memory_t read_memory;
	fp_write_memory_t write_memory;
	fp_check_mm_t	check_mm;
	fp_effective_to_physical effective_to_physical;
} arch_mem_ops_t;

enum {
	CPU_FLAG_ENDIAN_MASK   = (3 << 1),
	CPU_FLAG_ENDIAN_BIG    = (0 << 1),
	CPU_FLAG_ENDIAN_LITTLE = (1 << 1),
	CPU_FLAG_ENDIAN_MIDDLE = (2 << 1), // Middle endian (ie. PDP-11) NYI.
	CPU_FLAG_ENDIAN_NONE   = (3 << 1), // Mainly word oriented machines, where 
									   // word size is not a power of two.
									   // (ie. PDP-10)

	// @@@BEGIN_DEPRECATION
	CPU_FLAG_HARDWIRE_GPR0 = (1 << 4),
	CPU_FLAG_HARDWIRE_FPR0 = (1 << 4),
	// @@@END_DEPRECATION
	CPU_FLAG_DELAY_SLOT    = (1 << 5),
	CPU_FLAG_DELAY_NULLIFY = (1 << 6),

	// internal flags.
	CPU_FLAG_FP80          = (1 << 15), // FP80 is natively supported.
	CPU_FLAG_FP128         = (1 << 16), // FP128 is natively supported.
	CPU_FLAG_SWAPMEM       = (1 << 17), // Swap load/store
	/* if pc is saved in every instruction */
	CPU_FLAG_SAVE_PC	= (1 << 18),
	CPU_FLAG_SAVE_PC_BEFORE_EXEC	= (1 << 19)
};

// @@@BEGIN_DEPRECATION
// Four register classes
enum {
	CPU_REG_GPR, // General Purpose
	CPU_REG_FPR, // Floating Point
	CPU_REG_VR,  // Vector
	CPU_REG_XR,  // Extra Registers, the core expects these to follow
	 			 // GPRs in the memory layout, they are kept separate
				 // to avoid confusing the client about the number of
				 // registers available.
	CPU_REG_SPR,
	MAX_REG_TYPES
};
// @@@END_DEPRECATION

#define TIMER_COUNT	5
#define TIMER_TAG	0
#define TIMER_FE	1
#define TIMER_BE	2
#define TIMER_RUN	3
#define TIMER_OPT	4

// flags' types
enum {
	CPU_FLAGTYPE_NONE = 0,
	CPU_FLAGTYPE_CARRY = 'C',
	CPU_FLAGTYPE_OVERFLOW = 'V',
	CPU_FLAGTYPE_NEGATIVE = 'N',
	CPU_FLAGTYPE_PARITY = 'P',
	CPU_FLAGTYPE_ZERO = 'Z',
	CPU_FLAGTYPE_THUMB = 'T'
};

typedef struct cpu_flags_layout {
	void* flag_address;
	int shift;	/* bit position */
	char type;	/* 'N', 'V', 'Z', 'C' or 0 (some other flag unknown to the generic code) */
	const char *name; /* symbolic name */
} cpu_flags_layout_t;

// register layout types
enum {
	CPU_REGTYPE_INVALID = -1,
	CPU_REGTYPE_INT = 'i',
	CPU_REGTYPE_FLOAT = 'f',
	CPU_REGTYPE_VECTOR = 'v'
};

// register layout flags
enum {
	// special registers
	CPU_REGFLAG_SPECIAL_MASK = 0xf,
	CPU_REGFLAG_PC  = 1,
	CPU_REGFLAG_NPC = 2,
	CPU_REGFLAG_PSR = 3
};

typedef struct cpu_register_layout {
	char type;
	unsigned bits_size;
	unsigned aligned_size;
	unsigned byte_offset;
	unsigned flags;
	char const *name;
} cpu_register_layout_t;

typedef struct cpu_archinfo {
	char const *name;
	char const *full_name;

	uint32_t common_flags;
	uint32_t arch_flags;

	uint32_t delay_slots;

	uint32_t byte_size;
	uint32_t word_size;
	uint32_t float_size;
	uint32_t vector_size;
	uint32_t address_size;
	uint32_t psr_size;

	uint32_t min_page_size;
	uint32_t max_page_size;
	uint32_t default_page_size;

	// @@@BEGIN_DEPRECATION
	uint32_t register_count[MAX_REG_TYPES];
	uint32_t register_size[MAX_REG_TYPES];
	// @@@END_DEPRECATION

	cpu_register_layout_t const *register_layout;
	uint32_t register_count2;
	cpu_flags_layout_t *flags_layout;
	uint32_t flags_count;
	/* indicate the pc location in gpr array, only for arm now */
	int pc_index_in_gpr;
} cpu_archinfo_t;

typedef struct cpu_archrf {
	// @@@BEGIN_DEPRECATION
	void *pc;  // Program Counter
	void *phys_pc;  // Physical Program Counter
	void *context_id;  // process id of arm processor
	void *cpsr; // the state status of arm processor
	void *grf; // GP register file
	void *srf; // SP register file
	void *frf; // FP register file
	void *vrf; // Vector register file
	// @@@END_DEPRECATION
	void *storage;
} cpu_archrf_t;

/*
 * type of the debug callback; second parameter is
 * pointer to CPU specific register struct
 */
typedef uint32_t (*debug_function_t)(cpu_t*);

typedef void (*switch_mode_function_t)(cpu_t*);
/*
 * type of the syscall callback; second parameter is
 * pointer to CPU specific register struct
 */
typedef void (*syscall_function_t)(cpu_t*, uint32_t);
/*
 * Callout function types
 */
typedef void (*callout)(cpu_t *);
typedef void (*callout1)(cpu_t *, uint32_t);
typedef void (*callout2)(cpu_t *, uint32_t, uint32_t);
typedef void (*callout3)(cpu_t *, uint64_t, uint32_t, uint32_t);
typedef void (*callout4)(cpu_t *, uint32_t, uint32_t, uint32_t, uint32_t);


#define HASH_FAST_MAP
typedef std::map<addr_t, BasicBlock *> bbaddr_map;
typedef std::map<Function *, bbaddr_map> funcbb_map;
#ifdef HASH_FAST_MAP

#define L3_HASHMAP 1
#if L3_HASHMAP
#define HASH_MAP_SIZE_L1 1024
#define HASH_MAP_SIZE_L2 1024
#define HASH_MAP_SIZE_L3 4096

#define HASH_MAP_MASK_L1 0xffc00000
#define HASH_MAP_MASK_L2 0x003ff000
#define HASH_MAP_MASK_L3 0x00000fff

#define HASH_MAP_INDEX_L1(phys_pc) ((phys_pc & HASH_MAP_MASK_L1) >> 22)
#define HASH_MAP_INDEX_L2(phys_pc) ((phys_pc & HASH_MAP_MASK_L2) >> 12)
#define HASH_MAP_INDEX_L3(phys_pc) (phys_pc & HASH_MAP_MASK_L3)

typedef void**** fast_map;

static inline int init_fmap_l3(fast_map fmap, addr_t phys_pc)
{
	if(fmap[HASH_MAP_INDEX_L1(phys_pc)][HASH_MAP_INDEX_L2(phys_pc)] != NULL)
		return 1;
	fmap[HASH_MAP_INDEX_L1(phys_pc)][HASH_MAP_INDEX_L2(phys_pc)] =
		(void **)skyeye_mm_zero(sizeof(void **) * HASH_MAP_SIZE_L3 * 2);
	if(fmap[HASH_MAP_INDEX_L1(phys_pc)][HASH_MAP_INDEX_L2(phys_pc)] == NULL){
		skyeye_log(Error_log, __func__, "malloc error\n", __func__);
		exit(0);
	}
	return 1;
}
static inline int init_fmap_l2(fast_map fmap, addr_t phys_pc)
{
	if(fmap[HASH_MAP_INDEX_L1(phys_pc)] != NULL)
		return 1;
	fmap[HASH_MAP_INDEX_L1(phys_pc)] = (void ***)skyeye_mm_zero(sizeof(void ***) * HASH_MAP_SIZE_L2);
	if(fmap[HASH_MAP_INDEX_L1(phys_pc)] == NULL){
		skyeye_log(Error_log, __func__, "malloc error\n", __func__);
		exit(0);
	}
	return 1;
}
static inline void clear_fmap(fast_map fmap)
{
	int i, j, k;
	for(i = 0; i < HASH_MAP_SIZE_L1; i++){
		if(fmap[i] == NULL) continue;
		for(j = 0; j < HASH_MAP_SIZE_L2; j++){
			if(fmap[i][j] == NULL) continue;
			for(k = 0; k < HASH_MAP_SIZE_L3 * 2; k++)
				if(fmap[i][j][k] == NULL) continue;
				else fmap[i][j][k] = NULL;
		}
	}
}

inline void clear_cache_item(fast_map fmap, addr_t addr)
{
	//printf("In %s, addr=0x%x\n", __FUNCTION__, addr);
	for (int i = 0; i < HASH_MAP_SIZE_L3 * 2; i ++) {
		if(fmap[HASH_MAP_INDEX_L1(addr)] == NULL)
			init_fmap_l2(fmap, addr);
		if(fmap[HASH_MAP_INDEX_L1(addr)][HASH_MAP_INDEX_L2(addr)] == NULL)
			init_fmap_l3(fmap, addr);
		//if(fmap[HASH_MAP_INDEX_L1(addr)][HASH_MAP_INDEX_L2(addr)][HASH_MAP_INDEX_L3(addr)] == NULL)
		//	continue;
		fmap[HASH_MAP_INDEX_L1(addr)][HASH_MAP_INDEX_L2(addr)][i] = 0;
	}
}

#else
typedef void** fast_map;
//#define HASH_FAST_MAP_SIZE 0x100000
#define HASH_FAST_MAP_SIZE 0x20000000

#endif /* #if L3_HASHMAP */
#else
/* This map save <address, native code function pointer> */
typedef std::map<addr_t, void *> fast_map;
static inline int init_fmap_l3(fast_map fmap, addr_t phys_pc){}
static inline int init_fmap_l2(fast_map fmap, addr_t phys_pc){}
static inline void clear_fmap(fast_map fmap){}
#endif

typedef struct compiled_func{
	/* The compiled native code section */
	void* fp;
	/* The translate function of llvm */
	Function* func;
	/* The bb set by tagging procedure */
	vector<addr_t> startbb;
}compiled_func_t;
#define JIT_NUM 102400
typedef struct dyncom_engine{
	funcbb_map func_bb; // faster bb lookup

	/* for every JIT, we will save its startbb */
	vector<addr_t> startbb[JIT_NUM];
	int cur_tagging_pos;
	/* the lock for the compiled thread and running thread */
	pthread_rwlock_t rwlock;

	vector<addr_t> insns_in_jit;
	addr_t code_start;
	addr_t code_end;
	addr_t code1_start;
	addr_t code1_end;

	addr_t code_entry;
	addr_t tag_start;
	addr_t tag_end;

	uint32_t flags_codegen;
	uint32_t flags_debug;
	uint32_t flags_hint;
	uint32_t flags;
	uint8_t code_digest[20];
	FILE *file_entries;
	tag_t ***tag_table;
	tag_t *tag;
	bool tags_dirty;
	Module *mod;
	void *fp[JIT_NUM];
	uint8_t func_attr[JIT_NUM];
	uint32_t func_size[JIT_NUM];
	Function *func[JIT_NUM];
	Function *cur_func;
	fast_map fmap;
	uint32_t functions;
	ExecutionEngine *exec_engine;
	uint8_t *RAM;
	//unsigned long TLB;
//for four memory region
    tag_t *tag_array[4];
    uint32_t code_size[4];
//for 545CK
    uint8_t *system_ram;
    uint8_t *system_rom;
    uint8_t *data_ram;
    uint8_t *instr_ram;

	PointerType *type_pread_memory;
	PointerType *type_pwrite_memory;
	PointerType *type_check_mm;

	Value *ptr_RAM;
	Value *ptr_data_read_tlb;
	Value *ptr_data_write_tlb;
	Value *ptr_mixed_tlb;
	Value *ptr_io_tlb;

	Value *ptr_user_mode;
	Value *ptr_grf; // gpr register file
	Value *ptr_srf; // gpr register file
	Value *ptr_frf; // fp register file
	Value *ptr_func_read_memory;
	Value *ptr_func_write_memory;

	/* Temp variable for address translation */
	BasicBlock* bb_trap;
	BasicBlock* bb;
	/* Temp variable for arm write back decoder */
	Value* wb_value;
	int wb_flag;
	int need_exclusive;
	//int exclusive_result_reg;
	int bl_1_offset;
	addr_t bl_1_addr;

	Value* read_value;
	/* arch functions are for each architecture to declare it's own functions
	   which can be invoked in llvm IR.Usually,the functions to be invoked are
	   C functions which are complex to be implemented by llvm IR.
	   DEFAULT:
	   ptr_arch_func[0] is debug function.
	   ptr_arch_func[1] is syscall function for user mode simulation.
	   SO in your architecture, you can use callout function from 2 */
	#define MAX_ARCH_FUNC_NUM 10
	Value *ptr_arch_func[MAX_ARCH_FUNC_NUM];
	void *arch_func[MAX_ARCH_FUNC_NUM];
} dyncom_engine_t;

enum {
	CALLOUT_DEBUG,
	CALLOUT_SYSCALL,
	MAX_DYNCOM_CALLOUT
};

typedef struct cpu {
	cpu_archinfo_t info;
	cpu_archrf_t rf;
	conf_object_t* cpu_data;
	arch_func_t f;
	arch_mem_ops_t mem_ops;

	uint16_t pc_offset;

	/* icounter and old_icounter are used in kernel simulation, for
	 * we should use them to count the cycles to reponse the interrupt
	 * and exception. ANY more efficacious way? */
#define TIMEOUT_THRESHOLD 30
	uint64_t icounter;/* instruction counter */
	uint64_t old_icounter;/* old instruction counter */
	Value *ptr_ICOUNTER;
	Value *ptr_Nirq;

	uint32_t is_user_mode;
	/**
	 * current_page: for translation use. Translate the instructions
	 * within the page, for we cannot determine instructions out of the
	 * page is valid or not. It is flushed return from JIT Function. */
	addr_t current_page_phys;
	addr_t current_page_effec;
	Value *ptr_CURRENT_PAGE_PHYS;
	Value *ptr_CURRENT_PAGE_EFFEC;

	bool redirection;

	//Value *ptr_PC;
	//Value *ptr_PHYS_PC; /* The physical pc */
	Value **ptr_gpr; // GPRs
	Value **in_ptr_gpr;
	Value **ptr_xr; // XRs
	Value **in_ptr_xr;

	Value *ptr_srf;  //spr register file
	Value **ptr_spr; // SPRs
	Value **in_ptr_spr;

	Value **ptr_fpr; // FPRs
	Value **in_ptr_fpr;

	Value **ptr_FLAG; /* exploded version of flags */
	/* pointers to negative, overflow, zero and carry */
	Value *ptr_N;
	Value *ptr_V;
	Value *ptr_Z;
	Value *ptr_C;
	Value *ptr_T;

	uint64_t timer_total[TIMER_COUNT];
	uint64_t timer_start[TIMER_COUNT];

	void *feptr; /* This pointer can be used freely by the frontend. */
	dyncom_engine_t* dyncom_engine;
	debug_function_t debug_func;
	syscall_function_t syscall_func;
	switch_mode_function_t switch_mode;
	//int user_mode; /* indicate if the bb is for user mode during translation */
	//int TFlag; /* indicate if the bb is translated for thumb */
} cpu_t;

enum {
	JIT_RETURN_NOERR = 0,
	JIT_RETURN_FUNCNOTFOUND,
	JIT_RETURN_FUNC_BLANK,
	JIT_RETURN_SINGLESTEP,
	JIT_RETURN_TRAP,
	JIT_RETURN_TIMEOUT
};

//////////////////////////////////////////////////////////////////////
// codegen flags
//////////////////////////////////////////////////////////////////////
#define CPU_CODEGEN_NONE 0

// Run optimization passes on generated IR (disable for debugging)
#define CPU_CODEGEN_OPTIMIZE  (1<<1)

// Limits the DFS when tagging code, so that we don't
// translate all reachable code at a time, but only a
// certain amount of code in advance, and translate more
// on demand.
// If this is turned off, we do "entry caching", i.e. we
// create a file in /tmp that holds all entries to the code
// (i.e. all start addresses that can't be found automatically),
// and we start tagging at these addresses on load if the
// cache exists.
#define CPU_CODEGEN_TAG_LIMIT (1<<2)
#define CPU_CODEGEN_VERIFY  (1<<3)

//////////////////////////////////////////////////////////////////////
// debug flags
//////////////////////////////////////////////////////////////////////
#define CPU_DEBUG_NONE 0x00000000
#define CPU_DEBUG_SINGLESTEP			(1<<0)
#define CPU_DEBUG_SINGLESTEP_BB			(1<<1)
#define CPU_DEBUG_PRINT_IR				(1<<2)
#define CPU_DEBUG_PRINT_IR_OPTIMIZED	(1<<3)
#define CPU_DEBUG_LOG					(1<<4)
#define CPU_DEBUG_PROFILE				(1<<5)
#define CPU_DEBUG_PRINT_DISAS				(1<<6)
#define CPU_DEBUG_ALL 0xFFFFFFFF

//////////////////////////////////////////////////////////////////////
// hints
//////////////////////////////////////////////////////////////////////
#define CPU_HINT_NONE 0x00000000
#define CPU_HINT_TRAP_RETURNS		(1<<0)
#define CPU_HINT_TRAP_RETURNS_TWICE	(1<<1)

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define LOGGING (cpu->dyncom_engine->flags_debug & CPU_DEBUG_LOG)
#define LOG(...) do { if (LOGGING) printf(__VA_ARGS__); } while(0)

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

API_FUNC cpu_t *cpu_new(uint32_t flags, uint32_t arch_flags,arch_func_t arch_func);
API_FUNC void cpu_free(cpu_t *cpu);
API_FUNC void cpu_set_flags_codegen(cpu_t *cpu, uint32_t f);
API_FUNC void cpu_set_flags_hint(cpu_t *cpu, uint32_t f);
API_FUNC void cpu_set_flags_debug(cpu_t *cpu, uint32_t f);
API_FUNC void cpu_tag(cpu_t *cpu, addr_t pc);
API_FUNC int cpu_run(cpu_t *cpu);
API_FUNC int um_cpu_run(cpu_t *cpu);
API_FUNC void *cpu_translate(cpu_t *cpu, addr_t pc);
API_FUNC void cpu_set_ram(cpu_t *cpu, uint8_t *RAM);
API_FUNC void cpu_flush(cpu_t *cpu);
API_FUNC void cpu_print_statistics(cpu_t *cpu);

/* runs the interactive debugger */
API_FUNC int cpu_debugger(cpu_t *cpu, debug_function_t debug_function);

static inline int is_user_mode(cpu_t *cpu)
{
	return cpu->is_user_mode;
}

static inline int save_pc_for_all_insn(cpu_t* cpu){
	return ((cpu)->info.common_flags & CPU_FLAG_SAVE_PC);
}
static inline int save_pc_before_exec(cpu_t* cpu){
	return ((cpu)->info.common_flags & CPU_FLAG_SAVE_PC_BEFORE_EXEC);
}

#define FUNC_ATTR_NONE (0 << 0)
#define FUNC_ATTR_THUMB (1 << 0)
#define FUNC_ATTR_USERMODE (1 << 1)

static inline int is_thumb_func(cpu_t* cpu){
	return (cpu->dyncom_engine->func_attr[cpu->dyncom_engine->functions] & FUNC_ATTR_THUMB);
}

static inline int is_usermode_func(cpu_t* cpu){
	return (cpu->dyncom_engine->func_attr[cpu->dyncom_engine->functions] & FUNC_ATTR_USERMODE);
}

#endif
