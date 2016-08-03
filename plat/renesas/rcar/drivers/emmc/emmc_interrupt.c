/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @file  emmc_interrupt.c
 * @brief interrupt service for MMC boot driver.
 *
 */

#include <stddef.h>
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"
/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */
static EMMC_ERROR_CODE emmc_trans_sector (uint32_t *buff_address_virtual);


/* ********************************* CODE ********************************** */



/** @brief emmc driver interrupt service routine.
 *
 * - Pre-conditions:<BR>
 * Must be block emmc driver state machine.
 * - Post-conditions:<BR>
 * unblocking emmc driver state machine.
 * 
 * @retval INT_SUCCESS
 */
uint32_t emmc_interrupt(void)
{
    EMMC_ERROR_CODE result;

	/* SD_INFO */
	mmc_drv_obj.error_info.info1 = GETR_32(SD_INFO1);
	mmc_drv_obj.error_info.info2 = GETR_32(SD_INFO2);

	/* SD_INFO EVENT */
    mmc_drv_obj.int_event1 = mmc_drv_obj.error_info.info1 & GETR_32(SD_INFO1_MASK);
    mmc_drv_obj.int_event2 = mmc_drv_obj.error_info.info2 & GETR_32(SD_INFO2_MASK);

	/* ERR_STS */
	mmc_drv_obj.error_info.status1 = GETR_32(SD_ERR_STS1);
	mmc_drv_obj.error_info.status2 = GETR_32(SD_ERR_STS2);

	/* DM_CM_INFO */
	mmc_drv_obj.error_info.dm_info1 = GETR_32(DM_CM_INFO1);
	mmc_drv_obj.error_info.dm_info2 = GETR_32(DM_CM_INFO2);

	/* DM_CM_INFO EVENT */
    mmc_drv_obj.dm_event1 = mmc_drv_obj.error_info.dm_info1 & GETR_32(DM_CM_INFO1_MASK);
    mmc_drv_obj.dm_event2 = mmc_drv_obj.error_info.dm_info2 & GETR_32(DM_CM_INFO2_MASK);

/* ERR SD_INFO2 */
	if( (SD_INFO2_ALL_ERR & mmc_drv_obj.int_event2) != 0 )
    {
        SETR_32(SD_INFO1_MASK, 0x00000000U);   /* interrupt disable */
        SETR_32(SD_INFO2_MASK, SD_INFO2_CLEAR );   /* interrupt disable */
        SETR_32(SD_INFO1, 0x00000000U);        /* interrupt clear */
        SETR_32(SD_INFO2, SD_INFO2_CLEAR );    /* interrupt clear */
        mmc_drv_obj.state_machine_blocking = FALSE;
    }

/* PIO Transfer */
	/* BWE/BRE */
	else if((( SD_INFO2_BWE | SD_INFO2_BRE ) & mmc_drv_obj.int_event2 ) != 0 )
    {
    	/* BWE */
    	if( SD_INFO2_BWE & mmc_drv_obj.int_event2 )
    	{
        	SETR_32(SD_INFO2, (GETR_32(SD_INFO2) & ~SD_INFO2_BWE) );    /* interrupt clear */
    	}
    	/* BRE */
    	else
    	{
        	SETR_32(SD_INFO2, (GETR_32(SD_INFO2) & ~SD_INFO2_BRE) );    /* interrupt clear */
    	}

    	result = emmc_trans_sector((uint32_t *)mmc_drv_obj.buff_address_virtual);                 /* sector R/W */
        mmc_drv_obj.buff_address_virtual += EMMC_BLOCK_LENGTH;
    	mmc_drv_obj.remain_size -= EMMC_BLOCK_LENGTH;

        if(result != EMMC_SUCCESS)
        {
            /* data transfer error */
            emmc_write_error_info(EMMC_FUNCNO_NONE, result);

            /* Panic */
            SETR_32(SD_INFO1_MASK, 0x00000000U);   /* interrupt disable */
            SETR_32(SD_INFO2_MASK, SD_INFO2_CLEAR );   /* interrupt disable */
            SETR_32(SD_INFO1, 0x00000000U);    /* interrupt clear */
            SETR_32(SD_INFO2, SD_INFO2_CLEAR );    /* interrupt clear */
            mmc_drv_obj.force_terminate = TRUE;
        }
    	else
    	{
			mmc_drv_obj.during_transfer = FALSE;
        }
        mmc_drv_obj.state_machine_blocking = FALSE;
    }

/* DMA_TRANSFER */
	/* DM_CM_INFO1: DMA-ch0 transfer complete or error occured */
	else if( (BIT16 & mmc_drv_obj.dm_event1) != 0 )
	{  
		SETR_32(DM_CM_INFO1, 0x00000000U);
		SETR_32(DM_CM_INFO2, 0x00000000U);
		SETR_32(SD_INFO2, (GETR_32(SD_INFO2) & ~SD_INFO2_BWE) );    /* interrupt clear */
		/* DM_CM_INFO2:  DMA-ch0 error occured */
		if( ( BIT16 & mmc_drv_obj.dm_event2 ) != 0 )
		{
			mmc_drv_obj.dma_error_flag = TRUE;
		}
		else
		{
			mmc_drv_obj.during_dma_transfer = FALSE;
			mmc_drv_obj.during_transfer = FALSE;
		}
        mmc_drv_obj.state_machine_blocking = FALSE;		/* wait next interrupt */
	}
	/* DM_CM_INFO1: DMA-ch1 transfer complete or error occured */
	else if( (BIT17 & mmc_drv_obj.dm_event1) != 0 )
	{
		SETR_32(DM_CM_INFO1, 0x00000000U);
		SETR_32(DM_CM_INFO2, 0x00000000U);
		SETR_32(SD_INFO2, (GETR_32(SD_INFO2) & ~SD_INFO2_BRE) );    /* interrupt clear */
		/* DM_CM_INFO2: DMA-ch1 error occured */
		if( ( BIT17 & mmc_drv_obj.dm_event2 ) != 0 )
		{
			mmc_drv_obj.dma_error_flag = TRUE;
		}
		else
		{
			mmc_drv_obj.during_dma_transfer = FALSE;
			mmc_drv_obj.during_transfer = FALSE;
		}
        mmc_drv_obj.state_machine_blocking = FALSE;		/* wait next interrupt */
	}

	/* Response end  */
    else if( (SD_INFO1_INFO0 & mmc_drv_obj.int_event1) != 0)
	{
		SETR_32(SD_INFO1, (GETR_32(SD_INFO1) & ~SD_INFO1_INFO0) );    /* interrupt clear */
        mmc_drv_obj.state_machine_blocking = FALSE;
	}
	/* Access end  */
    else if( (SD_INFO1_INFO2 & mmc_drv_obj.int_event1) != 0)
	{
		SETR_32(SD_INFO1, (GETR_32(SD_INFO1) & ~SD_INFO1_INFO2) );    /* interrupt clear */
        mmc_drv_obj.state_machine_blocking = FALSE;
	}
    else
    {
    	/* nothing to do. */
    }

    return (uint32_t)0;
}

