# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += drvFT245

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install drvFT245.dbd into <top>/dbd
DBD += drvFT245.dbd

# specify all source files to be compiled and added to the library
#drvFT245_SRCS += xxx

drvFT245_LIBS += $(EPICS_BASE_IOC_LIBS)
drvFT245_LIBS += asyn

drvFT245_INC += drvFT245.h
drvFT245_SRCS += drvFT245.cpp

USR_CPPFLAGS += $(shell pkg-config --cflags libftdi1)
USR_LDFLAGS += $(shell pkg-config --libs libftdi1)

#External module: libftdi1


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

