/* Copyright (C) 
* 2012 - Michael.Kang blackfin.kang@gmail.com
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file ppc_dyncom_instr_main.cpp
* @brief The translation of main group instruction
* @author Michael.Kang blackfin.kang@gmail.com
* @version 78.77
* @date 2012-02-10
*/

#include "debug.h"
#include "tracers.h"
#include "ppc_dyncom_dec.h"
#include "ppc_exc.h"
#include "ppc_cpu.h"
#include "ppc_alu.h"
#include "ppc_dyncom_alu.h"
#include "ppc_dyncom_run.h"
#include "ppc_tools.h"
#include "ppc_syscall.h"
#include "ppc_mmu.h"
#include "ppc_e500_exc.h"

#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Instructions.h"
#include "llvm/DerivedTypes.h"
#include "llvm/CallingConv.h"
#include "llvm/Type.h"
#include "llvm/Constants.h"
#include "skyeye_dyncom.h"
#include <dyncom/dyncom_llvm.h>
#include <dyncom/frontend.h>
#include "dyncom/basicblock.h"
#include "skyeye.h"
#include "dyncom/defines.h"
#include "dyncom/tag.h"

#include "ppc_dyncom_debug.h"
#define TODO do{fprintf(stderr, "In %s, not implement\n", __FUNCTION__);exit(-1);}while(0);
#define NOT_TEST do{debug(DEBUG_NOT_TEST, "In %s,not tested\n", __FUNCTION__);}while(0);
/*
 *      addis           Add Immediate Shifted
 *      .428
 */
int opc_addis_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_Shift16(instr, rD, rA, imm);
	debug(DEBUG_TRANSLATE, "In %s, opc=0x%x,rA=%d, imm=%d\n", __FUNCTION__, instr, rA, imm);
	if(rA)
		LET(rD, ADD(R(rA), CONST(imm)));
	else
		LET(rD, CONST(imm));
	return 0;
}
ppc_opc_func_t ppc_opc_addis_func = {
	opc_default_tag,
	opc_addis_translate,
	opc_invalid_translate_cond,
};

/*
 *	addi		Add Immediate
 *	.425
 */
int opc_addi_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	debug(DEBUG_TRANSLATE, "In %s, opc=0x%x,rA=%d, imm=%d\n", __FUNCTION__, instr, rA, imm);
	if(rA)
		LET(rD, ADD(R(rA), CONST(imm)));
	else
		LET(rD, CONST(imm));
	return 0;
}
ppc_opc_func_t ppc_opc_addi_func = {
	opc_default_tag,
	opc_addi_translate,
	opc_invalid_translate_cond,
};

/*
 *	cmpi		Compare Immediate
 *	.443
 */
static int opc_cmpi_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	uint32 cr;
	int rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, cr, rA, imm);
	cr >>= 2;
	cr = 7 - cr;
	Value* c;
	c = SELECT(ICMP_SLT(R(rA), CONST(imm)), CONST(8), SELECT(ICMP_SGT(R(rA), CONST(imm)), CONST(4), CONST(2)));
	c = SELECT(ICMP_NE(AND(RSPR(XER_REGNUM), CONST(XER_SO)), CONST(0)), OR(c, CONST(1)), c);
	LETS(CR_REGNUM, AND(RSPR(CR_REGNUM), CONST(ppc_cmp_and_mask[cr])));
	LETS(CR_REGNUM, OR(RSPR(CR_REGNUM), SHL(c, CONST(cr * 4))));
	return 0;
}

ppc_opc_func_t ppc_opc_cmpi_func = {
	opc_default_tag,
	opc_cmpi_translate,
	opc_invalid_translate_cond,
};

/*
 *	rlwinmx		Rotate Left Word Immediate then AND with Mask
 *	.618
 */
static int opc_rlwinmx_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	int rS, rA, SH, MB, ME;
	PPC_OPC_TEMPL_M(instr, rS, rA, SH, MB, ME);
	Value* v = ROTL(R(rS), CONST(SH));
	uint32 mask = ppc_dyncom_mask(MB, ME);
	debug(DEBUG_TRANSLATE, "In %s,rS=%d,rA=%d,SH=%d,MB=%d,ME=%d,mask=0x%x\n",__func__,rS,rA,SH,MB,ME,mask);
	LET(rA, AND(v, CONST(mask)));
	if (instr & PPC_OPC_Rc) {
		// update cr0 flags
		ppc_dyncom_update_cr0(cpu, bb, rA);
	}
	NOT_TEST;
	return 0;
}
ppc_opc_func_t ppc_opc_rlwinmx_func = {
	opc_default_tag,
	opc_rlwinmx_translate,
	opc_invalid_translate_cond,
};