/** @brief Data transfer function with PIO (Single sector).
 *
 * - Pre-conditions:<BR>
 * Called from interrupt service.
 * - Post-conditions:<BR>
 * .
 * 
 * @param[in,out] buff_address_virtual Dest/Src buffer address(virtual).
 * @retval EMMC_SUCCESS successful.
 * @retval EMMC_ERR_PARAM parameter error.
 * @retval EMMC_ERR_STATE state error.
 */
static EMMC_ERROR_CODE emmc_trans_sector (
    uint32_t *buff_address_virtual
    )
{
	uint32_t length,i;
	uint64_t *bufPtrLL;

    if (buff_address_virtual == NULL)
    {
        return EMMC_ERR_PARAM;
    }

    if ((mmc_drv_obj.during_transfer != TRUE) || (mmc_drv_obj.remain_size == 0) )
    {
        return EMMC_ERR_STATE;
    }

	bufPtrLL = (uint64_t*)buff_address_virtual;
	length = mmc_drv_obj.remain_size;

	/* data transefer */
	for (i=0; i<(length>>3);i++)
    {
		/* Write */
	    if (mmc_drv_obj.cmd_info.dir == HAL_MEMCARD_WRITE)
	    {
	        SETR_64(SD_BUF0, *bufPtrLL);     /* buffer --> FIFO */
		}
    	/* Read */
		else
		{
            *bufPtrLL = GETR_64(SD_BUF0);    /* FIFO --> buffer */
        }
        bufPtrLL++;
	}

    return EMMC_SUCCESS;
}

/* ******************************** END ************************************ */

