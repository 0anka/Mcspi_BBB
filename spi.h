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
#define MAX_WL(WL)       ( ((WL) & 0x1F) )
#define MAX_CLKD_POS(CL)     ( ((CL) & 0xF ) << (2))

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

enum CLKG {
        CG_POW_2,
        CG_1
};

enum FFER {
        DEACTIVE_BUF,
        ACTIVE_BUF
};

enum FFEW {
        T_BUF_NA,
        T_BUF_A
};

enum TCS {
        CC_0_5,
        CC_1_5,
        CC_2_5,
        CC_3_5
};

enum SBPOL {
        POLARITY_LOW,
        POLARITY_HIGH
};

enum SBE {
        DEFAULT_WL,
        START_SBPOL
};

enum SPIENSLV {
        EN_1,
        EN_2,
        EN_3,
        EN_4
};

enum FORCE {
        SPILIN_DRIVE_LOW,
        SPILIN_DRIVE_HIGH
};

enum TURBO {
        TUR_D,
        TUR_A
};                

enum INPUT_S {
        SPIDAT0,
        SPIDAT1
};

enum DPE1 {
        SPIDAT1_S,
        SPIDAT1_NS
};

enum DPE0 {
        SPIDAT0_S,
        SPIDAT0_NS
};

enum DMAR {
        R_DMA_DISABLED,
        R_DMA_ENABLED 
};

enum DMAW {
        RW_DMA_DISABLED,
        RW_DMA_ENABLED
};

enum TRM {
        TR,
        RECEIVE_ONLY,
        TRANSMIT_ONLY
};

enum EPOL {
        SPIEN_HIGH,
        SPIEN_LOW
};

enum POL {
        SPICLK_HIGH,
        SPICLK_LOW
};

enum PHA {
        DATA_LATCH_ODD,
        DATA_LATCH_EVEN
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

struct mcspi_ch0_conf {
        spi_reg mcspi_reg;
        enum CLKG clk;
        enum FFER ff;
        enum FFEW fe;
        enum TCS tc;
        enum SBPOL sb;
        enum SBE sbe;
        enum SPIENSLV slv;
        enum FORCE frc;
        enum TURBO tu;
        enum INPUT_S ins;
        enum DPE1 dpe;
        enum DPE0 dp0;
        enum DMAR dmr;
        enum DMAW dmw;
        enum TRM tr;
        enum EPOL ep;
        enum POL pol;
        enum PHA pha;

};

void mcspi_sysconfig_clockactivity ( struct mcspi_sysconfig *config);
void mcspi_sysconfig_sidlemode (struct mcspi_sysconfig *config );
void mcspi_sysconfig_softrest( struct mcspi_sysconfig *config );
void mcspi_sysconfig_autoidle ( struct mcspi_sysconfig *config );
void mcspi_ch0_conf ( struct mcspi_ch0_conf * config , u8 wl ,u8 clkd);
bool regaddr ( mcspi_addr *addr,u32 base_addr );

void mcspi_sysconfig_clockactivity (struct mcspi_sysconfig *config ){

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
void mcspi_ch0_conf ( struct mcspi_ch0_conf * config, u8 wl, u8 clkd ) {
    config->mcspi_reg.reg=0;
    config->mcspi_reg.reg |=MAX_CLKD_POS(clkd);

    if ( MAX_WL(wl) != 0x00 && MAX_WL(wl) !=0x01 && MAX_WL(wl)!= 0x02 ) {
           config->mcspi_reg.reg |=(MAX_WL(wl) << 7 );
       }
    else { 
        config->mcspi_reg.reg |=(0x07 << 7);
    }
    switch( config->clk ) {
        case CG_POW_2:
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x05);
            break;
        case CG_1:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x05);
            break;
    }

