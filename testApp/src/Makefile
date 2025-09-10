# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_CPPFLAGS += -I$(TOP)/drvFT245App/src

drvFT245_DIR = $(TOP)/lib/$(EPICS_HOST_ARCH)
USR_CPPFLAGS += $(shell pkg-config --cflags libftdi1)
USR_LDFLAGS += $(shell pkg-config --libs libftdi1)
USR_LDFLAGS += -Wl,-rpath $(drvFT245_DIR)
test_LIBS +=  drvFT245
PROD_SYS_LIBS += drvFT245
#=============================
# Build the IOC application

PROD_IOC = test
# test.dbd will be created and installed
DBD += test.dbd

# test.dbd will be made up from these files:
test_DBD += base.dbd asyn.dbd

# Include dbd files from all support applications:
test_DBD += drvFT245.dbd

# Add all the support libraries needed by this IOC
test_LIBS += asyn

# test_registerRecordDeviceDriver.cpp derives from test.dbd
test_SRCS += test_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
test_SRCS_DEFAULT += testMain.cpp
test_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#test_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
test_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