/*
 *	lwz		Load Word and Zero
 *	.557
 */
int opc_lwz_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_lwz_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value* addr;
	Value* old_rd = R(rD);
	if(rA)
		addr = ADD(R(rA), CONST(imm));
	else
		addr = CONST(imm);
	Value* result = arch_read_memory(cpu, bb, addr, 0, 32);
	/**
	 * If occur exception, rD should not to be written.
	 **/
	if(is_user_mode(cpu)){
		LET(rD, result);
	}else{
		Value *current_pc = RSPR(PHYS_PC_REGNUM);
		Value *exc_occur = ICMP_EQ(current_pc, CONST(PPC_EXC_DSI_ADDR));
		LET(rD, SELECT(exc_occur, old_rd, result));
	}
	return 0;
}
ppc_opc_func_t ppc_opc_lwz_func = {
	opc_lwz_tag,
	opc_lwz_translate,
	opc_invalid_translate_cond,
};

/*
 *	lwzu		Load Word and Zero with Update
 *	.558
 */
int opc_lwzu_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_lwzu_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	//e500_core_t* current_core = get_current_core();
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	// FIXME: check rA!=0 && rA!=rD
	Value * addr = ADD(R(rA), CONST(imm));
	Value * result = arch_read_memory(cpu, bb, addr, 0, 32);
	/**
	 * If occur exception, rD should not to be written.
	 **/
	if(is_user_mode(cpu)){
		LET(rA, addr);
		LET(rD, result);
	}else{
		Value *current_pc = RSPR(PHYS_PC_REGNUM);
		Value *exc_occur = ICMP_EQ(current_pc, CONST(PPC_EXC_DSI_ADDR));
		LET(rA, SELECT(exc_occur, R(rA), addr));
		LET(rD, SELECT(exc_occur, R(rD), result));
	}
	return 0;
}
ppc_opc_func_t ppc_opc_lwzu_func = {
	opc_lwzu_tag,
	opc_lwzu_translate,
	opc_invalid_translate_cond,
};

/*
 *	stw		Store Word
 *	.659
 */
int opc_stw_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_stw_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	Value *addr;
	if (rA)
		addr = ADD(R(rA), CONST(imm));
	else
		addr = CONST(imm);
	arch_write_memory(cpu, bb, addr, R(rS), 32);
	return 0;
}
ppc_opc_func_t ppc_opc_stw_func = {
	opc_stw_tag,
	opc_stw_translate,
	opc_invalid_translate_cond,
};

/*
 *	stwu		Store Word with Update
 *	.663
 */
int opc_stwu_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_stwu_translate(cpu_t *cpu, uint32_t instr, BasicBlock *bb)
{
	debug(DEBUG_TRANSLATE, "In %s\n", __func__);
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	debug(DEBUG_TRANSLATE, "In %s,rS=%d,rA=%d,imm=%d\n", __FUNCTION__, rS, rA, imm);
	// FIXME: check rA!=0
	/* FIXME: no MMU now */
	Value *addr = ADD(R(rA), CONST(imm));
	arch_write_memory(cpu, bb, addr, R(rS), 32);
	/**
	 * If occur exception, rD should not to be written.
	 **/
	if(is_user_mode(cpu)){
		LET(rA, addr);
	}else{
		Value *current_pc = RSPR(PHYS_PC_REGNUM);
		Value *exc_occur = ICMP_EQ(current_pc, CONST(PPC_EXC_DSI_ADDR));
		LET(rA, SELECT(exc_occur, R(rA), addr));
	}
	return 0;
}
ppc_opc_func_t ppc_opc_stwu_func = {
	opc_stwu_tag,
	opc_stwu_translate,
	opc_invalid_translate_cond,
};
/*
 *	bx		Branch
 *	.435
 */
int opc_bx_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	uint32 li;
	PPC_OPC_TEMPL_I(instr, li);
	if(is_user_mode(cpu)) {
		if (instr & PPC_OPC_LK)
			*tag = TAG_CALL;
		else
			*tag = TAG_BRANCH;
	}else{
		*tag = TAG_BRANCH;
	}
