/*
    MCSPI_BBB TI am355x ,
    hardcoded driver for custom kernel build ,
    without DT
    Copyright (C) 2026 Malefax   
*/  
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/errno.h>
#include <linux/types.h>
#include <asm/io.h>
#include <linux/spinlock.h>
#include <linux/jiffies.h>
#include <linux/timex.h>
#include <linux/delay.h>
#include "spi.h"
#include "irq.h"
#include "padconf.h"
mcspi_addr addr;
mcspi_addr cmr_addr;
struct mcspi_sysconfig registor = {
        .cflag=OCP_ON_AND_FC_ON,
        .sflag=SMART_IDLE,
        .rflag=MODULE_RESET,
        .aflag=AUTOIDLE
};

struct mcspi_status status = {
        .mcspi_reg = {
                .reg = 0
        }                
};

DEFINE_CMR(spiclk,SELECTDOWN,PULLDOWN,OUTPUT);       //SCLK
DEFINE_CMR(d0,SELECTUP,PULLUP,BOTH);                 //MISO
DEFINE_CMR(d1,SELECTDOWN,PULLDOWN,OUTPUT);           //MOSI
DEFINE_CMR(cs0,SELECTUP,PULLUP,OUTPUT);              //CS0
DEFINE_CMR(cs1,SELECTUP,PULLUP,OUTPUT);              //CS1

static int __init entrymodule(void) {
        u32 read;
        int error;
        unsigned long int timestamp;
        if ( regaddr(&cmr_addr,CONTROL_MODULE_ADDR) ){
                pr_err("PAD_CONF regaddr failed: %d\n" ,cmr_addr.err);
                return cmr_addr.err;
        }
                mcspi_conf_spi(&spi0_spiclk,MODE0); 
                mcspi_conf_spi(&spi0_d0,MODE0); 
                mcspi_conf_spi(&spi0_d1,MODE0); 
                mcspi_conf_spi(&spi0_cs0,MODE0); 
                mcspi_conf_spi(&spi0_cs1,MODE0);
               
              iowrite32(spi0_spiclk.cmr_reg.reg,cmr_addr.regaddr+CONF_SPI0_SCLK);
              iowrite32(spi0_d0.cmr_reg.reg,cmr_addr.regaddr+CONF_SPI0_d0 ); 
              iowrite32(spi0_d1.cmr_reg.reg,cmr_addr.regaddr+CONF_SPI0_d1 );
              iowrite32(spi0_cs0.cmr_reg.reg,cmr_addr.regaddr+CONF_SPI0_CS0 );
              iowrite32(spi0_cs1.cmr_reg.reg,cmr_addr.regaddr+CONF_SPI0_CS1 );
              udelay(50);



        if ( regaddr(&addr,MCSPI_CORE_ADDR) ) {
                pr_err("MCSPI: regaddr failed: %d\n", addr.err);
                return addr.err;
                }

                registor.mcspi_reg.reg = 0;
                pr_info("MCSPI: Mapped @ %p\n", addr.regaddr);
                timestamp=jiffies+2*HZ;

            do {
                   
                   read = ioread32(addr.regaddr + MCSPI_SYSSTATUS);
                   if ( read & BIT(0)) {
                       break;
                    }    
                   cpu_relax();
        } while (time_before(jiffies,timestamp)); 

            if (!(read & BIT(0))){
                    goto timeout;
            }       
    
                pr_info("MCSPI: SYSSTATUS ready: 0x%08x\n", read);
    
               void (*spi_sysconfig[4])( struct mcspi_sysconfig *config ) = {
                mcspi_sysconfig_clockactivity,\
                mcspi_sysconfig_sidlemode,\
                mcspi_sysconfig_softrest,\
                mcspi_sysconfig_autoidle };

                spi_sysconfig[0](&registor);
                spi_sysconfig[1](&registor);
                spi_sysconfig[2](&registor);     
                spi_sysconfig[3](&registor);
                iowrite32 (registor.mcspi_reg.reg,addr.regaddr+MCSPI_SYSCONFIG); 
                timestamp=jiffies+2*HZ;
                do {
                        read = ioread32(addr.regaddr+MCSPI_SYSSTATUS);
                        if (read & BIT(0))
                        {
                          break;
                        }
                        cpu_relax();
                } while(time_before(jiffies,timestamp));
                if (!(read & BIT(0))){
                        pr_info("softreset not set");
                        goto timeout;
                }        

                pr_info("SOFTRESET successfully\t:%x",registor.mcspi_reg.bytes.byte0);
                if ((error=request_irq(SPI_IRQ_NUMBER,irq_handler,IRQF_SHARED,"my_device",(void *)(irq_handler)))<0){
                        pr_warn("Unable to request desired irq:%d",SPI_IRQ_NUMBER); 
                        goto error;
                }        
                       
        return 0;

timeout:
        if (addr.regaddr){
                iounmap(addr.regaddr);
        }
        if ( cmr_addr.regaddr ){
                iounmap(cmr_addr.regaddr);
        }        
        return -ETIMEDOUT;
error:        
        
        free_irq(SPI_IRQ_NUMBER,(void *)(irq_handler));
        return error; 
}
static void __exit exitmodule(void){
        
        free_irq(SPI_IRQ_NUMBER,(void *)(irq_handler)); 
        if (addr.regaddr){
            iounmap(addr.regaddr);
 }
if ( cmr_addr.regaddr ){
                iounmap(cmr_addr.regaddr);
        }        
        pr_info("%sMCSPI: Unloaded\n",__func__);
}        

module_init(entrymodule);
module_exit(exitmodule);
MODULE_AUTHOR("HARIHAR");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NONE");

