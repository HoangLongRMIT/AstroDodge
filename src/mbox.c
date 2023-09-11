// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "gpio.h"
#include "uart.h"
#include "../gcclib/stdarg.h"
/* Mailbox Data Buffer (each element is 32-bit)*/
/*
* The keyword attribute allows you to specify special attributes
*
* The aligned(N) attribute aligns the current data item on an address
* which is a multiple of N, by inserting padding bytes before the data item
*
* __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
*
*
* We must ensure that our our buffer is located at a 16 byte aligned address,
* so only the high 28 bits contain the address
* (last 4 bits is ZERO due to 16 byte alignment)
*
*/
volatile unsigned int __attribute__((aligned(16))) mBuf[36];
/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ((res & 0xF) != channel);
    return res;
}
/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (MBOX1_STATUS & MBOX_FULL);
    // send the message
    MBOX1_WRITE = msg;
}
/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    // Check Buffer Address
    uart_puts("Buffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');
    // Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);
    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n");
        return (mBuf[1] == MBOX_RESPONSE);
    }
    return 0;
}

void mbox_buffer_setup(unsigned int buffer_addr, unsigned int tag_identifier, unsigned int **res_data, unsigned int req_length, unsigned int res_length, ...)
{
    va_list variable_args;
    va_start(variable_args, res_length); // Start va_list with the last known parameter

    unsigned int *buffer_ptr = (unsigned int *)buffer_addr;
    const unsigned int msg_size = 8 * 4;

    buffer_ptr[0] = msg_size;     // Message Buffer Size in bytes (8 elements * 4 bytes each)
    buffer_ptr[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    buffer_ptr[2] = tag_identifier;
    buffer_ptr[3] = (req_length >= res_length) ? req_length : res_length; // Value buffer size

    for (int i = 0; i < 4; i++)
    {
        buffer_ptr[i + 4] = (i == 0) ? 0 : va_arg(variable_args, unsigned int); // Set buffer elements
    }
    buffer_ptr[7] = MBOX_TAG_LAST;

    for (int i = 0; i < req_length / 4; i++)
    {
        buffer_ptr[i + 8] = va_arg(variable_args, unsigned int); // Set the remaining buffer elements
    }

    va_end(variable_args);

    *res_data = &buffer_ptr[5];
}
