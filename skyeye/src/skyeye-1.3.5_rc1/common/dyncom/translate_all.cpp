/**
 * @file translate_all.cpp
 * 
 * This translates all known code by creating basic blocks and
 * filling them with instructions.
 *
 * @author OS Center,TsingHua University (Ported from libcpu)
 * @date 11/11/2010
 */

#include "llvm/BasicBlock.h"
#include "llvm/Instructions.h"

#include "dyncom/frontend.h"
#include "skyeye_dyncom.h"
#include "dyncom/dyncom_llvm.h"
#include "dyncom/basicblock.h"
#include "disasm.h"
#include "dyncom/tag.h"
#include "translate.h"
#include "dyncom/defines.h"
#include "llvm/Constants.h"
//#include "libcpu_run.h"

/**
 * @brief translate all the instructions. 
 *
 * @param cpu CPU core structure
 * @param bb_ret return basic block
 * @param bb_trap trap basic block
 *
 * @return dispatch basic block 
 */
BasicBlock *
cpu_translate_all(cpu_t *cpu, BasicBlock *bb_ret, BasicBlock *bb_trap, BasicBlock *bb_timeout)
{
	// find all instructions that need labels and create basic blocks for them
	int bbs = 0;
	addr_t pc;

	/* reset instruction contaioner. */
	vector<addr_t> &addrset = cpu->dyncom_engine->insns_in_jit;
	addrset.clear();

	int cur_pos = cpu->dyncom_engine->functions;
	vector<addr_t>::iterator i = cpu->dyncom_engine->startbb[cur_pos].begin();
	for(; i < cpu->dyncom_engine->startbb[cur_pos].end(); i++){

		create_basicblock(cpu, *i, cpu->dyncom_engine->cur_func, BB_TYPE_NORMAL);
		LOG("create bb 0x%x\n", *i);
		bbs ++;
	}
	LOG("bbs: %d\n", bbs);

	/* create dispatch basicblock
	 * create a bb to count the cycles stay inside the JIT Function.
	 * If up to some threshold value, force to return.
	 * */
	BasicBlock* bb_dispatch = BasicBlock::Create(_CTX(), "dispatch", cpu->dyncom_engine->cur_func, 0);
	SwitchInst* sw;
	if(is_user_mode(cpu)){
		#if OPT_LOCAL_REGISTERS
		Value *v_pc;
		/* Now only for arm platform */
		if(cpu->info.pc_index_in_gpr != -1)
			v_pc = arch_get_reg(cpu, cpu->info.pc_index_in_gpr, 32, bb_dispatch);
		else
			v_pc = new LoadInst(cpu->ptr_gpr[18], "", false, bb_dispatch);

#else
		Value *v_pc = new LoadInst(cpu->ptr_gpr[18], "", false, bb_dispatch);
#endif
		sw = SwitchInst::Create(v_pc, bb_ret, bbs, bb_dispatch);
	} else {
	
		BasicBlock* bb_real_dispatch = BasicBlock::Create(_CTX(), "real_dispatch", cpu->dyncom_engine->cur_func, 0);
                LoadInst* v_cpsr = new LoadInst(cpu->ptr_gpr[16], "", false, bb_dispatch);
                LoadInst* v_nirq_sig = new LoadInst(cpu->ptr_Nirq, "", false, bb_dispatch);
                Value *int_enable =     BinaryOperator::Create(Instruction::And, v_cpsr, CONST(0x80), "", bb_dispatch);
                Value *irq_pending =    BinaryOperator::Create(Instruction::Or, int_enable, v_nirq_sig, "", bb_dispatch);
                Value *gout = new ICmpInst(*bb_dispatch, ICmpInst::ICMP_EQ, irq_pending, CONST(0), "");
                BranchInst::Create(bb_trap, bb_real_dispatch, gout, bb_dispatch);
                // create dispatch basicblock
                Value *v_pc = new LoadInst(cpu->ptr_gpr[18], "", false, bb_real_dispatch);
                sw = SwitchInst::Create(v_pc, bb_ret, bbs, bb_real_dispatch);
	}

	// translate basic blocks
	bbaddr_map &bb_addr = cpu->dyncom_engine->func_bb[cpu->dyncom_engine->cur_func];
	bbaddr_map::const_iterator it;
	for (it = bb_addr.begin(); it != bb_addr.end(); it++) {
		pc = it->first;
		BasicBlock *cur_bb = it->second;

		tag_t tag;
		BasicBlock *bb_target = NULL, *bb_next = NULL, *bb_cont = NULL, *bb_cond = NULL;

		// Tag the function as translated.
		or_tag(cpu, pc, TAG_TRANSLATED);

		LOG("basicblock: L%08llx\n", (unsigned long long)pc);

		// Add dispatch switch case for basic block.
		ConstantInt* c = ConstantInt::get(getIntegerType(cpu->info.address_size), pc);
		/* we will not add entry of switch for the insn after memory access */
		if((get_tag(cpu, pc) & TAG_AFTER_NEW_BB) && !is_start_of_basicblock(cpu, pc))
			; /* do nothing */
		else{
			sw->addCase(c, cur_bb);
			or_tag(cpu, pc, TAG_ENTRY);
		}
		do {
			tag_t dummy1;

			tag = get_tag(cpu, pc);
			LOG("TAG of 0x%x = 0x%x\n", pc, tag);

			/* get address of the following instruction */
			addr_t new_pc, next_pc;
			cpu->f.tag_instr(cpu, pc, &dummy1, &new_pc, &next_pc);
			or_tag(cpu, pc, TAG_TRANSLATED);

			/* get target basic block */
			if (tag & TAG_RET)
				bb_target = bb_dispatch;
			if (tag & (TAG_CALL|TAG_BRANCH|TAG_POSTCOND)) {
				if (new_pc == NEW_PC_NONE) /* translate_instr() will set PC */
					bb_target = bb_dispatch;
				else {
					bb_target = (BasicBlock*)lookup_basicblock(cpu, cpu->dyncom_engine->cur_func, new_pc, bb_ret, BB_TYPE_NORMAL);
					/* Avoid dead loop in a process */
					//dummy1 = get_tag(cpu, new_pc);
					//if (dummy1 & TAG_TRANSLATED)
					//	check_intr(cpu,cur_bb,bb_trap,bb_target);
						//bb_target = bb_dispatch;
				}
			}
			#if 0
			if (new_pc > cpu->code_end) {
				return bb_dispatch;
			}
			#endif
			/* get not-taken basic block */
			if(is_user_mode(cpu)){
				if (tag & (TAG_CONDITIONAL | TAG_POSTCOND | TAG_LAST_INST ))
 					bb_next = (BasicBlock*)lookup_basicblock(cpu, cpu->dyncom_engine->cur_func, next_pc, bb_ret, BB_TYPE_NORMAL);
			}
			else{
				if (tag & (TAG_CONDITIONAL | TAG_POSTCOND | TAG_LAST_INST | (TAG_NEW_BB)))
 					bb_next = (BasicBlock*)lookup_basicblock(cpu, cpu->dyncom_engine->cur_func, next_pc, bb_ret, BB_TYPE_NORMAL);
			}
//			if (!(tag & TAG_BRANCH)) {
//			if (!(tag & TAG_COND_BRANCH) && !(tag & TAG_BRANCH)) {
//				arm_emit_store_pc(cpu, cur_bb, pc);
//			}
//			}
#if 0
#if 1 /* enabled for OS running */
//            printf("new pc is %x\n", pc);
			emit_store_pc(cpu, cur_bb, pc);
#else
			if(!(tag & TAG_CONTINUE))
				//update pc
				emit_store_pc(cpu, cur_bb, pc);
#endif
#endif
			if(save_pc_for_all_insn(cpu)){
				emit_store_pc(cpu, cur_bb, pc);
			}
			else{
				if(!(tag & TAG_CONTINUE))
					//update pc
					emit_store_pc(cpu, cur_bb, pc);
			}

#if ENABLE_ICOUNTER
			arch_inc_icounter(cpu, cur_bb);
#endif
// Only for debug all the execution instructions
#if ENABLE_DEBUG_ME 
			cur_bb = arch_debug_me(cpu, cur_bb, bb_trap);
#endif

			if((tag & TAG_EXCEPTION) && !is_user_mode(cpu))
				emit_store_pc(cpu, cur_bb, next_pc);
			if((tag & TAG_END_PAGE) && !is_user_mode(cpu)) {
				LOG("TAG_END_PAGE next pc is %x\n", next_pc);
				emit_store_pc_end_page(cpu, tag, cur_bb, next_pc);
			}
			bb_cont = translate_instr(cpu, pc, next_pc, tag, bb_target, bb_trap, bb_next, bb_ret, cur_bb);
			addrset.push_back(pc);
			if (!is_user_mode(cpu) && bb_cont && (tag & TAG_NEW_BB) && !(tag & TAG_BRANCH)) {
				if (!bb_cont->getTerminator()) {
					BranchInst::Create(bb_next, bb_cont);
				}
			}
			pc = next_pc;
		} while (
					/* new basic block starts here (and we haven't translated it yet)*/
					(!(is_start_of_basicblock(cpu, pc) || (get_tag(cpu, pc) & TAG_AFTER_NEW_BB))) &&
					/* end of code section */ //XXX no: this is whether it's TAG_CODE
					is_code(cpu, pc) &&
					/* last intruction jumped away */
					bb_cont
				);

		/* link with next basic block if there isn't a control flow instr. already */
		if (bb_cont) {
			BasicBlock *target = (BasicBlock*)lookup_basicblock(cpu, cpu->dyncom_engine->cur_func, pc, bb_ret, BB_TYPE_NORMAL);
			LOG("info: linking continue $%04llx!\n", (unsigned long long)pc);
			if (!(tag & TAG_CONDITIONAL)) {
				if (!bb_cont->getTerminator()) {
					BranchInst::Create(target, bb_cont);
				}
			}
		}
    }
	return bb_dispatch;
}
