/*
     MCSPI_BBB TI am335x ,
     hardcoded driver for custom kernel build ,
     without DT.
     Copyright (C) 2026 Malefax

*/
#if defined(__FreeBSD__)
#include <sys/bus.h>
#include <sys/errno.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/param.h>
#include <sys/system.h>
#include <sys/types.h>
#else

#include "irq.h"
#include "padconf.h"
#include "spi.h"
#include <asm/errno.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/types.h>
#endif
#if defined(__FreeBSD__)
static int module_event(module_t curr_mod, int modevent_type, void *arg);

moduledata_t module_reg_sysinit = {
    .name = "NONE", .evhand = module_event, .priv = NULL};
#endif
static mcspi_addr addr;
static mcspi_addr cmr_addr;
static struct mcspi_sysconfig registor = {.cflag = OCP_ON_AND_FC_ON,
                                          .sflag = SMART_IDLE,
                                          .rflag = MODULE_RESET,
                                          .aflag = AUTOIDLE};

static struct mcspi_ch0_conf ch0_reg = {
    .pol = SPICLK_HIGH,
    .pha = DATA_LATCH_ODD, // POL=0,PHA=0 Mode 0 up
    .clk = CG_1,
    .ff = DEACTIVE_BUF,
    .fe = T_BUF_NA,
    .tc = CC_0_5,
    .sb = POLARITY_LOW,
    .sbe = DEFAULT_WL,
    .slv = EN_1,
    .frc = SPILIN_DRIVE_LOW,
    .tu = TUR_D,
    .ins = SPIDAT0,
    .dpe = SPIDAT1_S,
    .dp0 = SPIDAT0_S,
    .dmr = R_DMA_DISABLED,
    .dmw = RW_DMA_DISABLED,
    .tr = TR,
    .ep = SPIEN_LOW
};

static struct module_ctrl ctrl_reg = {.fda = MCSPI_TX_RX_ENABLED,
                                      .moa = MWL_ENABLED,
                                      .init = NO_DELAY_SPI,
                                      .sys = FUNCTION_MODE,
                                      .ms = MASTER_G_SPICLK,
                                      .pin = SPIEN_CS,
                                      .sin = MULTI_CH_MASTER};




static struct mcspi_status status = {
        .mcspi_reg = {
                .reg = 0
        }                
};

