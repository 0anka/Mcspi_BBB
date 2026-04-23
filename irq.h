#ifndef IRQ_H
#define IRQ_H
#include <linux/interrupt.h>

#define SPI_IRQ_NUMBER 0x00000046U

static irqreturn_t irq_handler ( int irq ,void *dev_id) {

        return IRQ_HANDLED;
}

#endif
