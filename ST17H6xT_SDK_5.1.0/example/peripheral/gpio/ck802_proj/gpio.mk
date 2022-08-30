##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=gpio
ConfigurationName      :=BuildSet
WorkspacePath          :=./
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=admin
Date                   :=29/08/2022
CDKPath                :="D:/Program Files (x86)/C-Sky/CDK"
LinkerName             :=csky-elfabiv2-gcc
LinkerNameoption       :=
SIZE                   :=csky-elfabiv2-size
READELF                :=csky-elfabiv2-readelf
CHECKSUM               :=crc32
SharedObjectLinkerName :=
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DisassemSuffix         :=.asm
IHexSuffix             :=.ihex
BinSuffix              :=.bin
ExeSuffix              :=.elf
LibSuffix              :=.a
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
ElfInfoSwitch          :=-hlS
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
UnPreprocessorSwitch   :=-U
SourceSwitch           :=-c 
ObjdumpSwitch          :=-S
ObjcopySwitch          :=-O ihex
ObjcopyBinSwitch       :=-O binary
OutputFile             :=$(ProjectName)
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=gpio.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mcpu=ck802  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/ck802/gcc_csky.ld"
LinkOtherFlagsOption   :=-u jump_table_base -u global_config -Wl,-zmax-page-size=1024 "$(ProjectPath)/../../../../misc/jack_rom_ck802.elf.ck802sym"
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch)../../../../components/arch/ck802 $(IncludeSwitch)../../../../components/ble/controller $(IncludeSwitch)../../../../components/ble/hci $(IncludeSwitch)../../../../components/ble/host $(IncludeSwitch)../../../../components/ble/include $(IncludeSwitch)../../../../components/driver/include $(IncludeSwitch)../../../../components/inc $(IncludeSwitch)../../../../components/libraries/crc16 $(IncludeSwitch)../../../../components/libraries/dfl $(IncludeSwitch)../../../../components/libraries/dwc $(IncludeSwitch)../../../../components/libraries/secure $(IncludeSwitch)../../../../components/osal/include $(IncludeSwitch)../../../../components/profiles/DevInfo $(IncludeSwitch)../../../../components/profiles/Roles $(IncludeSwitch)../../../../components/profiles/SimpleProfile $(IncludeSwitch)../../../../misc $(IncludeSwitch)../source $(IncludeSwitch)ck802/include $(IncludeSwitch)driver/include $(IncludeSwitch)include  
IncludeAPath           := $(IncludeSwitch). $(IncludeSwitch)../source $(IncludeSwitch)driver_rom/include $(IncludeSwitch)include  
Libs                   := -Wl,--start-group  -Wl,--end-group $(LibrarySwitch)m  
ArLibs                 := "m" 
PackagesLibPath        :=
LibPath                := $(PackagesLibPath) 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       :=csky-elfabiv2-ar rcu
CXX      :=csky-elfabiv2-g++
CC       :=csky-elfabiv2-gcc
AS       :=csky-elfabiv2-gcc
OBJDUMP  :=csky-elfabiv2-objdump
OBJCOPY  :=csky-elfabiv2-objcopy
CXXFLAGS := -mcpu=ck802   $(PreprocessorSwitch)_PHY_DEBUG  $(PreprocessorSwitch)SERV_GGS=0  $(PreprocessorSwitch)SERV_GATT_SERV=0  $(PreprocessorSwitch)SERV_DEV_INFO=0  $(PreprocessorSwitch)PHY_MCU_TYPE=MCU_BUMBEE_CK802  $(PreprocessorSwitch)MAIN_HAS_NOARGC  $(PreprocessorSwitch)MAIN_HAS_NORETURN  $(PreprocessorSwitch)OSAL_CBTIMER_NUM_TASKS=1  $(PreprocessorSwitch)HCI_TL_NONE=1  $(PreprocessorSwitch)OSALMEM_METRICS=0  $(PreprocessorSwitch)DEBUG_INFO=1  $(PreprocessorSwitch)CFG_WDT_ENABLE=1  $(PreprocessorSwitch)CFG_SLEEP_MODE=PWR_MODE_SLEEP  $(PreprocessorSwitch)USE_ROMSYM_ALIAS=1  $(PreprocessorSwitch)HOST_CONFIG=6   -Os  -g3  -Wall  -ffunction-sections -ffunction-sections -fdata-sections -DADV_NCONN_CFG=0x01  -DADV_CONN_CFG=0x02  -DSCAN_CFG=0x04   -DINIT_CFG=0x08   -DBROADCASTER_CFG=0x01 -DOBSERVER_CFG=0x02  -DPERIPHERAL_CFG=0x04  -DCENTRAL_CFG=0x08  
CFLAGS   := -mcpu=ck802   $(PreprocessorSwitch)_PHY_DEBUG  $(PreprocessorSwitch)SERV_GGS=0  $(PreprocessorSwitch)SERV_GATT_SERV=0  $(PreprocessorSwitch)SERV_DEV_INFO=0  $(PreprocessorSwitch)PHY_MCU_TYPE=MCU_BUMBEE_CK802  $(PreprocessorSwitch)MAIN_HAS_NOARGC  $(PreprocessorSwitch)MAIN_HAS_NORETURN  $(PreprocessorSwitch)OSAL_CBTIMER_NUM_TASKS=1  $(PreprocessorSwitch)HCI_TL_NONE=1  $(PreprocessorSwitch)OSALMEM_METRICS=0  $(PreprocessorSwitch)DEBUG_INFO=1  $(PreprocessorSwitch)CFG_WDT_ENABLE=1  $(PreprocessorSwitch)CFG_SLEEP_MODE=PWR_MODE_SLEEP  $(PreprocessorSwitch)USE_ROMSYM_ALIAS=1  $(PreprocessorSwitch)HOST_CONFIG=6   -Os  -g3  -Wall  -ffunction-sections -ffunction-sections -fdata-sections -DADV_NCONN_CFG=0x01  -DADV_CONN_CFG=0x02  -DSCAN_CFG=0x04   -DINIT_CFG=0x08   -DBROADCASTER_CFG=0x01 -DOBSERVER_CFG=0x02  -DPERIPHERAL_CFG=0x04  -DCENTRAL_CFG=0x08  
ASFLAGS  := -mcpu=ck802    -Wa,--gdwarf2  -ffunction-sections -fdata-sections  


