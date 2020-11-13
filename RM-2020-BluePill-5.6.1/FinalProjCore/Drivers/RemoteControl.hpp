#include <FreeRTOS.h>
#include <usart.h>

namespace RemoteControl
{
void init();

uint16_t getChannelValue(uint8_t channelID);

}  // namespace RemoteControl
