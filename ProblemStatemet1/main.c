#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

// ---------------------------- Global Variables
// These values will be updated somewhere else in your program.
uint8_t  G_DataID    = 1;
int32_t  G_DataValue = 42;

// ---------------------------- Data Type
typedef struct {
    uint8_t dataID;
    int32_t DataValue;
} Data_t;

// ---------------------------- Queue and Task Handles
QueueHandle_t Queue1;          // Queue for sending and receiving Data_t items
TaskHandle_t TaskHandle_1;     // Handle for Task 1
TaskHandle_t TaskHandle_2;     // Handle for Task 2

// ---------------------------- Task 1: Send Data Every 500ms
void ExampleTask1(void *pV)
{
    TickType_t lastWakeTime = xTaskGetTickCount();         // Get the current tick count
    const TickType_t delay500ms = pdMS_TO_TICKS(500);      // Convert 500ms to ticks

    while (1)
    {
        // Prepare data using global variables
        Data_t dataToSend;
        dataToSend.dataID = G_DataID;
        dataToSend.DataValue = G_DataValue;

        // Send the data to the queue
        xQueueSend(Queue1, &dataToSend, portMAX_DELAY);

        // Wait exactly 500ms from the last wake time
        vTaskDelayUntil(&lastWakeTime, delay500ms);
    }
}

// ---------------------------- Task 2: Receive and React to Data
void ExampleTask2(void *pV)
{
    Data_t receivedData;
    UBaseType_t originalPriority = uxTaskPriorityGet(NULL);
    BaseType_t isPriorityIncreased = pdFALSE;

    while (1)
    {
        if (xQueueReceive(Queue1, &receivedData, portMAX_DELAY) == pdTRUE)
        {
            printf("Received Data: ID = %d, Value = %ld\n", receivedData.dataID, receivedData.DataValue);

            if (receivedData.dataID == 0)
            {
                // Task deletes itself
                printf("Task deleting itself (dataID == 0)\n");
                vTaskDelete(NULL);
            }
            else if (receivedData.dataID == 1)
            {
                if (receivedData.DataValue == 0 && !isPriorityIncreased)
                {
                    vTaskPrioritySet(NULL, originalPriority + 2);
                    isPriorityIncreased = pdTRUE;
                    printf("Priority increased by 2\n");
                }
                else if (receivedData.DataValue == 1 && isPriorityIncreased)
                {
                    vTaskPrioritySet(NULL, originalPriority);
                    isPriorityIncreased = pdFALSE;
                    printf("Priority restored to original\n");
                }
                else if (receivedData.DataValue == 2)
                {
                    printf("Task deleting itself (DataValue == 2)\n");
                    vTaskDelete(NULL);
                }
            }
        }
    }
}

// ---------------------------- Main Function
void main_app(void)
{
    // Create a queue that can hold 5 items of type Data_t
    Queue1 = xQueueCreate(5, sizeof(Data_t));

    if (Queue1 == NULL)
    {
        printf("Queue creation failed!, Stopping! \n");
        while (1);
    }

    // Create Task 1 (sender task)
    xTaskCreate(ExampleTask1, "SenderTask", 256, NULL, 2, &TaskHandle_1);

    // Create Task 2 (receiver task)
    xTaskCreate(ExampleTask2, "ReceiverTask", 256, NULL, 1, &TaskHandle_2);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach
    printf("Error: Scheduler error\n");
    while (1);
}
