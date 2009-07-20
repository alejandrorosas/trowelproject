#include <io.h>
#include "clock.h"

void clock_dco_set(uint16_t freq) {
    switch (freq) {
        case 1:
            DCOCTL = CALDCO_1MHZ;
            BCSCTL1 = CALBC1_1MHZ;
            break;
        case 8:
            DCOCTL = CALDCO_8MHZ;
            BCSCTL1 = CALBC1_8MHZ;
            break;
        case 12:
            DCOCTL = CALDCO_12MHZ;
            BCSCTL1 = CALBC1_12MHZ;
            break;
        case 16:
            DCOCTL = CALDCO_16MHZ;
            BCSCTL1 = CALBC1_16MHZ;
            break;
        default:
            DCOCTL = CALDCO_1MHZ;
            BCSCTL1 = CALBC1_1MHZ;
            break;
    }
    
    BCSCTL3 = LFXT1S_2;
}


void clock_mclk_set(int source, int div) {
    switch (source) {
        case CLOCK_SOURCE_DCO:
            BCSCTL2 = (BCSCTL2 & ~SELM_3) | SELM_DCOCLK;
            break;
        case CLOCK_SOURCE_VLO:
            BCSCTL2 = (BCSCTL2 & ~SELM_3) | SELM_LFXTCLK;
            break;
        default: 
            BCSCTL2 = (BCSCTL2 & ~SELM_3) | SELM_DCOCLK;
            break;
    }
    
    
    BCSCTL2 &= ~DIVM_3;
    switch (div) {
        case 1:
            BCSCTL2 |= DIVM_DIV1;
            break;
        case 2:
            BCSCTL2 |= DIVM_DIV2;
            break;
        case 4:
            BCSCTL2 |= DIVM_DIV4;
            break;
        case 8:
            BCSCTL2 |= DIVM_DIV8;
            break;
    }
}

void clock_smclk_set(int source, int div) {
    switch (source) {
        case CLOCK_SOURCE_DCO:
            BCSCTL2 &= SELS;
            break;
        case CLOCK_SOURCE_VLO:
            BCSCTL2 |= SELS;
            break;
        default: 
            BCSCTL2 &= SELS;
            break;
    }
    
    BCSCTL2 &= ~DIVS_3;
    switch (div) {
        case 1:
            BCSCTL2 |= DIVS_DIV1;
            break;
        case 2:
            BCSCTL2 |= DIVS_DIV2;
            break;
        case 4:
            BCSCTL2 |= DIVS_DIV4;
            break;
        case 8:
            BCSCTL2 |= DIVS_DIV8;
            break;
    }
    
}


void clock_aclk_set(int div) {
    BCSCTL1 &= ~DIVA_3;
    switch (div) {
        case 1:
            BCSCTL1 |= DIVA_DIV1;
            break;
        case 2:
            BCSCTL1 |= DIVA_DIV2;
            break;
        case 4:
            BCSCTL1 |= DIVA_DIV4;
            break;
        case 8:
            BCSCTL1 |= DIVA_DIV8;
            break;
    }
}
