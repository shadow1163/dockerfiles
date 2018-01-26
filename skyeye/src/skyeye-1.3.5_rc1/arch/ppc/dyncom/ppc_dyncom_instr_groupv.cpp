#include "debug.h"
#include "tracers.h"
#include "ppc_dyncom_dec.h"
#include "ppc_exc.h"
#include "ppc_cpu.h"
#include "ppc_dyncom_alu.h"
#include "ppc_dyncom_run.h"
#include "ppc_tools.h"
#include "ppc_mmu.h"

#include "llvm/Instructions.h"
#include <dyncom/dyncom_llvm.h>
#include <dyncom/frontend.h>
#include "dyncom/basicblock.h"
#include "skyeye.h"

#include "ppc_dyncom_debug.h"
ppc_opc_func_t ppc_opc_vaddubm_func;
ppc_opc_func_t ppc_opc_vmaxub_func;
ppc_opc_func_t ppc_opc_vrlb_func;
ppc_opc_func_t ppc_opc_vmuloub_func;
ppc_opc_func_t ppc_opc_vaddfp_func;
ppc_opc_func_t ppc_opc_vmrghb_func;
ppc_opc_func_t ppc_opc_vpkuhum_func;
ppc_opc_func_t ppc_opc_vadduhm_func;
ppc_opc_func_t ppc_opc_vmaxuh_func;
ppc_opc_func_t ppc_opc_vrlh_func;
ppc_opc_func_t ppc_opc_vmulouh_func;
ppc_opc_func_t ppc_opc_vsubfp_func;
ppc_opc_func_t ppc_opc_vmrghh_func;
ppc_opc_func_t ppc_opc_vpkuwum_func;
ppc_opc_func_t ppc_opc_vadduwm_func;
ppc_opc_func_t ppc_opc_vmaxuw_func;
ppc_opc_func_t ppc_opc_vrlw_func;
ppc_opc_func_t ppc_opc_vmrghw_func;
ppc_opc_func_t ppc_opc_vpkuhus_func;
ppc_opc_func_t ppc_opc_vpkuwus_func;
ppc_opc_func_t ppc_opc_vmaxsb_func;
ppc_opc_func_t ppc_opc_vslb_func;
ppc_opc_func_t ppc_opc_vmulosb_func;
ppc_opc_func_t ppc_opc_vrefp_func;
ppc_opc_func_t ppc_opc_vmrglb_func;
ppc_opc_func_t ppc_opc_vpkshus_func;
ppc_opc_func_t ppc_opc_vmaxsh_func;
ppc_opc_func_t ppc_opc_vslh_func;
ppc_opc_func_t ppc_opc_vmulosh_func;
ppc_opc_func_t ppc_opc_vrsqrtefp_func;
ppc_opc_func_t ppc_opc_vmrglh_func;
ppc_opc_func_t ppc_opc_vpkswus_func;
ppc_opc_func_t ppc_opc_vaddcuw_func;
ppc_opc_func_t ppc_opc_vmaxsw_func;
ppc_opc_func_t ppc_opc_vslw_func;
ppc_opc_func_t ppc_opc_vexptefp_func;
ppc_opc_func_t ppc_opc_vmrglw_func;
ppc_opc_func_t ppc_opc_vpkshss_func;
ppc_opc_func_t ppc_opc_vsl_func;
ppc_opc_func_t ppc_opc_vlogefp_func;
ppc_opc_func_t ppc_opc_vpkswss_func;
ppc_opc_func_t ppc_opc_vaddubs_func;
ppc_opc_func_t ppc_opc_vminub_func;
ppc_opc_func_t ppc_opc_vsrb_func;
ppc_opc_func_t ppc_opc_vmuleub_func;
ppc_opc_func_t ppc_opc_vrfin_func;
ppc_opc_func_t ppc_opc_vspltb_func;
ppc_opc_func_t ppc_opc_vupkhsb_func;
ppc_opc_func_t ppc_opc_vadduhs_func;
ppc_opc_func_t ppc_opc_vminuh_func;
ppc_opc_func_t ppc_opc_vsrh_func;
ppc_opc_func_t ppc_opc_vmuleuh_func;
ppc_opc_func_t ppc_opc_vrfiz_func;
ppc_opc_func_t ppc_opc_vsplth_func;
ppc_opc_func_t ppc_opc_vupkhsh_func;
ppc_opc_func_t ppc_opc_vadduws_func;
ppc_opc_func_t ppc_opc_vminuw_func;
ppc_opc_func_t ppc_opc_vsrw_func;
ppc_opc_func_t ppc_opc_vrfip_func;
ppc_opc_func_t ppc_opc_vspltw_func;
ppc_opc_func_t ppc_opc_vupklsb_func;
ppc_opc_func_t ppc_opc_vsr_func;
ppc_opc_func_t ppc_opc_vrfim_func;
ppc_opc_func_t ppc_opc_vupklsh_func;
ppc_opc_func_t ppc_opc_vaddsbs_func;
ppc_opc_func_t ppc_opc_vminsb_func;
ppc_opc_func_t ppc_opc_vsrab_func;
ppc_opc_func_t ppc_opc_vmulesb_func;
ppc_opc_func_t ppc_opc_vcfux_func;
ppc_opc_func_t ppc_opc_vspltisb_func;
ppc_opc_func_t ppc_opc_vpkpx_func;
ppc_opc_func_t ppc_opc_vaddshs_func;
ppc_opc_func_t ppc_opc_vminsh_func;
ppc_opc_func_t ppc_opc_vsrah_func;
ppc_opc_func_t ppc_opc_vmulesh_func;
ppc_opc_func_t ppc_opc_vcfsx_func;
ppc_opc_func_t ppc_opc_vspltish_func;
ppc_opc_func_t ppc_opc_vupkhpx_func;
ppc_opc_func_t ppc_opc_vaddsws_func;
ppc_opc_func_t ppc_opc_vminsw_func;
ppc_opc_func_t ppc_opc_vsraw_func;
ppc_opc_func_t ppc_opc_vctuxs_func;
ppc_opc_func_t ppc_opc_vspltisw_func;
ppc_opc_func_t ppc_opc_vctsxs_func;
ppc_opc_func_t ppc_opc_vupklpx_func;
ppc_opc_func_t ppc_opc_vsububm_func;
ppc_opc_func_t ppc_opc_vavgub_func;
ppc_opc_func_t ppc_opc_vand_func;
ppc_opc_func_t ppc_opc_vmaxfp_func;
ppc_opc_func_t ppc_opc_vslo_func;
ppc_opc_func_t ppc_opc_vsubuhm_func;
ppc_opc_func_t ppc_opc_vavguh_func;
ppc_opc_func_t ppc_opc_vandc_func;
ppc_opc_func_t ppc_opc_vminfp_func;
ppc_opc_func_t ppc_opc_vsro_func;
ppc_opc_func_t ppc_opc_vsubuwm_func;
ppc_opc_func_t ppc_opc_vavguw_func;
ppc_opc_func_t ppc_opc_vor_func;
ppc_opc_func_t ppc_opc_vxor_func;
ppc_opc_func_t ppc_opc_vavgsb_func;
ppc_opc_func_t ppc_opc_vnor_func;
ppc_opc_func_t ppc_opc_vavgsh_func;
ppc_opc_func_t ppc_opc_vsubcuw_func;
ppc_opc_func_t ppc_opc_vavgsw_func;
ppc_opc_func_t ppc_opc_vsububs_func;
ppc_opc_func_t ppc_opc_mfvscr_func;
ppc_opc_func_t ppc_opc_vsum4ubs_func;
ppc_opc_func_t ppc_opc_vsubuhs_func;
ppc_opc_func_t ppc_opc_mtvscr_func;
ppc_opc_func_t ppc_opc_vsum4shs_func;
ppc_opc_func_t ppc_opc_vsubuws_func;
ppc_opc_func_t ppc_opc_vsum2sws_func;
ppc_opc_func_t ppc_opc_vsubsbs_func;
ppc_opc_func_t ppc_opc_vsum4sbs_func;
ppc_opc_func_t ppc_opc_vsubshs_func;
ppc_opc_func_t ppc_opc_vsubsws_func;
ppc_opc_func_t ppc_opc_vsumsws_func;
