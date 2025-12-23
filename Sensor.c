#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <gpiod.h>
#include "Sensor.h"

#define GPIO_CHIP "gpiochip0"
#define TRIG_LINE 23
#define ECHO_LINE 24

static struct gpiod_chip *chip;
static struct gpiod_line *trig;
static struct gpiod_line *echo;

void sensorInit()
{
    chip = gpiod_chip_open_by_name(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return;
    }

    trig = gpiod_chip_get_line(chip, TRIG_LINE);
    echo = gpiod_chip_get_line(chip, ECHO_LINE);

    if (!trig || !echo) {
        perror("gpiod_chip_get_line");
        return;
    }

    gpiod_line_request_output(trig, "trig", 0);
    struct gpiod_line_request_config cfg;
    cfg.consumer = "echo";
    cfg.request_type = GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES;
    cfg.flags = 0;

    if (gpiod_line_request(echo, &cfg, 0) < 0) {
        perror("gpiod_line_request");
        return;
    }


    usleep(100000);
}

float getDistanceCM()
{
    struct gpiod_line_event event;
    struct timespec start, end;
    int ret;

    /* Send trigger pulse (10 µs) */
    gpiod_line_set_value(trig, 1);
    usleep(10);
    gpiod_line_set_value(trig, 0);

    /* Wait for rising edge */
    ret = gpiod_line_event_wait(echo, NULL);
    if (ret <= 0) return -1;

    gpiod_line_event_read(echo, &event);
    if (event.event_type != GPIOD_LINE_EVENT_RISING_EDGE)
        return -1;

    start = event.ts;

    /* Wait for falling edge */
    ret = gpiod_line_event_wait(echo, NULL);
    if (ret <= 0) return -1;

    gpiod_line_event_read(echo, &event);
    if (event.event_type != GPIOD_LINE_EVENT_FALLING_EDGE)
        return -1;

    end = event.ts;

    /* Compute pulse width in seconds */
    double pulse_width =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    /* Convert to distance */
    float distance_cm = pulse_width * 17150.0f;

    /* Required delay between pings */
    usleep(60000);   // 60 ms

    return distance_cm;
}


