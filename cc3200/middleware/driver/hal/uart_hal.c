//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************


#include <string.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_udma.h"
#include "hw_uart.h"
#include "rom.h"
#include "rom_map.h"
#include "uart.h"
#include "prcm.h"
#include "udma.h"

#include "cc_types.h"
#include "uart_hal.h"

#define UartGetChar(x)	        MAP_UARTCharGet(x)
#define UartPutChar(x,c)        MAP_UARTCharPut(x,c)

struct cc_uart_state{

		struct cc_uart_config uart_config;
        /* reading related parameters */
        uint8_t *rd_buff;
        uint32_t rd_num_wrds;
        uint8_t rd_num_byts;
        uint8_t rd_wrd_size;
        uint32_t rd_wrds_done;
        uint32_t rd_byts_done;
        enum cc_boolean rd_done;

        /* writing related parameters */
        uint8_t *wrt_buff;
        uint32_t wrt_num_wrds;
        uint8_t wrt_num_byts;
        uint8_t wrt_wrd_size;
        uint32_t wrt_wrds_done;
        uint32_t wrt_byts_done;
        enum cc_boolean wrt_done;

};

struct cc_uart_state uart_state[MAX_UART_MODULES];

/* Reset the UART control pool */
static void reset_uart_state()
{
        i32 loop_cnt;

        /* Clear the inuse pointers to indicate availability
         * of all control structure */
        for(loop_cnt = 0; loop_cnt < MAX_UART_MODULES; loop_cnt++) {
                /* Clear the SPI control structure */
                memset(&uart_state[loop_cnt], 0, sizeof(struct cc_uart_state));
        }
}

/*
*   Check if the requested SPI module is already in use. This is
*   acheived by comparing the base address.
*/
static struct cc_uart_state* check_uart_state_inuse(const struct cc_uart_config *uart_config)
{
        i32 loop_cnt;
        struct cc_uart_state *temp_store = NULL;

        /* Check if any of the control structures
         * already refer to the base address as specified in the SPI info
         * structure. If yes return the handle to that structure */
        for(loop_cnt = 0; loop_cnt < MAX_UART_MODULES; loop_cnt++) {
                if(uart_state[loop_cnt].uart_config.module_info.base_addr ==
                        (uart_config->module_info).base_addr) {
                        temp_store = &uart_state[loop_cnt];
                        break;
                }
        }
        return temp_store;
}

/* Get an available UART control structure from the pool */
static struct cc_uart_state* get_available_uart_state()
{
        i32 loop_cnt;
        struct cc_uart_state *temp_store = NULL;

        /* Find an available control structure and return a handle */
        for(loop_cnt = 0; loop_cnt < MAX_UART_MODULES; loop_cnt++) {
                if(0 == uart_state[loop_cnt].uart_config.module_info.base_addr) {
                        temp_store = &uart_state[loop_cnt];
                        break;
                }
        }
        return temp_store;
}

/* Free up the UART control structure */
static i32 free_uart_state(struct cc_uart_state *uart_state_hndl)
{
        i32 loop_cnt;
        i32 ret = -1;

        /* Free up the SPI control structure of the specified handle */
        for(loop_cnt = 0; loop_cnt < MAX_UART_MODULES; loop_cnt++) {
                if(uart_state_hndl == &uart_state[loop_cnt]) {
                        ret = 0;
                        /* Clear the SPI control structure */
                        memset(uart_state_hndl, 0,
                                sizeof(struct cc_uart_state));
                        break;
                }
        }
        return ret;
}

