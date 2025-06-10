#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BLOCK_SIZE (4 * 1024)

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

static volatile unsigned int gpiobase;
static volatile uint32_t *gpio;

void set_input(int pin)
{
    int fsel, shift;

    fprintf(stderr, "Setting pin %d to %d ...\n", pin, INPUT);
    fsel = pin / 10;
    shift = 3 * (pin % 10);
    *(gpio + fsel) = (*(gpio + fsel) & ~(7 << shift));

    return;
}

void set_output(int pin)
{
    int fsel, shift;

    fprintf(stderr, "Setting pin %d to %d ...\n", pin, OUTPUT);
    fsel = pin / 10;
    shift = 3 * (pin % 10);
    *(gpio + fsel) = (*(gpio + fsel) & ~(7 << shift)) | (1 << shift);

    return;
}

void led_on(int pin)
{
    int offset = (pin < 32) ? 7 : 8;
    *(gpio + offset) = 1 << (pin & 31);
    return;
}

void led_off(int pin)
{
    int offset = (pin < 32) ? 10 : 11;
    *(gpio + offset) = 1 << (pin & 31);
    return;
}

void delay(int ms)
{
    struct timespec sleep, dum;
    sleep.tv_sec = (time_t)(ms / 1000);
    sleep.tv_nsec = (long)(ms % 1000) * 1000000;
    nanosleep(&sleep, &dum);
}

void button_press(int pin)
{
    int offset = (pin < 32) ? 13 : 14;
    int value = LOW;

    for (int i = 0; i < 40; i++)
    {
        delay(50);
        if ((*(gpio + offset) & (1 << (26 & 31))) != 0)
            return;
    }

    return;
}

int main(void)
{
    int fd;

    if (geteuid() != 0)
        fprintf(stderr, "setupp: must be root. (Did you forget sudo?)\n");

    gpiobase = 0x3F200000;

    if ((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
        return 1;

    gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                            gpiobase);
    if ((int32_t)gpio == -1)
        return 1;
    else
        fprintf(stderr, "NB: gpio = %x for gpiobase %x\n", gpio, gpiobase);

    set_output(10);
    set_output(13);
    set_input(26);

    fprintf(stdout, "Traffic lights loop\n");
    while (1)
    {
        led_on(10);
        delay(4000);
        led_off(10);
        led_on(13);
        button_press(26);
        delay(2000);
        led_off(13);
    }

    fprintf(stdout, " END\n");
    return 0;
}