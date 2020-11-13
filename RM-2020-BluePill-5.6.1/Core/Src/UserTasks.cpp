/**
 * @file UserTasks.cpp
 * @author Will (phliuab@connect.ust.hk)
 * @brief File storing all user tasks
 * @version 0.1
 * @date 2020-09-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <can.h>

#include <Protocol.hpp>

#include "FreeRTOS.h"
#include "main.h"
#include "string.h"
#include "task.h"
#include "usart.h"

StaticTask_t xBlinkTaskTCB;
StackType_t uxBlinkTaskStack[64];
void blinky(void *param)
{
    while (true)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        vTaskDelay(200);
    }
}

StaticTask_t uartTaskTCB;
StackType_t uartTaskStack[128];
static volatile uint8_t msg[] = "IKEMURA";
static volatile uint8_t msg2[] = "KEI";
void CVCommunicationTask(void *param)
{
    HAL_UART_Init(&huart1);
    uint16_t strLen = strlen((char *)msg);
    uint16_t strLen2 = strlen((char *)msg2);

    while (true)
    {
        Protocol::sendCustomInfo((uint8_t *)msg2, strLen2);
        vTaskDelay(800);
        Protocol::sendCustomInfo((uint8_t *)msg, strLen);
    }
}

StaticTask_t canTaskTCB;
StackType_t canTaskStack[128];
uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

void canTask(void *param)
{
    CAN_TxHeaderTypeDef header = {0, 0, CAN_ID_STD, CAN_RTR_DATA, 8, DISABLE};
    HAL_CAN_Start(&hcan);
    uint32_t mailbox;

    while (1)
    {
        HAL_CAN_AddTxMessage(&hcan, &header, data, &mailbox);
        vTaskDelay(1);
    }
}

extern "C"
{
    void startUserTasks()
    {
        xTaskCreateStatic(
            blinky, "blink", 64, NULL, 0, uxBlinkTaskStack, &xBlinkTaskTCB);

        xTaskCreateStatic(CVCommunicationTask,
                          "serial",
                          128,
                          NULL,
                          0,
                          uartTaskStack,
                          &uartTaskTCB);

        xTaskCreateStatic(
            canTask, "can", 128, NULL, 0, canTaskStack, &canTaskTCB);
    }
}