/* UART interrupt handler callback */
void cc_uart_isr(cc_hndl uart_hndl)
{
        uint32_t uiIntStat;
        int8_t temp;
        u32 disable = 0;
        uint32_t i,y= 0;
        struct cc_uart_state *uart_state = (struct cc_uart_state *)uart_hndl;
        struct cc_uart_config *uart_config = (struct cc_uart_config *)&(uart_state->uart_config);
        uint32_t base_addr = uart_config->module_info.base_addr;
        uiIntStat = MAP_UARTIntStatus(base_addr, 1);

        if(uiIntStat & 0x800){
        		MAP_UARTIntClear(base_addr, 0x800);
        	    MAP_UARTIntDisable(base_addr, 0x800);
        		/* Invoke the callback */
        		goto invoke_wr_callback;
        }
        /* for receive timeout case */
        if(uiIntStat & 0x40){
                MAP_UARTIntClear(base_addr, 0x40);
                while(uart_state->rd_num_byts != 0){
                         temp = MAP_UARTCharGetNonBlocking(base_addr);
                         if(-1 != temp){
                                 y = uart_state->rd_wrds_done * uart_state->rd_wrd_size;
                                 uart_state->rd_buff[y + uart_state->rd_byts_done] = temp;
                                (uart_state->rd_byts_done)++;
                                (uart_state->rd_num_byts)--;
                        }else{
                                break;
                        }
                }
                
                if(uart_state->rd_num_byts == 0){
                		uart_state->rd_done = e_true;
                        MAP_UARTIntClear(base_addr, 0x40);
                        MAP_UARTIntDisable(base_addr, 0x40);
                        goto invoke_rd_callback;
                }
        }
        
        /* for receive interrupt(can be fifo trigger or single i8acter) */
        if(uiIntStat & 0x10){
                i = 0;
                MAP_UARTIntClear(base_addr, 0x10);
                y = uart_state->rd_wrds_done * uart_state->rd_wrd_size;
                while(i < uart_state->rd_wrd_size){
                	uart_state->rd_buff[y + i] =
                			MAP_UARTCharGetNonBlocking(base_addr);
                        i++;
                }
                (uart_state->rd_wrds_done)++;
                (uart_state->rd_num_wrds)--;
                if(uart_state->rd_num_wrds == 0){
                        MAP_UARTIntClear(base_addr, 0x10);
                        MAP_UARTIntDisable(base_addr, 0x10);
                        if(uart_state->rd_num_byts == 0){
                        	uart_state->rd_done = e_true;
                        	goto invoke_rd_callback;
                        }else{
                                MAP_UARTIntEnable(base_addr, 0x40);
                        }
                }
        }
        
        /* for transmit interrupt(can be for fifo trigger or single i8acter) */
        if(uiIntStat & 0x20){
                i = 0;
                MAP_UARTIntClear(base_addr, 0x20);
                 y = (uart_config->module_info).fifo_size +
                       (uart_state->wrt_wrds_done * uart_state->wrt_wrd_size);
                if(uart_state->wrt_num_wrds <= 0){
                        while(i < uart_state->wrt_num_byts){
                                MAP_UARTCharPutNonBlocking(base_addr,
                                				uart_state->wrt_buff[y + i]);
                                i++;
                        }
                        uart_state->wrt_num_byts = 0;
                 }else{
                         while(i < uart_state->wrt_wrd_size){
                                MAP_UARTCharPutNonBlocking(base_addr,uart_state->wrt_buff[y + i]);
                                i++;
                         }
                         (uart_state->wrt_wrds_done)++;
                         (uart_state->wrt_num_wrds)--;
                }
                
                if(uart_state->wrt_num_wrds == 0  && uart_state->wrt_num_byts == 0 ){
                	uart_state->wrt_done = e_true;
                        MAP_UARTIntClear(base_addr, 0x20);
                        MAP_UARTIntDisable(base_addr, 0x20);
                        /* Invoke the callback */
                        goto invoke_wr_callback;
                        //UARTIntEnable(base_addr, 0x800);
                }
        }
        
        /* for DMA receive complete */
        if(uiIntStat & 0x10000){
                MAP_UARTIntClear(base_addr, 0x10000);
                cc_uart_control(uart_state, e_start_rx_dma_transfer,
                        		&disable);
                MAP_UARTIntDisable(base_addr, 0x10000);
                uart_state->rd_wrds_done = uart_state->rd_num_wrds;
                uart_state->rd_num_wrds = 0;
                uart_state->rd_done = e_true;
                /* Invoke the callback */
                goto invoke_rd_callback;
        }

        /* for DMA transmit complete */
        if(uiIntStat & 0x20000){
                MAP_UARTIntClear(base_addr, 0x20000);
                cc_uart_control(uart_state, e_start_tx_dma_transfer,
                                        		&disable);
                uart_state->wrt_done = e_true;
                MAP_UARTIntDisable(base_addr, 0x20000);
                //UARTIntEnable(base_addr, 0x800);
                /* Invoke the callback */
                goto invoke_wr_callback;
        }
        return;
invoke_wr_callback:
		if((uart_config->module_info).int_callback) {
					(uart_config->module_info).int_callback(
						uart_config->rtos_hndl, write_complete,
						(void *)&(uart_state->wrt_done),
						NULL);
		}
invoke_rd_callback:
	if((uart_config->module_info).int_callback) {
				(uart_config->module_info).int_callback(
					uart_config->rtos_hndl, read_complete,
					(void *)&(uart_state->rd_done),
					NULL);
	}
}

