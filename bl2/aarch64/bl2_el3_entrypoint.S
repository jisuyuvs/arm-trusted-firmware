/*
 * Copyright (c) 2017-2021, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

#include <arch.h>
#include <asm_macros.S>
#include <common/bl_common.h>
#include <el3_common_macros.S>

	.globl	bl2_entrypoint
	.globl	bl2_el3_run_image
	.globl	bl2_run_next_image

#if BL2_IN_XIP_MEM
#define FIXUP_SIZE	0
#else
#define FIXUP_SIZE	((BL2_LIMIT) - (BL2_BASE))
#endif

func bl2_entrypoint
	/* Save arguments x0-x3 from previous Boot loader */
	mov	x20, x0
	mov	x21, x1
	mov	x22, x2
	mov	x23, x3

	/* Clear TCR_EL1 */
	mov	x0, #0x0
	msr	tcr_el1, x0

	el3_entrypoint_common                                   \
		_init_sctlr=1                                   \
		_warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS  \
		_secondary_cold_boot=!COLD_BOOT_SINGLE_CPU      \
		_init_memory=1                                  \
		_init_c_runtime=1                               \
		_exception_vectors=bl2_el3_exceptions		\
		_pie_fixup_size=FIXUP_SIZE

	/* ---------------------------------------------
	 * Restore parameters of boot rom
	 * ---------------------------------------------
	 */
	mov	x0, x20
	mov	x1, x21
	mov	x2, x22
	mov	x3, x23

	/* ---------------------------------------------
	 * Perform BL2 setup
	 * ---------------------------------------------
	 */
	bl	bl2_el3_setup

#if ENABLE_PAUTH
	/* ---------------------------------------------
	 * Program APIAKey_EL1 and enable pointer authentication.
	 * ---------------------------------------------
	 */
	bl	pauth_init_enable_el3
#endif /* ENABLE_PAUTH */

	/* ---------------------------------------------
	 * Jump to main function.
	 * ---------------------------------------------
	 */
	bl	bl2_main

	/* ---------------------------------------------
	 * Should never reach this point.
	 * ---------------------------------------------
	 */
	no_ret	plat_panic_handler
endfunc bl2_entrypoint

func bl2_run_next_image
	mov	x20,x0
	/* ---------------------------------------------
	 * MMU needs to be disabled because both BL2 and BL31 execute
	 * in EL3, and therefore share the same address space.
	 * BL31 will initialize the address space according to its
	 * own requirement.
	 * ---------------------------------------------
	 */
	bl	disable_mmu_icache_el3
	tlbi	alle3
	bl	bl2_el3_plat_prepare_exit

#if ENABLE_PAUTH
	/* ---------------------------------------------
	 * Disable pointer authentication before jumping
	 * to next boot image.
	 * ---------------------------------------------
	 */
	bl	pauth_disable_el3
#endif /* ENABLE_PAUTH */

	ldp	x0, x1, [x20, #ENTRY_POINT_INFO_PC_OFFSET]
	msr	elr_el3, x0
	msr	spsr_el3, x1

	ldp	x6, x7, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x30)]
	ldp	x4, x5, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x20)]
	ldp	x2, x3, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x10)]
	ldp	x0, x1, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x0)]
	exception_return
endfunc bl2_run_next_image