/*
	e500_core_t* current_core = get_core_from_dyncom_cpu(cpu);
	debug(DEBUG_TAG, "pc=0x%x,page_begin=0x%x,page_end=0x%x\n",
	current_core->pc,get_begin_of_page(current_core->pc),get_end_of_page(current_core->pc));
*/
//	if(li < get_begin_of_page(current_core->pc) && li > get_end_of_page(current_core->pc)){
//		*tag |= TAG_STOP;
//	}
	if(is_user_mode(cpu)) {
		if (!(instr & PPC_OPC_AA)){
			*new_pc = phys_pc + li;
		}
		else{
			*new_pc = li;
		}
	}else{
		*new_pc = NEW_PC_NONE;
	}
	*next_pc = phys_pc + PPC_INSN_SIZE;
	debug(DEBUG_TAG, "In %s, new_pc=0x%x\n", __FUNCTION__, *new_pc);
	return PPC_INSN_SIZE;
}
static int opc_bx_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	uint32 li;
	PPC_OPC_TEMPL_I(instr, li);
	if (instr & PPC_OPC_LK) {
		if(is_user_mode(cpu))
			LETS(LR_REGNUM, ADD(CONST(4), RSPR(PHYS_PC_REGNUM)));
		else
			LETS(LR_REGNUM, ADD(CONST(4), RSPR(PC_REGNUM)));
	}
	if (!(instr & PPC_OPC_AA)) {
		LETS(PHYS_PC_REGNUM, ADD(CONST(li), RSPR(PHYS_PC_REGNUM)));
		if(!is_user_mode(cpu))
			LETS(PC_REGNUM, ADD(CONST(li), RSPR(PC_REGNUM)));
		debug(DEBUG_TRANSLATE, "In %s, li=0x%x\n",__FUNCTION__, li);
	}else{
		LETS(PHYS_PC_REGNUM, CONST(li));
		if(!is_user_mode(cpu))
			LETS(PC_REGNUM, CONST(li));
		debug(DEBUG_TRANSLATE, "In %s, li=0x%x, br 0x%x\n", __FUNCTION__, li, li);
	}
	return PPC_INSN_SIZE;
}
ppc_opc_func_t ppc_opc_bx_func = {
	opc_bx_tag,
	opc_bx_translate,
	opc_invalid_translate_cond,
};

/*
 *	bcx		Branch Conditional
 *	.436
 */
int opc_bcx_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_COND_BRANCH;
	uint32 BO, BI, BD;
	PPC_OPC_TEMPL_B(instr, BO, BI, BD);
	uint32_t AA = instr & PPC_OPC_AA;
	if(is_user_mode(cpu)){
		if(AA)
			*new_pc = BD;
		else
			*new_pc = BD + phys_pc;
	}else{
		*new_pc = NEW_PC_NONE;
	}

#if 0
	if(*new_pc > get_end_of_page(phys_pc) || *new_pc < get_begin_of_page(phys_pc)){
		*new_pc = NEW_PC_NONE;
		*tag |= TAG_STOP;
	}