/* Initialize the UART module */
void cc_uart_configure(const struct cc_uart_config *uart_config)
{
        struct cc_uart_info uart_info;
        uart_info = uart_config->module_info;

        MAP_UARTConfigSetExpClk(uart_info.base_addr,uart_info.inp_clk,
                                uart_info.baud_rate,uart_info.config_reg);

		if(uart_config->fifo_enabled == e_true){
				MAP_UARTFIFOEnable(uart_info.base_addr);
		}else{
			    MAP_UARTFIFODisable(uart_info.base_addr);
		}
		cc_uart_control((void*)uart_config,e_change_rx_fifo_trig,
						(void*)&(uart_config->rx_fifo_trig_level));
		cc_uart_control((void*)uart_config,e_change_rx_fifo_trig,
					    (void*)&(uart_config->tx_fifo_trig_level));

        return;
}

cc31xx_hndl cc_uart_init(const struct cc_uart_config *uart_config)
{
        cc31xx_hndl hndl;
        struct cc_uart_state *uart_state_hndl;

        /* Reset the UART control structure once in the begining */
        reset_uart_state();

        /* Check if the module is already in use */
        uart_state_hndl = check_uart_state_inuse(uart_config);
		if(NULL == uart_state_hndl) {
				/* Acquire a new control structure from the list */
				uart_state_hndl = get_available_uart_state();
				if(NULL == uart_state_hndl) {
						/* Cannot support more modules of this type */
						return NULL;
				}
		} else {
				/* Module already in use. */
				return uart_state_hndl;
		}

        cc_uart_configure(uart_config);

        /* configuring reading related parameters */
        uart_state_hndl->rd_num_wrds = 0;
        uart_state_hndl->rd_num_byts = 0;
        uart_state_hndl->rd_wrd_size = 8;
        uart_state_hndl->rd_wrds_done= 0;
        uart_state_hndl->rd_byts_done = 0;
        uart_state_hndl->rd_done = e_true;

        /* configuring writing related parameters */
        uart_state_hndl->wrt_num_wrds = 0;
        uart_state_hndl->wrt_num_byts = 0;
        uart_state_hndl->wrt_wrd_size = 8;
        uart_state_hndl->wrt_wrds_done = 0;
        uart_state_hndl->wrt_byts_done = 0;
        uart_state_hndl->wrt_done = e_true;

        /* Copy the user selection of SPI parameters */
		memcpy(&uart_state_hndl->uart_config,
				uart_config,
				sizeof(struct cc_uart_config));
        hndl = (cc_hndl)uart_state_hndl;
        return(hndl);
}

/* DeInitialize UART HAL */
i32 cc_uart_deinit(cc_hndl hndl)
{
        struct cc_uart_state *uart_state = (struct cc_uart_state *)hndl;
        i32 retval;

        /* Free up the SPI control structure */
        retval = free_uart_state(uart_state);

        return retval;
}

