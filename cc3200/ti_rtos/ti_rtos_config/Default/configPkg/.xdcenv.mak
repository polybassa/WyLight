#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/ccsv6/ccs_base;C:/ti/tirtos_simplelink_2_00_02_36/packages;C:/ti/tirtos_simplelink_2_00_02_36/products/bios_6_40_02_27/packages;C:/ti/tirtos_simplelink_2_00_02_36/products/uia_2_00_01_34/packages
override XDCROOT = c:/ti/xdctools_3_30_03_47_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/ccsv6/ccs_base;C:/ti/tirtos_simplelink_2_00_02_36/packages;C:/ti/tirtos_simplelink_2_00_02_36/products/bios_6_40_02_27/packages;C:/ti/tirtos_simplelink_2_00_02_36/products/uia_2_00_01_34/packages;c:/ti/xdctools_3_30_03_47_core/packages;..
HOSTOS = Windows
endif