Objects0=$(IntermediateDirectory)/ck802_board_init$(ObjectSuffix) $(IntermediateDirectory)/ck802_startup$(ObjectSuffix) $(IntermediateDirectory)/ck802_system$(ObjectSuffix) $(IntermediateDirectory)/ck802_trap_c$(ObjectSuffix) $(IntermediateDirectory)/ck802_vectors$(ObjectSuffix) $(IntermediateDirectory)/misc_jump_table$(ObjectSuffix) $(IntermediateDirectory)/source_clock$(ObjectSuffix) $(IntermediateDirectory)/source_my_printf$(ObjectSuffix) $(IntermediateDirectory)/source_pwrmgr$(ObjectSuffix) $(IntermediateDirectory)/source_watchdog$(ObjectSuffix) \
	$(IntermediateDirectory)/source_flash$(ObjectSuffix) $(IntermediateDirectory)/source_main$(ObjectSuffix) $(IntermediateDirectory)/source_gpio_demo$(ObjectSuffix) $(IntermediateDirectory)/source_gpio_Main$(ObjectSuffix) $(IntermediateDirectory)/source_OSAL_gpio$(ObjectSuffix) $(IntermediateDirectory)/__rt_entry$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) -Wl,--callgraph_file=$(ProjectPath)/Lst/$(OutputFile).htm $(LinkOtherFlagsOption)  -Wl,--ckmap=$(ProjectPath)/Lst/$(OutputFile).map  @$(ObjectsFileList) $(LibraryPathSwitch)E:/Lenze/Git_Projects/ST17H66T_SDK_5.1.0/lib  -Wl,--whole-archive $(LibrarySwitch)rf   -Wl,--no-whole-archive  $(LinkOptions) $(LibPath) $(Libs)
	@mv $(ProjectPath)/Lst/$(OutputFile).map $(ProjectPath)/Lst/$(OutputFile).temp && $(READELF) $(ElfInfoSwitch) $(ProjectPath)/Obj/$(OutputFile)$(ExeSuffix) > $(ProjectPath)/Lst/$(OutputFile).map && echo ====================================================================== >> $(ProjectPath)/Lst/$(OutputFile).map && cat $(ProjectPath)/Lst/$(OutputFile).temp >> $(ProjectPath)/Lst/$(OutputFile).map && rm -rf $(ProjectPath)/Lst/$(OutputFile).temp
	$(OBJCOPY) $(ObjcopySwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  $(ProjectPath)/Obj/$(OutputFile)$(IHexSuffix) 
	$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@$(ProjectName).modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/ck802_board_init$(ObjectSuffix): ck802/board_init.c  Lst/ck802_board_init$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ck802/board_init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ck802_board_init$(ObjectSuffix) -MF$(IntermediateDirectory)/ck802_board_init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ck802_board_init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ck802_board_init$(PreprocessSuffix): ck802/board_init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ck802_board_init$(PreprocessSuffix) ck802/board_init.c

$(IntermediateDirectory)/ck802_startup$(ObjectSuffix): ck802/startup.S  Lst/ck802_startup$(PreprocessSuffix)
	$(AS) $(SourceSwitch) ck802/startup.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ck802_startup$(ObjectSuffix) -MF$(IntermediateDirectory)/ck802_startup$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ck802_startup$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/ck802_startup$(PreprocessSuffix): ck802/startup.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ck802_startup$(PreprocessSuffix) ck802/startup.S

$(IntermediateDirectory)/ck802_system$(ObjectSuffix): ck802/system.c  Lst/ck802_system$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ck802/system.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ck802_system$(ObjectSuffix) -MF$(IntermediateDirectory)/ck802_system$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ck802_system$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ck802_system$(PreprocessSuffix): ck802/system.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ck802_system$(PreprocessSuffix) ck802/system.c

$(IntermediateDirectory)/ck802_trap_c$(ObjectSuffix): ck802/trap_c.c  Lst/ck802_trap_c$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ck802/trap_c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ck802_trap_c$(ObjectSuffix) -MF$(IntermediateDirectory)/ck802_trap_c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ck802_trap_c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ck802_trap_c$(PreprocessSuffix): ck802/trap_c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ck802_trap_c$(PreprocessSuffix) ck802/trap_c.c

$(IntermediateDirectory)/ck802_vectors$(ObjectSuffix): ck802/vectors.S  Lst/ck802_vectors$(PreprocessSuffix)
	$(AS) $(SourceSwitch) ck802/vectors.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ck802_vectors$(ObjectSuffix) -MF$(IntermediateDirectory)/ck802_vectors$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ck802_vectors$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/ck802_vectors$(PreprocessSuffix): ck802/vectors.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ck802_vectors$(PreprocessSuffix) ck802/vectors.S

$(IntermediateDirectory)/misc_jump_table$(ObjectSuffix): ../../../../misc/jump_table.c  Lst/misc_jump_table$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../misc/jump_table.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/misc_jump_table$(ObjectSuffix) -MF$(IntermediateDirectory)/misc_jump_table$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/misc_jump_table$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/misc_jump_table$(PreprocessSuffix): ../../../../misc/jump_table.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/misc_jump_table$(PreprocessSuffix) ../../../../misc/jump_table.c

$(IntermediateDirectory)/source_clock$(ObjectSuffix): ../../../../components/driver/source/clock.c  Lst/source_clock$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../components/driver/source/clock.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_clock$(ObjectSuffix) -MF$(IntermediateDirectory)/source_clock$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_clock$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_clock$(PreprocessSuffix): ../../../../components/driver/source/clock.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_clock$(PreprocessSuffix) ../../../../components/driver/source/clock.c

$(IntermediateDirectory)/source_my_printf$(ObjectSuffix): ../../../../components/driver/source/my_printf.c  Lst/source_my_printf$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../components/driver/source/my_printf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_my_printf$(ObjectSuffix) -MF$(IntermediateDirectory)/source_my_printf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_my_printf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_my_printf$(PreprocessSuffix): ../../../../components/driver/source/my_printf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_my_printf$(PreprocessSuffix) ../../../../components/driver/source/my_printf.c

$(IntermediateDirectory)/source_pwrmgr$(ObjectSuffix): ../../../../components/driver/source/pwrmgr.c  Lst/source_pwrmgr$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../components/driver/source/pwrmgr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_pwrmgr$(ObjectSuffix) -MF$(IntermediateDirectory)/source_pwrmgr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_pwrmgr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_pwrmgr$(PreprocessSuffix): ../../../../components/driver/source/pwrmgr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_pwrmgr$(PreprocessSuffix) ../../../../components/driver/source/pwrmgr.c

$(IntermediateDirectory)/source_watchdog$(ObjectSuffix): ../../../../components/driver/source/watchdog.c  Lst/source_watchdog$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../components/driver/source/watchdog.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_watchdog$(ObjectSuffix) -MF$(IntermediateDirectory)/source_watchdog$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_watchdog$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_watchdog$(PreprocessSuffix): ../../../../components/driver/source/watchdog.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_watchdog$(PreprocessSuffix) ../../../../components/driver/source/watchdog.c

$(IntermediateDirectory)/source_flash$(ObjectSuffix): ../../../../components/driver/source/flash.c  Lst/source_flash$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../../../../components/driver/source/flash.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_flash$(ObjectSuffix) -MF$(IntermediateDirectory)/source_flash$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_flash$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_flash$(PreprocessSuffix): ../../../../components/driver/source/flash.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_flash$(PreprocessSuffix) ../../../../components/driver/source/flash.c

$(IntermediateDirectory)/source_main$(ObjectSuffix): ../source/main.c  Lst/source_main$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../source/main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_main$(ObjectSuffix) -MF$(IntermediateDirectory)/source_main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_main$(PreprocessSuffix): ../source/main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_main$(PreprocessSuffix) ../source/main.c

$(IntermediateDirectory)/source_gpio_demo$(ObjectSuffix): ../source/gpio_demo.c  Lst/source_gpio_demo$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../source/gpio_demo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_gpio_demo$(ObjectSuffix) -MF$(IntermediateDirectory)/source_gpio_demo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_gpio_demo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_gpio_demo$(PreprocessSuffix): ../source/gpio_demo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_gpio_demo$(PreprocessSuffix) ../source/gpio_demo.c

$(IntermediateDirectory)/source_gpio_Main$(ObjectSuffix): ../source/gpio_Main.c  Lst/source_gpio_Main$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../source/gpio_Main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_gpio_Main$(ObjectSuffix) -MF$(IntermediateDirectory)/source_gpio_Main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_gpio_Main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_gpio_Main$(PreprocessSuffix): ../source/gpio_Main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_gpio_Main$(PreprocessSuffix) ../source/gpio_Main.c

$(IntermediateDirectory)/source_OSAL_gpio$(ObjectSuffix): ../source/OSAL_gpio.c  Lst/source_OSAL_gpio$(PreprocessSuffix)
	$(CC) $(SourceSwitch) ../source/OSAL_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_OSAL_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/source_OSAL_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_OSAL_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_OSAL_gpio$(PreprocessSuffix): ../source/OSAL_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_OSAL_gpio$(PreprocessSuffix) ../source/OSAL_gpio.c


$(IntermediateDirectory)/__rt_entry$(ObjectSuffix): $(IntermediateDirectory)/__rt_entry$(DependSuffix)
	@$(AS) $(SourceSwitch) $(ProjectPath)/$(IntermediateDirectory)/__rt_entry.S $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/__rt_entry$(ObjectSuffix) $(IncludeAPath)
$(IntermediateDirectory)/__rt_entry$(DependSuffix):
	@$(CC) $(CFLAGS) $(IncludeAPath) -MG -MP -MT$(IntermediateDirectory)/__rt_entry$(ObjectSuffix) -MF$(IntermediateDirectory)/__rt_entry$(DependSuffix) -MM $(ProjectPath)/$(IntermediateDirectory)/__rt_entry.S

-include $(IntermediateDirectory)/*$(DependSuffix)