i32 cc_uart_rd_submit(cc31xx_hndl hndl,uint8_t *rd_data, uint32_t rd_size,
                      void *cookie)
{
        uint32_t base_addr;
        uint32_t i = 0;
        struct cc_uart_state *uart_state = (struct cc_uart_state *)hndl;
        struct cc_uart_config *uart_config = (struct cc_uart_config *)&(uart_state->uart_config);
        base_addr = (uart_config->module_info).base_addr;

        uart_state->rd_buff = rd_data;
        uart_state->rd_wrds_done= 0;
        uart_state->rd_byts_done = 0;
        
        
        switch(uart_config->rx_transfer_mode){
        case e_poll_based:
        	uart_state->rd_done = e_false;
                while(uart_state->rd_done != e_true){
                		uart_state->rd_buff[i] = UartGetChar(base_addr);
                        i++;
                        if(i == rd_size){
                        		uart_state->rd_done = e_true;
                        }
                }
                break;

        case e_int_based: 
        		uart_state->rd_num_wrds = (rd_size/uart_state->rd_wrd_size);
        		uart_state->rd_num_byts = (rd_size%uart_state->rd_wrd_size);
                
        		uart_state->rd_done = e_false;
                MAP_UARTIntClear(base_addr, 0x10);
                MAP_UARTIntEnable(base_addr, 0x10);
                break;

        case e_dma_based:
        		uart_state->rd_num_wrds = (rd_size/uart_state->rd_wrd_size);
        		uart_state->rd_num_byts = (rd_size%uart_state->rd_wrd_size);
        		uart_state->rd_done = e_false;
                MAP_UARTIntClear(base_addr, 0x10000);
                MAP_UARTIntEnable(base_addr, 0x10000);
                break;

        default:
                return -1;
        }
        /* waiting for the read operation to complete */
        while(uart_state->rd_done == e_false);
        return 0;
}

i32 cc_uart_wr_submit(cc31xx_hndl hndl,uint8_t *wrt_data, uint32_t wrt_size,
                      void *cookie)
{
        uint32_t base_addr;
        uint32_t i = 0;
        uint32_t y = 0;
        uint32_t fifo_size;
        struct cc_uart_state *uart_state = (struct cc_uart_state *)hndl;
        struct cc_uart_config *uart_config = (struct cc_uart_config *)&(uart_state->uart_config);
        base_addr = (uart_config->module_info).base_addr;
        
        uart_state->wrt_buff = wrt_data;
        uart_state->wrt_wrds_done= 0;
        uart_state->wrt_byts_done = 0;

        /* waiting for the tx fifo to clear */
        while(e_true == MAP_UARTBusy(base_addr));
        switch(uart_config->tx_transfer_mode){
        case e_poll_based:
        		uart_state->wrt_done = e_false;
                while(uart_state->wrt_done != e_true){
                        MAP_UARTCharPut(base_addr,uart_state->wrt_buff[i]);
                        i++;
                        if(i == wrt_size){
                        		uart_state->wrt_done = e_true;
                        }
                }
                break;

        case e_int_based:
                i = 0;
                fifo_size = (uart_config->module_info).fifo_size;
                
                uart_state->wrt_done = e_false;
                y = (fifo_size < wrt_size)?fifo_size:wrt_size;
                while(i < y){
                        MAP_UARTCharPutNonBlocking(base_addr,
                        						   uart_state->wrt_buff[i]);
                        i++;
                }
                if(i >= wrt_size){
                		uart_state->wrt_done = e_true;
                		//UARTIntClear(base_addr, 0x800);
                		//UARTIntEnable(base_addr, 0x800);
                		/* Invoke the callback */
						if((uart_config->module_info).int_callback) {
									(uart_config->module_info).int_callback(
										uart_config->rtos_hndl, write_complete,
										(void *)&(uart_state->wrt_done),
										NULL);

						}
                        break;
                }
                
                uart_state->wrt_num_wrds = ((wrt_size-i)/uart_state->wrt_wrd_size);
                uart_state->wrt_num_byts = ((wrt_size-i)%uart_state->wrt_wrd_size);
                MAP_UARTIntClear(base_addr, 0x20);
                MAP_UARTIntEnable(base_addr, 0x20);
                break;

        case e_dma_based:
				uart_state->wrt_num_wrds = ((wrt_size)/uart_state->wrt_wrd_size);
				uart_state->wrt_num_byts = ((wrt_size)%uart_state->wrt_wrd_size);
				uart_state->wrt_done = e_false;
                MAP_UARTIntClear(base_addr, 0x20000);
                MAP_UARTIntEnable(base_addr, 0x20000);
                break;
                
        default:
                return -1;
                
        }            
        return 0;

}

