#!../../bin/linux-x86_64/test

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change test to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "$(TOP)/dbd/test.dbd"
test_registerRecordDeviceDriver pdbbase

epicsEnvSet("PORT", "FT245A")
drvFT245Configure("$(PORT)", 0)
asynSetTraceMask("$(PORT)",-1, 255)
asynSetTraceIOMask("$(PORT)",-1,255)
## Load record instances
dbLoadRecords("$(TOP)/db/ft245-simple.db","D=TEST_FT245, PORT=$(PORT), TIMEOUT=1")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=daykin"