    switch( config->ff ) {
        case DEACTIVE_BUF:
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x04);
            break;
        case ACTIVE_BUF:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x04);
            break;
    }

    switch ( config->tc ) {
        case CC_0_5:
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x02);
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x01);
            break;
        case CC_1_5:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x01);
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x02);
            break;
        case CC_2_5:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x02);
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x01);
            break;
        case CC_3_5:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x02);
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x01);
            break;
    }

    switch ( config->sb ) {
        case POLARITY_LOW:
            config->mcspi_reg.bytes.byte3 &= ~(0x01 << 0x00);
            break;
        case POLARITY_HIGH:
            config->mcspi_reg.bytes.byte3 |= (0x01 << 0x00);
            break;
    }

    switch ( config->sbe ) {
        case DEFAULT_WL:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x07);
            break;
        case START_SBPOL:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x07);
            break;
    }

    switch ( config->slv ) {
        case EN_1:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x06);
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x05);
            break;
        case EN_2:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x05);
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x06);
            break;
        case EN_3:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x06);
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x05);
            break;
        case EN_4:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x06);
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x05);
            break;
    }

    switch ( config->frc ) {
        case SPILIN_DRIVE_LOW:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x04);
            break;
        case SPILIN_DRIVE_HIGH:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x04);
            break;
    }

    switch ( config->ins ) {
        case SPIDAT0:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x02);
            break;
        case SPIDAT1:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x02);
            break;
    }

    switch (config->dpe ) {
        case SPIDAT1_S:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x01);
            break;
        case SPIDAT1_NS:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x01);
            break;
    }

    switch ( config->dp0 ) {
        case SPIDAT0_S:
            config->mcspi_reg.bytes.byte2 &= ~(0x01 << 0x00);
            break;
        case SPIDAT0_NS:
            config->mcspi_reg.bytes.byte2 |= (0x01 << 0x00);
            break;
    }

    switch ( config->dmr ) {
        case R_DMA_DISABLED:
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x07);
            break;
        case R_DMA_ENABLED:
            config->mcspi_reg.bytes.byte1 |= (0x01 << 0x07);
            break;
    }

    switch ( config->dmw ) {
        case RW_DMA_DISABLED:
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x06);
            break;
        case RW_DMA_ENABLED:
            config->mcspi_reg.bytes.byte1 |= (0x01 << 0x06);
            break;
    }

    switch ( config->tr ) {
        case TR:
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x05);
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x04);
            break;
        case RECEIVE_ONLY:
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x05);
            config->mcspi_reg.bytes.byte1 |= (0x01 << 0x04);
            break;
        case TRANSMIT_ONLY:
            config->mcspi_reg.bytes.byte1 |= (0x01 << 0x05);
            config->mcspi_reg.bytes.byte1 &= ~(0x01 << 0x04);
            break;
    }

    switch ( config->ep ) {
        case SPIEN_HIGH:
            config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x06);
            break;
        case SPIEN_LOW:
            config->mcspi_reg.bytes.byte0 |= (0x01 << 0x06);
            break;
    }

    switch ( config->pol ) {
        case SPICLK_HIGH:
            config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x01);
            break;
        case SPICLK_LOW:
            config->mcspi_reg.bytes.byte0 |= (0x01 << 0x01);
            break;
    }

    switch ( config->pha ) {
        case DATA_LATCH_ODD:
            config->mcspi_reg.bytes.byte0 &= ~(0x01 << 0x00);
            break;
        case DATA_LATCH_EVEN:
            config->mcspi_reg.bytes.byte0 |= (0x01 << 0x00);
            break;
    }

    switch ( config->fe ) {
        case T_BUF_NA:
            config->mcspi_reg.bytes.byte3  &= ~( 0x01 << 0x03 );
            break;
        case T_BUF_A:
            config->mcspi_reg.bytes.byte3 |= ( 0x01 << 0x03 );
            break;
    }

    switch ( config->tu ) {
        case TUR_D:
            config->mcspi_reg.bytes.byte2 &= ~( 0x01 << 0x03 );
            break;
        case TUR_A:
            config->mcspi_reg.bytes.byte2 |= ( 0x01 << 0x03 );
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
