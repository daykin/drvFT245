#!../../bin/linux-x86_64/ft245gpio

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change ft245gpio to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ft245gpio.dbd"
ft245gpio_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/ft245gpio.db","user=daykin")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=daykin"
