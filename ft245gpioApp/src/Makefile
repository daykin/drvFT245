# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ft245gpio
# ft245gpio.dbd will be created and installed
DBD += ft245gpio.dbd

# ft245gpio.dbd will be made up from these files:
ft245gpio_DBD += base.dbd asyn.dbd

ft245gpio_LIBS += asyn
# Include dbd files from all support applications:
#ft245gpio_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
USR_CPPFLAGS += $(shell pkg-config --cflags libftdi1)
USR_LDFLAGS += $(shell pkg-config --libs libftdi1)

# ft245gpio_registerRecordDeviceDriver.cpp derives from ft245gpio.dbd
ft245gpio_SRCS += ft245gpio_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ft245gpio_SRCS_DEFAULT += ft245gpioMain.cpp
ft245gpio_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ft245gpio_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ft245gpio_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