#endif
	*next_pc = phys_pc + PPC_INSN_SIZE;
	debug(DEBUG_TAG, "In %s, new_pc=0x%x, BD=0x%x\n", __FUNCTION__, *new_pc, BD);
	return PPC_INSN_SIZE;
}
Value* opc_bcx_translate_cond(cpu_t *cpu, uint32_t instr, BasicBlock *bb){
	NOT_TEST;
	uint32 BO, BI, BD;
	PPC_OPC_TEMPL_B(instr, BO, BI, BD);
	if (!(BO & 4)) {
		LETS(CTR_REGNUM, SUB(RSPR(CTR_REGNUM), CONST(1)));
	}
	bool_t bo2 = ((BO & 2)?True: False); //branch if ctr==0 when true
	bool_t bo8 = ((BO & 8)?True: False); // branch condition true
	Value* cr_value = SELECT(ICMP_NE(AND(RSPR(CR_REGNUM), CONST(1<<(31-BI))), CONST(0)), CONST(1), CONST(0));
	Value* tmp1 = ICMP_NE(AND(CONST(BO), CONST(4)), CONST(0));
	Value* tmp2 = XOR(ICMP_NE(RSPR(CTR_REGNUM), CONST(0)), ICMP_NE(CONST(bo2), CONST(0)));
	Value* tmp3 = ICMP_NE(AND(CONST(BO), CONST(16)), CONST(0));
	Value* tmp4 = ICMP_EQ(XOR(cr_value, CONST(bo8)), CONST(0));
	return AND(OR(tmp1, tmp2), OR(tmp3, tmp4));
	//return AND(
	//	OR(ICMP_NE(CONST(BO & 4), CONST(0)), XOR(LOG_NOT(RS(CTR_REGNUM)), CONST1(bo2))), 
	//	OR(ICMP_NE(CONST(BO & 16), CONST(0)), LOG_NOT(XOR(cr_value, CONST(bo8))))
	//);
}   //
static int opc_bcx_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	uint32 BO, BI, BD;
	PPC_OPC_TEMPL_B(instr, BO, BI, BD);
	if (instr & PPC_OPC_LK) {
		if(is_user_mode(cpu))
			LETS(LR_REGNUM, ADD(RSPR(PHYS_PC_REGNUM), CONST(4)));
		else
			LETS(LR_REGNUM, ADD(RSPR(PC_REGNUM), CONST(4)));
	}
	if (!(instr & PPC_OPC_AA)) {
		//arch_store(ADD(CONST(BD), RSPR(PHYS_PC_REGNUM)), cpu->ptr_PHYS_PC, bb);
		LETS(PHYS_PC_REGNUM, ADD(CONST(BD), RSPR(PHYS_PC_REGNUM)));
		if(!is_user_mode(cpu))
			//arch_store(ADD(CONST(BD), RSPR(PC_REGNUM)), cpu->ptr_PC, bb);
			LETS(PC_REGNUM, ADD(CONST(BD), RSPR(PC_REGNUM)));
	}else{
		//arch_store(CONST(BD), cpu->ptr_PHYS_PC, bb);
		LETS(PHYS_PC_REGNUM, CONST(BD));
		if(!is_user_mode(cpu))
			//arch_store(CONST(BD), cpu->ptr_PC, bb);
			LETS(PC_REGNUM, CONST(BD));
	}
	return 0;
}
ppc_opc_func_t ppc_opc_bcx_func = {
	opc_bcx_tag,
	opc_bcx_translate,
	opc_bcx_translate_cond,
};
/*
 *	twi		Trap Word Immediate
 *	.679
 */
int opc_twi_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_twi_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
	int TO, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, TO, rA, imm);
	Value *reg_a = R(rA);
	Value *cond = OR(OR(OR(OR(AND(ICMP_EQ(CONST(TO & 16), CONST(0)), ICMP_SLT(reg_a, CONST(imm))),
		AND(ICMP_EQ(CONST(TO & 8), CONST(0)), ICMP_SGT(reg_a, CONST(imm)))),
			AND(ICMP_EQ(CONST(TO & 4), CONST(0)), ICMP_EQ(reg_a, CONST(imm)))),
				AND(ICMP_EQ(CONST(TO & 2), CONST(0)), ICMP_ULT(reg_a, CONST(imm)))),
					AND(ICMP_EQ(CONST(TO & 1), CONST(0)), ICMP_UGT(reg_a, CONST(imm))));
	arch_ppc_dyncom_exception(cpu, bb, cond, PROG, PPC_EXC_PROGRAM_TRAP, 0);

	return No_exp;
}
ppc_opc_func_t ppc_opc_twi_func = {
        opc_twi_tag,
        opc_twi_translate,
        opc_invalid_translate_cond,
};
/*
 *	mulli		Multiply Low Immediate
 *	.598
 */
static int opc_mulli_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rD, rA;
	uint32 imm;

	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	// FIXME: signed / unsigned correct?
	LET(rD,MUL(R(rA), CONST(imm)));
	NOT_TEST;
	//current_core->gpr[rD] = current_core->gpr[rA] * imm;
	return No_exp;
}

ppc_opc_func_t ppc_opc_mulli_func = {
        opc_default_tag,
        opc_mulli_translate,
        opc_invalid_translate_cond,
};
/*
 *	subfic		Subtract From Immediate Carrying
 *	.669
 */
static int opc_subfic_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value *v_ra = R(rA);
	LET(rD, ADD(XOR(v_ra, CONST(-1)), CONST(imm + 1)));
	// update XER
	Value* cond = ppc_dyncom_carry_3(cpu, bb, XOR(v_ra, CONST(-1)), CONST(imm), CONST(1));
	LETS(XER_REGNUM, SELECT(cond,
			OR(RSPR(XER_REGNUM), CONST(XER_CA)),
			AND(RSPR(XER_REGNUM), CONST(~XER_CA)))
	);

	return No_exp;
}

