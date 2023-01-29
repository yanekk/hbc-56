#include "acutest.h"
#include <stdint.h>
#include "devices/via/via.h"

void test_create_timer1StateIsStoppedByDefault(void)
{
    // arrange
    VIA* via = VIA_New();

    // act
    VIATimerState actualState = VIA_Timer1_State(via);

    // assert 
    TEST_CHECK_(actualState == Stopped, 
        "Expected timer state to be Stopped, got %d", actualState);

    // cleanup
    VIA_Free(via);
}

void test_set_timer1CanBeSet(void)
{
    // arrange
    uint16_t expectedTimerValue = 0xFAAD;
    VIA* via = VIA_New();

    // act
    VIA_Timer1_Set(via, expectedTimerValue);

    // assert 
    uint16_t actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(actualTimerValue == expectedTimerValue, 
        "Expected timer value to be %x, got %x", expectedTimerValue, actualTimerValue);

    // cleanup
    VIA_Free(via);
}

void test_decrement_timer1IsNotDecrementedInfNotStarted(void)
{
    // arrange
    uint16_t initialTimerValue = 0xCDDA;
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, initialTimerValue);

    // act
    VIA_Timer1_Decrement(via);
    
    // assert 
    uint16_t actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(actualTimerValue == initialTimerValue, "Expected timer value to be %x, got %x", initialTimerValue, actualTimerValue);

    // cleanup
    VIA_Free(via);
}

void test_state_timer1StateIsRunningIfStarted(void)
{
    // arrange
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, 0xFAAD);
    VIA_Timer1_Start(via);

    // act
    VIATimerState actualState = VIA_Timer1_State(via);
    
    // assert 
    TEST_CHECK_(actualState == Running, 
        "Expected timer state to be Running, got %d", actualState);

    // cleanup
    VIA_Free(via);
}

void test_decrement_timer1CanBeDecrementedIfStarted(void)
{
    // arrange
    uint16_t initialTimerValue = 0xCDDA;
    uint16_t expectedTimerValue = initialTimerValue - 1;
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, initialTimerValue);

    // act
    VIA_Timer1_Start(via);
    VIA_Timer1_Decrement(via);
    
    // assert 
    uint16_t actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(actualTimerValue == expectedTimerValue, "Expected timer value to be %x, got %x", expectedTimerValue, actualTimerValue);

    // cleanup
    VIA_Free(via);
}

void test_reset_timer1CanBeReset(void)
{
    // arrange
    uint16_t initialTimerValue = 0xDAAF;
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, initialTimerValue);
    VIA_Timer1_Start(via);
    VIA_Timer1_Decrement(via);

    // act
    VIA_Timer1_Reset(via);

    // assert 
    uint16_t actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(actualTimerValue == initialTimerValue, 
        "Expected timer value to be %x, got %x", initialTimerValue, actualTimerValue);
    TEST_CHECK_(VIA_Timer1_State(via) == Stopped, 
        "Expected reset timer state to be Stopped, got Running");

    // cleanup
    VIA_Free(via);
}

void test_decrement_timer1CannotBeOverflown(void)
{
    // arrange
    uint16_t initialTimerValue = 1;
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, initialTimerValue);
    VIA_Timer1_Start(via);

    // act & assert
    VIA_Timer1_Decrement(via); 
    uint16_t actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(VIA_Timer1_Get(via) == 0, "Expected timer value to be %x, got %x", initialTimerValue, actualTimerValue);

    VIA_Timer1_Decrement(via); 
    actualTimerValue = VIA_Timer1_Get(via);
    TEST_CHECK_(VIA_Timer1_Get(via) == 0, "Expected timer value to be %x, got %x", initialTimerValue, actualTimerValue);

    // cleanup
    VIA_Free(via);
}

void test_decrement_timer1IsStoppedOnZero(void)
{
    // arrange
    uint16_t initialTimerValue = 1;
    VIA* via = VIA_New();
    VIA_Timer1_Set(via, initialTimerValue);
    VIA_Timer1_Start(via);

    // act 
    VIA_Timer1_Decrement(via); 

    // assert
    VIATimerState actualState = VIA_Timer1_State(via);
    TEST_CHECK_(VIA_Timer1_State(via) == Stopped, 
        "Expected timer state to be Stopped, got %d", actualState);

    // cleanup
    VIA_Free(via);
}

void test_setMode_timer1ModeIsSet(void)
{
    // arrange
    VIA* via = VIA_New();

    // act 
    VIA_Timer1_SetMode(via, OneShot); 

    // assert
    VIATimerMode actualMode = VIA_Timer1_GetMode(via);
    TEST_CHECK_(actualMode == OneShot, 
        "Expected timer mode to be OneShot, got %d", actualMode);

    // cleanup
    VIA_Free(via);
}

void test_timer1PB7Output_enable(void)
{
    // arrange
    VIA* via = VIA_New();

    // act 
    VIA_Timer1_SetPB7Output(via, true); 

    // assert
    TEST_CHECK_(VIA_Timer1_GetPB7Output(via) == true, 
        "Expected PB7 to be enabled, is disabled");

    // cleanup
    VIA_Free(via);
}

void test_timer1PB7Output_disable(void)
{
    // arrange
    VIA* via = VIA_New();

    // act 
    VIA_Timer1_SetPB7Output(via, false); 

    // assert
    TEST_CHECK_(VIA_Timer1_GetPB7Output(via) == false, 
        "Expected PB7 output to be disabled, is enabled");

    // cleanup
    VIA_Free(via);
}

void test_timer1Interrupt_enable(void)
{
    // arrange
    VIA* via = VIA_New();

    // act 
    VIA_Timer1_SetInterrupt(via, true); 

    // assert
    TEST_CHECK_(VIA_Timer1_GetInterrupt(via) == true, 
        "Expected interrupt to be enabled, is disabled");

    // cleanup
    VIA_Free(via);
}

void test_timer1Interrupt_disable(void)
{
    // arrange
    VIA* via = VIA_New();

    // act 
    VIA_Timer1_SetInterrupt(via, false); 

    // assert
    TEST_CHECK_(VIA_Timer1_GetInterrupt(via) == false, 
        "Expected interrupt to be disabled, is enabled");

    // cleanup
    VIA_Free(via);
}

TEST_LIST = {
   { "test_create_timer1StateIsStoppedByDefault", test_create_timer1StateIsStoppedByDefault },
   { "test_set_timer1CanBeSet", test_set_timer1CanBeSet },
   { "test_decrement_timer1IsNotDecrementedInfNotStarted", test_decrement_timer1IsNotDecrementedInfNotStarted },
   { "test_state_timer1StateIsRunningIfStarted", test_state_timer1StateIsRunningIfStarted },
   { "test_decrement_timer1CanBeDecrementedIfStarted", test_decrement_timer1CanBeDecrementedIfStarted },
   { "test_reset_timer1CanBeReset", test_reset_timer1CanBeReset },
   { "test_decrement_timer1CannotBeOverflown", test_decrement_timer1CannotBeOverflown },
   { "test_decrement_timer1IsStoppedOnZero", test_decrement_timer1IsStoppedOnZero },
   { "test_setMode_timer1ModeIsSet", test_setMode_timer1ModeIsSet },
   
   { "test_timer1PB7Output_enable", test_timer1PB7Output_enable },
   { "test_timer1PB7Output_disable", test_timer1PB7Output_disable },

   { "test_timer1Interrupt_enable", test_timer1Interrupt_enable },
   { "test_timer1Interrupt_disable", test_timer1Interrupt_disable },

   { NULL, NULL }     /* zeroed record marking the end of the list */
};