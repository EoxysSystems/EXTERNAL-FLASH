/*
 * freertos_user.c
 *
 *  Created on: 24-Feb-2023
 *  Author: unni0
 */
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/*
 * CMSIS clock configuration function.
 */
void vApplicationMallocFailedHook(void)__attribute__((noreturn));
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)__attribute__((noreturn));
void vApplicationTickHook(void);
/*-----------------------------------------------------------------------------------------*/
void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    //__BKPT();

    //printf("Stack overflow task name=%s\n", pcTaskName);

    for(;;);

}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()).  The code in this
    tick hook implementation is for demonstration only - it has no real
    purpose.  It just gives a semaphore every 50ms.  The semaphore unblocks a
    task that then toggles an LED.  Additionally, the call to
    vQueueSetAccessQueueSetFromISR() is part of the "standard demo tasks"
    functionality. */
}
/*-----------------------------------------------------------*/

#ifdef JUST_AN_EXAMPLE_ISR

void Dummy_IRQHandler(void)
{
    long lHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt if necessary. */
    Dummy_ClearITPendingBit();

    /* This interrupt does nothing more than demonstrate how to synchronise a
    task with an interrupt.  A semaphore is used for this purpose.  Note
    lHigherPriorityTaskWoken is initialised to zero. Only FreeRTOS API functions
    that end in "FromISR" can be called from an ISR. */
    xSemaphoreGiveFromISR(xTestSemaphore, &lHigherPriorityTaskWoken);

    /* If there was a task that was blocked on the semaphore, and giving the
    semaphore caused the task to unblock, and the unblocked task has a priority
    higher than the current Running state task (the task that this interrupt
    interrupted), then lHigherPriorityTaskWoken will have been set to pdTRUE
    internally within xSemaphoreGiveFromISR().  Passing pdTRUE into the
    portEND_SWITCHING_ISR() macro will result in a context switch being pended to
    ensure this interrupt returns directly to the unblocked, higher priority,
    task.  Passing pdFALSE into portEND_SWITCHING_ISR() has no effect. */
    portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);
}

#endif /* JUST_AN_EXAMPLE_ISR */