ppc_opc_func_t ppc_opc_subfic_func = {
        opc_default_tag,
        opc_subfic_translate,
        opc_invalid_translate_cond,
};
/*
 *	cmpli		Compare Logical Immediate
 *	.445
 */
static int opc_cmpli_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	uint32 cr;
	int rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_UImm(instr, cr, rA, imm);
	cr >>= 2;
	cr = 7-cr;
	Value* c;
	c = SELECT(ICMP_ULT(R(rA), CONST(imm)), CONST(8), SELECT(ICMP_UGT(R(rA), CONST(imm)), CONST(4), CONST(2)));
	c = SELECT(ICMP_NE(AND(RSPR(XER_REGNUM), CONST(XER_SO)), CONST(0)), OR(c, CONST(1)), c);
	LETS(CR_REGNUM, AND(RSPR(CR_REGNUM), CONST(ppc_cmp_and_mask[cr])));
	LETS(CR_REGNUM, OR(RSPR(CR_REGNUM), SHL(c, CONST(cr * 4))));
	return 0;
}

ppc_opc_func_t ppc_opc_cmpli_func = {
	opc_default_tag,
	opc_cmpli_translate,
	opc_invalid_translate_cond,
};
/*
 *	addic		Add Immediate Carrying
 *	.426
 */
static int opc_addic_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value *v_ra = R(rA);
	LET(rD, ADD(v_ra, CONST(imm)));
	// update XER
	LETS(XER_REGNUM, SELECT(ICMP_ULT(R(rD), v_ra), 
		OR(RSPR(XER_REGNUM), CONST(XER_CA)), 
		AND(RSPR(XER_REGNUM), CONST(~XER_CA)))
	);
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_addic_func = {
	opc_default_tag,
	opc_addic_translate,
	opc_invalid_translate_cond,
};
/*
 *	addic.		Add Immediate Carrying and Record
 *	.427
 */
static int opc_addic__translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value *v_ra = R(rA);
	LET(rD, ADD(v_ra, CONST(imm)));
	// update XER
	LETS(XER_REGNUM, SELECT(ICMP_ULT(R(rD), v_ra), 
		OR(RSPR(XER_REGNUM), CONST(XER_CA)), 
		AND(RSPR(XER_REGNUM), CONST(~XER_CA)))
	);
	// update cr0 flags
	ppc_dyncom_update_cr0(cpu, bb, rD);

	return No_exp;
}

ppc_opc_func_t ppc_opc_addic__func = {
        opc_default_tag,
        opc_addic__translate,
        opc_invalid_translate_cond,
};
int opc_sc_tag(cpu_t *cpu, uint32_t instr, addr_t phys_addr,tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	if(is_user_mode(cpu)) {
#ifdef OPT_LOCAL_REGISTERS
		*tag = TAG_TRAP;
		*next_pc = phys_addr + PPC_INSN_SIZE;
#else
		*tag = TAG_CONTINUE;
		*next_pc = phys_addr + PPC_INSN_SIZE;
#endif
	}else{
		*tag = TAG_EXCEPTION;
	}
	return PPC_INSN_SIZE;
}
static int opc_sc_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	if(is_user_mode(cpu)) {
#ifdef OPT_LOCAL_REGISTERS
#else
		arch_syscall(cpu, bb, 0);
#endif
	}else{
		arch_ppc_dyncom_exception(cpu, bb, CONST1(1), ((uint32_t *)cpu->rf.srf)[SYSCALL_NUMBER_REGNUM], 0, 0);
	}
	return 0;
}

ppc_opc_func_t ppc_opc_sc_func = {
        opc_sc_tag,
        opc_sc_translate,
        opc_invalid_translate_cond,
};
/*
 *	rlwimix		Rotate Left Word Immediate then Mask Insert
 *	.617
 */
static int opc_rlwimix_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA, SH, MB, ME;
	PPC_OPC_TEMPL_M(instr, rS, rA, SH, MB, ME);
	Value* v = ROTL(R(rS), CONST(SH));
	uint32 mask = ppc_dyncom_mask(MB, ME);
	//current_core->gpr[rA] = (v & mask) | (current_core->gpr[rA] & ~mask);
	LET(rA, OR(AND(v, CONST(mask)), AND(R(rA), CONST(~mask))));
	if (instr & PPC_OPC_Rc) {
		// update cr0 flags
		ppc_dyncom_update_cr0(cpu, bb, rA);
	}

	return No_exp;
}