DEFINE_CMR(spiclk,SELECTDOWN,PULLDOWN,OUTPUT);       //SCLK
DEFINE_CMR(d0,SELECTUP,PULLUP,BOTH);                 //MISO
DEFINE_CMR(d1,SELECTDOWN,PULLDOWN,OUTPUT);           //MOSI
DEFINE_CMR(cs0,SELECTUP,PULLUP,OUTPUT);              //CS0
DEFINE_CMR(cs1,SELECTUP,PULLUP,OUTPUT);              //CS1
#if defined(__FreeBSD__) 
static int module_event(module_t curr_mod,int modevent_type, void *arg){
        switch(modevent_type){
                case MOD_LOAD:
                    void (*spi_sysconfig[4])( struct mcspi_sysconfig *config ) = {
                    mcspi_sysconfig_clockactivity,\
                    mcspi_sysconfig_sidlemode,\
                    mcspi_sysconfig_softrest,\
                    mcspi_sysconfig_autoidle };
                        if ( regaddr(&cmr_addr,CONTROL_MODULE_ADDR) ){
                                pr_err("PAD_CONF regaddr failed: %d\n" ,cmr_addr.err);
                                return cmr_addr.err;
                            }               
                       if ( regaddr(&addr, MCSPI_CORE_ADDR)   ){
                                pr_err("MCSPI_CORE regaddr failed: %d\n",addr.err);
                                if (cmr_addr.regaddr){
                                        pmap_unmapdev(cmr_addr.regaddr,(vm_size_t)MCSPI_REGISTER_SIZE);
                                        cmr_addr.regaddr = NULL;
                                
                       }
                              return addr.err;

                       }
                                mcspi_conf_spi(&spi0_spiclk,MODE0); 
                                mcspi_conf_spi(&spi0_d0,MODE0); 
                                mcspi_conf_spi(&spi0_d1,MODE0); 
                                mcspi_conf_spi(&spi0_cs0,MODE0); 
                                mcspi_conf_spi(&spi0_cs1,MODE0);
                              __iowrite32(spi0_spiclk.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_SCLK);
                              __iowrite32(spi0_d0.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_d0);
                              __iowrite32(spi0_d1.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_d1);
                              __iowrite32(spi0_cs0.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_CS0);
                              __iowrite32(spi0_cs1.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_CS1);
                               DELAY(50);


                                
                               
#else 
static int __init entrymodule(void) {
  u32 read;
  int error;
  unsigned long int timestamp;
  void (*spi_sysconfig[4])(struct mcspi_sysconfig *config) = {
      mcspi_sysconfig_clockactivity, mcspi_sysconfig_sidlemode,
      mcspi_sysconfig_softrest, mcspi_sysconfig_autoidle};

  if (regaddr(&cmr_addr, CONTROL_MODULE_ADDR)) {
    pr_err("PAD_CONF regaddr failed: %d\n", cmr_addr.err);
    return cmr_addr.err;
  }
  if (regaddr(&addr, MCSPI_CORE_ADDR)) {
    pr_err("MCSPI_CORE regaddr failed: %d\n", addr.err);
    if (cmr_addr.regaddr) {
      iounmap(cmr_addr.regaddr);
      cmr_addr.regaddr = NULL;
    }

    return addr.err;
  }
  mcspi_conf_spi(&spi0_spiclk, MODE0);
  mcspi_conf_spi(&spi0_d0, MODE0);
  mcspi_conf_spi(&spi0_d1, MODE0);
  mcspi_conf_spi(&spi0_cs0, MODE0);
  mcspi_conf_spi(&spi0_cs1, MODE0);

  iowrite32(spi0_spiclk.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_SCLK);
  iowrite32(spi0_d0.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_d0);
  iowrite32(spi0_d1.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_d1);
  iowrite32(spi0_cs0.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_CS0);
  iowrite32(spi0_cs1.cmr_reg.reg, cmr_addr.regaddr + CONF_SPI0_CS1);
  udelay(50);

  // registor.mcspi_reg.reg = 0;
  timestamp = jiffies + 2 * HZ;

  do {

    read = ioread32(addr.regaddr + MCSPI_SYSSTATUS);
    if (read & BIT(0)) {
      break;
    }
    cpu_relax();
  } while (time_before(jiffies, timestamp));

  if (!(read & BIT(0))) {
    goto timeout;
  }

  pr_info("MCSPI: SYSSTATUS ready: 0x%08x\n", read);

  registor.mcspi_reg.reg = 0;
  registor.rflag = MODULE_RESET;
  spi_sysconfig[2](&registor);
  iowrite32(registor.mcspi_reg.reg, addr.regaddr + MCSPI_SYSCONFIG);
  timestamp = jiffies + 2 * HZ;
  do {
    read = ioread32(addr.regaddr + MCSPI_SYSSTATUS);
    if (read & BIT(0)) {
      break;
    }
    cpu_relax();
  } while (time_before(jiffies, timestamp));
  if (!(read & BIT(0))) {
    pr_info("softreset not set");
    goto timeout;
  }

  registor.mcspi_reg.reg = 0;
  registor.rflag = NORMAL_MODE;
  spi_sysconfig[2](&registor);
  iowrite32(registor.mcspi_reg.reg, addr.regaddr + MCSPI_SYSCONFIG);

  registor.mcspi_reg.reg = 0;
  spi_sysconfig[0](&registor);
  spi_sysconfig[1](&registor);
  spi_sysconfig[3](&registor);
  iowrite32(registor.mcspi_reg.reg, addr.regaddr + MCSPI_SYSCONFIG);

  mcspi_ctrl_conf(&ctrl_reg);
  iowrite32(ctrl_reg.mcspi_reg.reg, addr.regaddr + MCSPI_MODULCTRL);
  udelay(50);

  mcspi_ch0_conf(&ch0_reg, 0x07, 0x01);
  iowrite32(ch0_reg.mcspi_reg.reg, addr.regaddr + MCSPI_CH0CONF);
  udelay(50);

  pr_info("SOFTRESET successfully\t:%x", registor.mcspi_reg.bytes.byte0);
  if ((error = request_irq(SPI_IRQ_NUMBER, irq_handler, IRQF_SHARED,
                           "my_device", (void *)(irq_handler))) < 0) {
    pr_warn("Unable to request desired irq:%d", SPI_IRQ_NUMBER);
    goto error;
  }

  return 0;

timeout:
  if (addr.regaddr) {
    iounmap(addr.regaddr);
    addr.regaddr = NULL;
  }
  if (cmr_addr.regaddr) {
    iounmap(cmr_addr.regaddr);
    cmr_addr.regaddr = NULL;
  }

  return -ETIMEDOUT;
error:

  free_irq(SPI_IRQ_NUMBER, (void *)(irq_handler));
  if (addr.regaddr) {
    iounmap(addr.regaddr);
    addr.regaddr = NULL;
  }
  if (cmr_addr.regaddr) {
    iounmap(cmr_addr.regaddr);
    cmr_addr.regaddr = NULL;
  }

  return error;
}
static void __exit exitmodule(void) {

  free_irq(SPI_IRQ_NUMBER, (void *)(irq_handler));

  if (addr.regaddr) {
    iounmap(addr.regaddr);
    addr.regaddr = NULL;
  }

  if (cmr_addr.regaddr) {
    iounmap(cmr_addr.regaddr);
    cmr_addr.regaddr = NULL;
  }

  pr_info("%sMCSPI: Unloaded\n", __func__);
}

module_init(entrymodule);
module_exit(exitmodule);
MODULE_AUTHOR("HARIHAR");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NONE");

#endif
#if defined(__FreeBSD__)
DECLARE_MODULE("NONE", &data, SI_SUB_EXEC, SI_SUB_KLD);
#endif
