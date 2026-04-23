#ifndef __SPI_H_
#define __SPI_H_

#include<linux/types.h>
#include<linux/io.h>
#define MCSPI_CORE_ADDR       0x48030000U
#define MCSPI_REGISTER_SIZE   0x1000U
#define MCSPI_REVISION        0x00000000U
#define MCSPI_SYSCONFIG       0x00000110U
#define MCSPI_SYSSTATUS       0x00000114U
#define MCSPI_IRQSTATUS       0x00000118U
#define MCSPI_IRQENABLE       0x0000011CU
#define MCSPI_SYST            0x00000124U
#define MCSPI_MODULCTRL       0x00000128U
#define MCSPI_XFERLEVEL       0x0000017CU
#define MCSPI_DAFTX           0x00000180U
#define MCSPI_DAFRX           0x000001A0U
#define MCSPI_CH0CONF         0x0000012CU
#define MCSPI_CH0STAT         0x00000130U
#define MCSPI_CH0CTRL         0x00000134U
#define MCSPI_TX0             0x00000138U
#define MCSPI_RX0             0x0000013CU
#define MCSPI_CH1CONF         0x00000140U
#define MCSPI_CH1STAT         0x00000144U
#define MCSPI_CH1CTRL         0x00000148U
#define MCSPI_TX1             0x0000014CU
#define MCSPI_RX1             0x00000150U
#define MCSPI_CH2CONF         0x00000154U
#define MCSPI_CH2STAT         0x00000158U
#define MCSPI_CH2CTRL         0x0000015CU
#define MCSPI_TX2             0x00000160U
#define MCSPI_RX2             0x00000164U
#define MCSPI_CH3CONF         0x0000016CU
#define MCSPI_CH3CTRL         0x00000170U
#define MCSPI_TX3             0x00000174U
#define MCSPI_RX3             0x00000178U
#define MCSPSI_STATUS         (0xFU & 0x1U ) 
#define MEM_ADDR(OFFSET) (MCSPI_CORE_ADDR+OFFSET)

#define __x86
#define armv7l CONFIG_ARM

#ifdef __x86
typedef union {
    u32 reg;
    struct {
#if defined(__LITTLE_ENDIAN_BITFIELD)
        u8 byte0;
        u8 byte1;
        u8 byte2;
        u8 byte3;
#else
        u8 byte3;
        u8 byte2;
        u8 byte1;
        u8 byte0;
#endif
    } bytes;
} spi_reg;
#endif


enum Clockactivity {  
    OCP_OFF_AND_FC_OFF,
    OCP_ON_AND_FC_OFF,
    OCP_OFF_AND_FC_ON,
    OCP_ON_AND_FC_ON
};

enum sidlemode {  
    IDLE_DETECTED,
    IDLE_REQUEST_NORMAL,
    SMART_IDLE
};

enum Softreset {
    NORMAL_MODE,
    MODULE_RESET
};

enum Autoidle {  
    OCP_CLOCK_FREE,
    AUTOIDLE
};

typedef struct MCSPI_ADDR {
    void * __iomem regaddr;
    int err;
}mcspi_addr;


struct mcspi_sysconfig {
    spi_reg mcspi_reg;
    enum Clockactivity cflag;  
    enum sidlemode sflag;      
    enum Softreset rflag;
    enum Autoidle aflag;
};

struct mcspi_status {
        spi_reg mcspi_reg;
};

void mcspi_sysconfig_clockactivity ( struct mcspi_sysconfig *config);
void mcspi_sysconfig_sidlemode (struct mcspi_sysconfig *config );
void mcspi_sysconfig_softrest( struct mcspi_sysconfig *config );
void mcspi_sysconfig_autoidle ( struct mcspi_sysconfig *config );
bool regaddr ( mcspi_addr *addr,u32 base_addr );


void mcspi_sysconfig_clockactivity (struct mcspi_sysconfig *config ){
        config->mcspi_reg.bytes.byte1 &= ~0x03;

        switch (config->cflag){
            case OCP_OFF_AND_FC_OFF:
                    config->mcspi_reg.bytes.byte1 &= ~(0x01);
                    config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x01);
                    break;
            case OCP_ON_AND_FC_OFF:
                    config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x01 );
                    config->mcspi_reg.bytes.byte1 |= 0x01;
                    break;
            case OCP_OFF_AND_FC_ON:
                    config->mcspi_reg.bytes.byte1 &= ~(0x01);
                    config->mcspi_reg.bytes.byte1 |= (0x01 << 0x01);
                    break;
            case OCP_ON_AND_FC_ON:
                    config->mcspi_reg.bytes.byte1 |= (0x01);
                    config->mcspi_reg.bytes.byte1 |= (0x01 << 0x01 );
                    break;
        }


}

void mcspi_sysconfig_sidlemode (struct mcspi_sysconfig *config ){

        switch (config->sflag){
            case IDLE_DETECTED:
                    config->mcspi_reg.bytes.byte0 &= ~(0x01<< 0x04);
                    config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x05);
                    break;
            case IDLE_REQUEST_NORMAL:
                    config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x05 );
                    config->mcspi_reg.bytes.byte0 |= (0x01 << 0x04);
                    break;
            case SMART_IDLE:
                    config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x04);
                    config->mcspi_reg.bytes.byte0 |= (0x01 << 0x05);
                    break;

       }


}

void mcspi_sysconfig_softrest( struct mcspi_sysconfig *config ){

                switch(config->rflag){
                    case NORMAL_MODE:
                            config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x02);
                            break;
                    case MODULE_RESET:
                            config->mcspi_reg.bytes.byte0 |= (0x01 << 0x02);
                            break;
                }
}

void mcspi_sysconfig_autoidle ( struct mcspi_sysconfig *config ){
           switch(config->aflag) {
           case OCP_CLOCK_FREE:
                   config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x01);
                   break;
           case AUTOIDLE:
                   config->mcspi_reg.bytes.byte0 |= (0x01 << 0x01);
                   break;
           }
}

bool regaddr ( mcspi_addr *addr,u32 base_addr ) {
        if (!addr ){
                return true;
        }        
        addr->regaddr=ioremap(base_addr,MCSPI_REGISTER_SIZE);
        if (!(addr->regaddr)) {
                  addr->err=-ENOMEM;
                  return true;
                  }
        addr->err=0;           
        return false;
}

#endif