ppc_opc_func_t ppc_opc_rlwimix_func = {
        opc_default_tag,
        opc_rlwimix_translate,
        opc_invalid_translate_cond,
};
/*
 *	rlwnmx		Rotate Left Word then AND with Mask
 *	.620
 */
static int opc_rlwnmx_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA, rB, MB, ME;
	PPC_OPC_TEMPL_M(instr, rS, rA, rB, MB, ME);
	
	uint32 mask = ppc_dyncom_mask(MB, ME);
	LET(rA, AND(ROTL(R(rS), R(rB)), CONST(mask)));
	if (instr & PPC_OPC_Rc) {
		// update cr0 flags
		ppc_dyncom_update_cr0(cpu, bb, rA);
	}

	return No_exp;
}
ppc_opc_func_t ppc_opc_rlwnmx_func = {
        opc_default_tag,
        opc_rlwnmx_translate,
        opc_invalid_translate_cond,
};
/*
 *	ori		OR Immediate
 *	.605
 */
static int opc_ori_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_UImm(instr, rS, rA, imm);
	LET(rA, OR(R(rS), CONST(imm)));
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_ori_func = {
        opc_default_tag,
        opc_ori_translate,
        opc_invalid_translate_cond,
};
/*
 *	oris		OR Immediate Shifted
 *	.606
 */
static int opc_oris_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_Shift16(instr, rS, rA, imm);
	LET(rA, OR(R(rS), CONST(imm)));
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_oris_func = {
        opc_default_tag,
        opc_oris_translate,
        opc_invalid_translate_cond,
};
/*
 *	xori		XOR Immediate
 *	.681
 */
static int opc_xori_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_UImm(instr, rS, rA, imm);
	LET(rA, XOR(R(rS), CONST(imm)));

	return No_exp;
}

ppc_opc_func_t ppc_opc_xori_func = {
        opc_default_tag,
        opc_xori_translate,
        opc_invalid_translate_cond,
};
/*
 *	xoris		XOR Immediate Shifted
 *	.682
 */
static int opc_xoris_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_Shift16(instr, rS, rA, imm);
	LET(rA, XOR(R(rS), CONST(imm)));

	return No_exp;
}

ppc_opc_func_t ppc_opc_xoris_func = {
	opc_default_tag,
	opc_xoris_translate,
	opc_invalid_translate_cond,
};
/*
 *	andi.		AND Immediate
 *	.433
 */
static int opc_andi__translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_UImm(instr, rS, rA, imm);
	LET(rA, AND(R(rS), CONST(imm)));
	NOT_TEST;
	// update cr0 flags
	ppc_dyncom_update_cr0(cpu, bb, rA);

	return No_exp;
}

ppc_opc_func_t ppc_opc_andi__func = {
        opc_default_tag,
        opc_andi__translate,
        opc_invalid_translate_cond,
};
/*
 *	andis.		AND Immediate Shifted
 *	.434
 */
static int opc_andis__translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_Shift16(instr, rS, rA, imm);
	LET(rA, AND(R(rS), CONST(imm)));
	NOT_TEST;
	// update cr0 flags
	ppc_dyncom_update_cr0(cpu, bb, rA);

	return No_exp;
}

ppc_opc_func_t ppc_opc_andis__func = {
        opc_default_tag,
        opc_andis__translate,
        opc_invalid_translate_cond,
};
/*
 *	lbz		Load Byte and Zero
 *	.521
 */
int opc_lbz_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_lbz_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value* ret;
	Value* rd_old = R(rD);
	if(rA)
		ret = arch_read_memory(cpu, bb, ADD(R(rA), CONST(imm)), 0 ,8);
	else
		ret = arch_read_memory(cpu, bb,  CONST(imm), 0 ,8);
	/**
	 * If occur exception, rD should not to be written.
	 **/
	if(is_user_mode(cpu)){
		LET(rD, ret);
	}else{
		Value *current_pc = RSPR(PHYS_PC_REGNUM);
		Value *exc_occur = ICMP_EQ(current_pc, CONST(PPC_EXC_DSI_ADDR));
		LET(rD, SELECT(exc_occur, rd_old, ret));
	}
	return 0;
}

