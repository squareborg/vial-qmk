/*
 * This software is experimental and a work in progress.
 * Under no circumstances should these files be used in relation to any critical system(s).
 * Use of these files is at your own risk.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This files are free to use from https://github.com/rogerclarkmelbourne/Arduino_STM32 and
 * https://github.com/leaflabs/libmaple
 *
 * Modifications for QMK and STM32F303 by Yiancar
 */

#if defined(EEPROM_EMU_STM32F303xC)
#    define STM32F303xC
#    include "stm32f3xx.h"
#elif defined(EEPROM_EMU_STM32F103xB)
#    define STM32F103xB
#    include "stm32f1xx.h"
#elif defined(EEPROM_EMU_STM32F072xB)
#    define STM32F072xB
#    include "stm32f0xx.h"
#elif defined(EEPROM_EMU_STM32F042x6)
#    define STM32F042x6
#    include "stm32f0xx.h"
#else
#    error "not implemented."
#endif

#include "flash_stm32.h"

#if defined(EEPROM_EMU_STM32F103xB)
#    define FLASH_SR_WRPERR FLASH_SR_WRPRTERR
#endif

/**
 * @brief  Erases a specified FLASH page.
 * @param  Page_Address: The page address to be erased.
 * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address) {
    FLASH_Status status = FLASH_COMPLETE;
    /* Check the parameters */
    ASSERT(IS_FLASH_ADDRESS(Page_Address));
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if (status == FLASH_COMPLETE) {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CR |= FLASH_CR_PER;
        FLASH->AR = Page_Address;
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(EraseTimeout);
        if (status != FLASH_TIMEOUT) {
            /* if the erase operation is completed, disable the PER Bit */
            FLASH->CR &= ~FLASH_CR_PER;
        }
        FLASH->SR = (FLASH_SR_EOP | FLASH_SR_PGERR | FLASH_SR_WRPERR);
    }
    /* Return the Erase Status */
    return status;
}

/**
 * @brief  Programs a half word at a specified address.
 * @param  Address: specifies the address to be programmed.
 * @param  Data: specifies the data to be programmed.
 * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
 *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data) {
    FLASH_Status status = FLASH_BAD_ADDRESS;

    if (IS_FLASH_ADDRESS(Address)) {
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(ProgramTimeout);
        if (status == FLASH_COMPLETE) {
            /* if the previous operation is completed, proceed to program the new data */
            FLASH->CR |= FLASH_CR_PG;
            *(__IO uint16_t*)Address = Data;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(ProgramTimeout);
            if (status != FLASH_TIMEOUT) {
                /* if the program operation is completed, disable the PG Bit */
                FLASH->CR &= ~FLASH_CR_PG;
            }
            FLASH->SR = (FLASH_SR_EOP | FLASH_SR_PGERR | FLASH_SR_WRPERR);
        }
    }
    return status;
}

/**
 * @brief  Programs a full word at a specified address.
 * @param  Address: specifies the address to be programmed.
 * @param  Data: specifies the data to be programmed.
 * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
 *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ProgramFullWord(uint32_t Address, uint32_t Data) {
	FLASH_Status status;
	status = FLASH_ProgramHalfWord(Address, Data & 0xFF);
	if (status == FLASH_COMPLETE) {
		status = FLASH_ProgramHalfWord(Address + 2, Data >> 16);
	}
	return status;
}

void FLASH_Init(void) { }
