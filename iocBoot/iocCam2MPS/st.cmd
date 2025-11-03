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
epicsEnvSet("D", "TEST_FT245")
drvFT245Configure("$(PORT)", 0)
#asynSetTraceMask("$(PORT)",-1, 255)
#asynSetTraceIOMask("$(PORT)",-1,255)
## Load record instances
dbLoadRecords("$(TOP)/db/ft245.db","D=$(D), PORT=$(PORT), TIMEOUT=1")

cd "${TOP}/iocBoot/${IOC}"
iocInit

#Pin assignments: 0-1 in, 2 DC, 3-4 out, 5 DC 6-7 out
dbpf $(D):MODE_CMD_PIN0 0
dbpf $(D):MODE_CMD_PIN1 0
dbpf $(D):MODE_CMD_PIN2 0
dbpf $(D):MODE_CMD_PIN3 1
dbpf $(D):MODE_CMD_PIN4 1
dbpf $(D):MODE_CMD_PIN5 0
dbpf $(D):MODE_CMD_PIN6 1
dbpf $(D):MODE_CMD_PIN7 1

## Start any sequence programs
#seq sncxxx,"user=daykin"