ppc_opc_func_t ppc_opc_lbz_func = {
        opc_lbz_tag,
        opc_lbz_translate,
        opc_invalid_translate_cond,
};
/*
 *	lbzu		Load Byte and Zero with Update
 *	.522
 */
static int opc_lbzu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	// FIXME: check rA!=0 && rA!=rD
	uint8 r;
	Value* ret;
	Value *addr = ADD(R(rA), CONST(imm));
	ret = arch_read_memory(cpu, bb, addr, 0 ,8);
	LET(rD, ret);
	LET(rA, addr);
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_lbzu_func = {
        opc_default_tag,
        opc_lbzu_translate,
        opc_invalid_translate_cond,
};
 /*
 *	stb		Store Byte
 *	.632
 */
int opc_stb_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_stb_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	if(rA)
		arch_write_memory(cpu, bb, ADD(R(rA), CONST(imm)), R(rS), 8);
	else
		arch_write_memory(cpu, bb, CONST(imm), R(rS), 8);
	NOT_TEST;		

	return No_exp;
}

ppc_opc_func_t ppc_opc_stb_func = {
        opc_stb_tag,
        opc_stb_translate,
        opc_invalid_translate_cond,
};

static int opc_stbu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	Value* addr = ADD(R(rA), CONST(imm));
	arch_write_memory(cpu, bb, addr, R(rS), 8);
	LET(rA, addr);
	return 0;
}

ppc_opc_func_t ppc_opc_stbu_func = {
        opc_default_tag,
        opc_stbu_translate,
        opc_invalid_translate_cond,
};

/*
 *	lhz		Load Half Word and Zero
 *	.543
 */
int opc_lhz_tag(cpu_t *cpu, uint32_t instr, addr_t phys_pc, tag_t *tag, addr_t *new_pc, addr_t *next_pc){
	*tag = TAG_CONTINUE;
	*tag |= TAG_EXCEPTION;
	*next_pc = phys_pc + PPC_INSN_SIZE;
	*new_pc = NEW_PC_NONE;
	return PPC_INSN_SIZE;
}
static int opc_lhz_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	uint16 r;
	Value* ret;
	if(rA)
		ret = arch_read_memory(cpu, bb, ADD(R(rA), CONST(imm)), 0 ,16);
	else
		ret = arch_read_memory(cpu, bb,  CONST(imm), 0 ,16);
	/**
	 * If occur exception, rD should not to be written.
	 **/
	if(is_user_mode(cpu)){
		LET(rD, ret);
	}else{
		Value *current_pc = RSPR(PHYS_PC_REGNUM);
		Value *exc_occur = ICMP_EQ(current_pc, CONST(PPC_EXC_DSI_ADDR));
		LET(rD, SELECT(exc_occur, R(rD), ret));
	}

	return No_exp;
}

ppc_opc_func_t ppc_opc_lhz_func = {
        opc_lhz_tag,
        opc_lhz_translate,
        opc_invalid_translate_cond,
};
/*
 *	lhzu		Load Half Word and Zero with Update
 *	.544
 */
static int opc_lhzu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	uint16 r;
	Value* ret;
	Value* addr = ADD(R(rA), CONST(imm));
	ret = arch_read_memory(cpu, bb, addr, 0 ,16);
	LET(rD, ret);
	LET(rA, addr);
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_lhzu_func = {
        opc_default_tag,
        opc_lhzu_translate,
        opc_invalid_translate_cond,
};
/*
 *	lha		Load Half Word Algebraic
 *	.538
 */
static int opc_lha_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value* ret;
	if(rA)
		ret = arch_read_memory(cpu, bb, ADD(R(rA), CONST(imm)), 0 ,16);
	else
		ret = arch_read_memory(cpu, bb,  CONST(imm), 0 ,16);
	LET(rD, SELECT(ICMP_NE(AND(ret, CONST(0x8000)), CONST(0)), OR(ret, CONST(0xFFFF0000)), ret));
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_lha_func = {
        opc_default_tag,
        opc_lha_translate,
        opc_invalid_translate_cond,
};
/*
 *	lhau		Load Half Word Algebraic with Update
 *	.539
 */
static int opc_lhau_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rD;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value* ret;
	Value* addr = ADD(R(rA), CONST(imm));
	ret = arch_read_memory(cpu, bb, addr, 0 ,16);
	LET(rD, SELECT(ICMP_NE(AND(ret, CONST(0x8000)), CONST(0)), OR(ret, CONST(0xFFFF0000)), ret));
	LET(rA, addr);
	NOT_TEST;

	return No_exp;
}