i32 cc_uart_control(cc_hndl hndl, enum cc_uart_ctrl_cmd cmd, void *arg)
{
		struct cc_uart_state *uart_state = (struct cc_uart_state *)hndl;
		struct cc_uart_config *uart_config = (struct cc_uart_config *)&(uart_state->uart_config);
		uint32_t base_addr = (uart_config->module_info).base_addr;
        uint32_t flag = HWREG(base_addr + UART_O_FR);
        if(!((flag & UART_FR_TXFE) && (flag & UART_FR_RXFE))){
                return -1;
        }
        switch(cmd){
        case e_rx_change_mode:
                uart_config->rx_transfer_mode = *(enum cc_transfer_mode*)arg;
                break;

        case e_tx_change_mode:
                uart_config->tx_transfer_mode = *(enum cc_transfer_mode*)arg;
                break;

        case e_change_rx_fifo_trig:
                uart_config->rx_fifo_trig_level = *(enum cc_fifo_trig_level*)arg;
                HWREG(base_addr + UART_O_IFLS) &= 0x07;
                switch(uart_config->rx_fifo_trig_level){
                case e_one_eighth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX1_8;
                        break;

                case e_one_fourth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX2_8;
                        break;

                case e_one_half:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX4_8;
                        break;

                case e_three_fourth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX6_8;
                        break;

                case e_seven_eighth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX7_8;
                        break;

                default:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_RX4_8;
                        uart_config->tx_fifo_trig_level = e_one_half;
                        break;
                }
                break;

        case e_change_tx_fifo_trig:
                uart_config->tx_fifo_trig_level = *(enum cc_fifo_trig_level*)arg;
                HWREG(base_addr + UART_O_IFLS) &= 0x38;
                switch(uart_config->tx_fifo_trig_level){
                case e_one_eighth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX1_8;
                        break;

                case e_one_fourth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX2_8;
                        break;

                case e_one_half:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX4_8;
                        break;

                case e_three_fourth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX6_8;
                        break;

                case e_seven_eighth:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX7_8;
                        break;

                default:
                        HWREG(base_addr + UART_O_IFLS) |= UART_FIFO_TX4_8;
                        uart_config->tx_fifo_trig_level = e_one_half;
                        break;
                }
                break;

        case e_enable_disable_fifo:
                uart_config->fifo_enabled = *(enum cc_boolean*)arg;
                if(e_true == uart_config->fifo_enabled){
                        MAP_UARTFIFOEnable(base_addr);
                }else{
                        MAP_UARTFIFODisable(base_addr);
                }
                break;

        case e_context_save:
        		break;

        case e_context_restore:
        		cc_uart_configure(uart_config);
        	    break;

        case e_get_dma_info:
        		break;

        case e_start_tx_dma_transfer:
        		if(*(u32*)arg == 1){
        				MAP_UARTDMAEnable(base_addr, UART_DMA_TX);
        		}else{
        				MAP_UARTDMADisable(base_addr, UART_DMA_TX);
        		}

        	    break;

        case e_start_rx_dma_transfer:
        		if(*(u32*)arg == 1){
        				MAP_UARTDMAEnable(base_addr, UART_DMA_RX);
        		}else{
        				MAP_UARTDMADisable(base_addr, UART_DMA_RX);
        		}

        	    break;

        default:
                return -1;
        }
        return 0;
}
