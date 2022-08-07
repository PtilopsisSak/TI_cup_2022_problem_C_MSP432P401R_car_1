SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = main.obj system_msp432p401r.obj iar_startup_msp432p401r_ewarm.obj

NAME = empty

CFLAGS = -I.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P401R__ \
    -DDeviceFamily_MSP432P401x \
    --cpu=Cortex-M4F \
    --debug \
    --silent \
    -e \
    --aeabi \
    --thumb \
    --diag_suppress=Pa050

LFLAGS = "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib/display.arm4f" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/grlib/lib/iar/m4f/grlib.a" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/spiffs/lib/iar/m4f/spiffs.a" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib/drivers_msp432p401x.arm4f" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib/iar/m4f/fatfs.a" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/iar/msp432p4xx_driverlib.a" \
    --config ../iar/msp432p401r.icf \
    --map "$(NAME).map" \
    --silent \
    --cpu=Cortex-M4F \
    --semihosting=iar_breakpoint

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

system_msp432p401r.obj: ../system_msp432p401r.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_msp432p401r_ewarm.obj: ../iar/startup_msp432p401r_ewarm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