ppc_opc_func_t ppc_opc_lhau_func = {
        opc_default_tag,
        opc_lhau_translate,
        opc_invalid_translate_cond,
};

/*
 *	sth		Store Half Word
 *	.651
 */
static int opc_sth_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	if(rA)
		arch_write_memory(cpu, bb, ADD(R(rA), CONST(imm)), R(rS), 16);
	else
		arch_write_memory(cpu, bb, CONST(imm), R(rS), 16);

	return No_exp;
}

ppc_opc_func_t ppc_opc_sth_func = {
        opc_default_tag,
        opc_sth_translate,
        opc_invalid_translate_cond,
};
/*
 *	sthu		Store Half Word with Update
 *	.653
 */
static int opc_sthu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rA, rS;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	Value *addr = ADD(R(rA), CONST(imm));
	arch_write_memory(cpu, bb, addr, R(rS), 16);
	LET(rA, addr);

	return No_exp;
}

ppc_opc_func_t ppc_opc_sthu_func = {
        opc_default_tag,
        opc_sthu_translate,
        opc_invalid_translate_cond,
};
/*
 *	lmw		Load Multiple Word
 *	.547
 */
static int opc_lmw_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rD, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rD, rA, imm);
	Value* ea;
	if(rA)
		ea = ADD(R(rA), CONST(imm));
	else
		ea = CONST(imm);
	while (rD <= 31) {
		LET(rD, arch_read_memory(cpu, bb, ea, 0 ,32));
		rD++;
		ea = ADD(ea, CONST(4));
	}

	return No_exp;
}
ppc_opc_func_t ppc_opc_lmw_func = {
        opc_default_tag,
        opc_lmw_translate,
        opc_invalid_translate_cond,
};
/*
 *	stmw		Store Multiple Word
 *	.656
 */
static int opc_stmw_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb)
{
	int rS, rA;
	uint32 imm;
	PPC_OPC_TEMPL_D_SImm(instr, rS, rA, imm);
	Value* ea;
	if(rA)
		ea = ADD(R(rA), CONST(imm));
	else
		ea = CONST(imm);

	while (rS <= 31) {
		arch_write_memory(cpu, bb, ea, R(rS), 32);
		rS++;
		ea = ADD(ea, CONST(4));
	}

	return No_exp;
}

ppc_opc_func_t ppc_opc_stmw_func = {
        opc_default_tag,
        opc_stmw_translate,
        opc_invalid_translate_cond,
};
static int opc_lfs_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
	TODO;
	return 0;
}
ppc_opc_func_t ppc_opc_lfs_func = {
        opc_default_tag,
        opc_lfs_translate,
        opc_invalid_translate_cond,
};
static int opc_lfsu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
	TODO;
	return 0;
}

ppc_opc_func_t ppc_opc_lfsu_func = {
        opc_default_tag,
        opc_lfsu_translate,
        opc_invalid_translate_cond,
};
static int opc_lfd_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}

ppc_opc_func_t ppc_opc_lfd_func = {
        opc_default_tag,
        opc_lfd_translate,
        opc_invalid_translate_cond,
};
static int opc_lfdu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}

ppc_opc_func_t ppc_opc_lfdu_func = {
        opc_default_tag,
        opc_lfdu_translate,
        opc_invalid_translate_cond,
};
static int opc_stfs_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}
ppc_opc_func_t ppc_opc_stfs_func = {
        opc_default_tag,
        opc_stfs_translate,
        opc_invalid_translate_cond,
};
static int opc_stfsu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}
ppc_opc_func_t ppc_opc_stfsu_func = {
        opc_default_tag,
        opc_stfsu_translate,
        opc_invalid_translate_cond,
};
static int opc_stfd_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}
ppc_opc_func_t ppc_opc_stfd_func = {
        opc_default_tag,
        opc_stfd_translate,
        opc_invalid_translate_cond,
};
static int opc_stfdu_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
        TODO;
        return 0;
}
ppc_opc_func_t ppc_opc_stfdu_func = {
        opc_default_tag,
        opc_stfdu_translate,
        opc_invalid_translate_cond,
};
static int opc_none_translate(cpu_t* cpu, uint32_t instr, BasicBlock* bb){
	return No_exp;
}
ppc_opc_func_t ppc_opc_none_func = {
        opc_default_tag,
        opc_none_translate,
        opc_invalid_translate_cond,
};
