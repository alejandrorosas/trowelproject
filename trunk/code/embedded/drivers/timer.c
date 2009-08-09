#include <io.h>
#include <signal.h>
#include "timer.h"

static timer_cb timer_callbacks[TIMER_ALARM_NUMBER+1];
static uint16_t timer_periods[TIMER_ALARM_NUMBER];
static uint16_t *TACCTLx = (uint16_t*) 0x162;
static uint16_t *TACCRx  = (uint16_t*) 0x172;

void timer_init() {
    uint16_t i;
    
    // stop everything
    TACTL = TACLR;
    
    // clear the CCR and CCTL registers, and the associated callbacks
    for (i=0;i<TIMER_ALARM_NUMBER;i++)
    {
        TACCTLx[i] = 0;
        TACCRx[i] = 0;
        timer_callbacks[i] = 0x0;
        timer_periods[i] = 0;
    }
    // clear the overflow callback
    timer_callbacks[TIMER_ALARM_NUMBER+1] = 0x0;
}

uint16_t timer_start(int source, int div) {
    // clear
    TACTL = TACLR;
    
    switch (source) {
        case TIMER_SOURCE_ACLK:
            TACTL |= TASSEL_ACLK;
            break;
        case TIMER_SOURCE_SMCLK:
            TACTL |= TASSEL_SMCLK;
            break;
        default:
            return 0;
    }
    
    switch (div) {
        case 1:
            TACTL |= ID_DIV1;
            break;
        case 2:
            TACTL |= ID_DIV2;
            break;
        case 4:
            TACTL |= ID_DIV4;
            break;
        case 8:
            TACTL |= ID_DIV8;
            break;
        default:
            return 1;
    }
    
    // start timer
    TACTL |= MC_CONT;
    
    return 1;
}

uint16_t timer_register_cb (int alarm, timer_cb cb) {
    if (alarm >= TIMER_ALARM_NUMBER+1)
        return 0;
    
    timer_callbacks[alarm] = cb;
    
    if (alarm == TIMER_ALARM_OVERFLOW)
        TACTL = (TACTL & ~TAIE) & (cb==0x0?0:TAIE);
    
    return 1;
}

uint16_t timer_time()
{
    return TAR;
}

uint16_t timer_set_alarm(int alarm, uint16_t delay, uint16_t period, int mode, uint16_t ref)
{
    uint16_t now;
    now = TAR;
    
    if (alarm > TIMER_ALARM_NUMBER)
        return 0;
    
    switch (mode) {
        case TIMER_MODE_FROM_NOW:
            TACCRx[alarm] = now + delay;
            break;
        case TIMER_MODE_FROM_REF:
            TACCRx[alarm] = ref + delay;
            break;
        default:
            return 0;
    }
    
    TACCTLx[alarm] = CCIE;
    timer_periods[alarm] = period;
    
    TACCTLx[alarm] = CCIE;
    timer_periods[alarm] = period;
    return 1;
}

uint16_t timer_unset_alarm(int alarm)
{
    if (alarm > TIMER_ALARM_NUMBER)
        return 0;
    
    TACCRx[alarm] = 0;
    TACCTLx[alarm] = 0;
    timer_periods[alarm] = 0;
    timer_callbacks[alarm] = 0;
    
    return 1;
}

void timer_stop()
{
    TACTL = TACLR;
}

void timer0irq(void);
interrupt (TIMERA0_VECTOR) timer0irq( void )
{
    if (timer_periods[0])
        TACCRx[0] += timer_periods[0];
    else {
        TACCRx[0] = 0;
        TACCTLx[0] = 0;
    }
    
    if (timer_callbacks[0] && timer_callbacks[0]() )
        LPM4_EXIT;
}

void timer1irq(void);
interrupt (TIMERA1_VECTOR) timer1irq( void )
{
    uint16_t alarm;
    
    alarm = TAIV >> 1;
    
    if (alarm == 0x05) {
        if (timer_callbacks[TIMER_ALARM_OVERFLOW] && timer_callbacks[TIMER_ALARM_OVERFLOW]() )
            LPM4_EXIT;
    } else {
        if (timer_periods[alarm])
            TACCRx[alarm] += timer_periods[alarm];
        else {
            TACCRx[alarm] = 0;
            TACCTLx[alarm] = 0;
        }
        
        if (timer_callbacks[alarm] && timer_callbacks[alarm]() )
            LPM4_EXIT;
    }
}
