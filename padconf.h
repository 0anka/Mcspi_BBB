#ifndef __PAD_CONF__
#define __PAD_CONF__
#include "spi.h"

#define CONTROL_MODULE_ADDR 0x44E10000U
#define CONF_SPI0_SCLK 0x44E10950U            //Electrical conf  p9_22
#define CONF_SPI0_d0 0x44E10954U              //Electrical conf. P9_21
#define CONF_SPI0_d1 0x44E10958U              //Electrrical conf.P9_18
#define CONF_SPI0_CS0 0x44E1095CU             // Electrical conf  p9_17
#define CONF_SPI0_CS1 0x44E10960U             // Electrical conf  P9_23 

#define MODE0 (0x01U >> 1U )
#define MODE1 0x00000001U
#define MODE2 ( MODE1 << 1 )
#define MODE3 ( MODE1 | MODE2 )
#define MODE4 (MODE1 << 2 )
#define MODE5 ( MODE4 | MODE1 )
#define MODE6 ( MODE4 | MODE2 )
#define MODE7 ( MODE1 | MODE2 | MODE4 )
#define MASK  ((1U << 7U) | (1U << 6U ) | (1U << 5U ) | ( 1U << 4U )) 

#define DEFINE_CMR(__name,pullselect,pulltype,rxt) \
        struct conf_spi spi0_##__name = { \
                .sctr=FAST, \
                .rx=rxt, \
                .ps=pullselect, \
                .pt=pulltype \
        }       

enum Mode {
        mode0=MODE0,
        mode1=MODE1,
        mode2=MODE2,
        mode3=MODE3,
        mode4=MODE4,
        mode5=MODE5,
        mode6=MODE6,
        mode7=MODE7
};

enum Slewctrl {
        SLOW,
        FAST
};

enum Rxactive {
        OUTPUT,
        BOTH
};

enum Pullselect {
        SELECTDOWN,
        SELECTUP
};

enum Pulltype {
        PULLDOWN,
        PULLUP
};                

struct conf_spi {
        spi_reg cmr_reg;
        enum Slewctrl sctr;
        enum Rxactive rx;
        enum Pullselect ps;
        enum Pulltype pt;
};

void mcspi_conf_spi (struct conf_spi *config ,enum Mode mode);

void mcspi_conf_spi ( struct conf_spi * config ,enum Mode mode) {
        config->cmr_reg.reg|=mode;
        switch ( config->sctr ) {
                case SLOW:
                         config->cmr_reg.bytes.byte0 &= ~(0x01 << 0x07);
                         break;
                case FAST:
                         config->cmr_reg.bytes.byte0 |= (0x01 << 0x07);
                         break;
        }               
        switch ( config->rx ) {
                case OUTPUT:
                         config->cmr_reg.bytes.byte0 &= ~(0x01 << 0x06);
                         break;
                case BOTH:
                         config->cmr_reg.bytes.byte0 |= (0x01 << 0x06);
                         break;
        }               

        switch ( config->ps ) {
                case SELECTDOWN:
                         config->cmr_reg.bytes.byte0 &= ~(0x01 << 0x05);
                         break;
                case SELECTUP:
                         config->cmr_reg.bytes.byte0 |= (0x01 << 0x05);
                         break;
        }               

        switch ( config->pt ) {
                case PULLDOWN:
                         config->cmr_reg.bytes.byte0 &= ~(0x01 << 0x04);
                         break;
                case PULLUP:
                         config->cmr_reg.bytes.byte0 |= (0x01 << 0x04);
                         break;
        }               


}        

#endif 
