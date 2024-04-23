#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

int main(int argc, char *argv[]) {
    unsigned char *mem_base;
    uint32_t val_line=5;
    int i;
    printf("Hello world\n");

    sleep(1);

    /*
     * Setup memory mapping which provides access to the peripheral
     * registers region of RGB LEDs, knobs and line of yellow LEDs.
     */
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    /* If mapping fails exit with error code */
    if (mem_base == NULL)
        exit(1);

    for (i=0; i<30; i++) {
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
        val_line<<=1;
        printf("LED val 0x%x\n", val_line);
        sleep(1);
    }

    printf("Goodbye world\n");

    return 0;
}