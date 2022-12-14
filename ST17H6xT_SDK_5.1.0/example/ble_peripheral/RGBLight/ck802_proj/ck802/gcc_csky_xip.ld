/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     gcc_csky.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
MEMORY
{
	/*ROM : ORIGIN = 0x1FFF4000  , LENGTH = 0xc000*/   /* ROM  96KB */
	SRAM : ORIGIN = 0x1FFF1000 , LENGTH = 0x7000   /* SRAM  27KB */
	/*SRAM : ORIGIN = 0x20000000 , LENGTH = 0xc000 */  /* SRAM  64KB */
	/*XIPROM : ORIGIN = 0x11032000 , LENGTH = 0x4e000*/   /* XIPROM  512KB */
	XIPROM : ORIGIN = 0x1FFFB000 , LENGTH = 0x1000   /* XIPROM  512KB */
	JUMPTBL : ORIGIN = 0x1FFF0000 , LENGTH = 0x100   /* Jumptable  1KB */
	GCONFIG : ORIGIN = 0x1FFF0100 , LENGTH = 0x100   /* global config  1KB */
}

__min_heap_size = 0x200;
PROVIDE (__ram_end  = 0x20000000);
/*PROVIDE (__ram_end  = 0x2000C000);*/
PROVIDE (__heap_end = __ram_end);

REGION_ALIAS("REGION_TEXT",    XIPROM);
REGION_ALIAS("REGION_RODATA",  SRAM);
REGION_ALIAS("REGION_DATA",    SRAM);
REGION_ALIAS("REGION_BSS",     SRAM);
REGION_ALIAS("REGION_XTEXT",    SRAM);
REGION_ALIAS("REGION_XRODATA",  SRAM);
REGION_ALIAS("REGION_JT",  JUMPTBL);
REGION_ALIAS("REGION_GC",  GCONFIG);

ENTRY(Reset_Handler)
SECTIONS
{

  .jtdata : {
	  *(jump_table_mem_area*)
  } > REGION_JT AT > REGION_JT
  .gcdata : {
	  *(global_config_area*)
  } > REGION_GC AT > REGION_GC
	
 .rodata : {
  . = ALIGN(0x4) ;
  KEEP(*startup.o(.vectors))
   *startup*.o(.text*);
  *system*.o(.text* );
  *board*.o(.text* );
  *vectors*.o(.text* );
  *source_main*.o(.text* );
  *trap*.o(.text* );
  __srodata = .;
  *(.rdata)
  *(.rdata*)
  *(.rdata1)
  *(.rdata.*)
  *(.rodata)
  *(.rodata1)
  *(.rodata*)
  *(.rodata.*)
  *(.rodata.str1.4)
  KEEP (*crtbegin.o(.ctors))
  KEEP (*crtbegin?.o(.ctors))
  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
  KEEP (*(SORT(.ctors.*)))
  KEEP (*(.ctors))
  KEEP (*crtbegin.o(.dtors))
  KEEP (*crtbegin?.o(.dtors))
  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
  KEEP (*(SORT(.dtors.*)))
  KEEP (*(.dtors))
  . = ALIGN(0x4) ;
  __erodata = .;
  __rodata_end__ = .;
 } > REGION_RODATA
 .data : {
  . = ALIGN(0x4) ;
  __sdata = . ;
  __data_start__ = . ;
  data_start = . ;
  *jump*.o(.text* );
  *rf*.o(.text* );
  *main*.o(.text* );
  *pwrmgr*.o(.text* );
  *watchdog*.o(.text* );
  *clock*.o(.text* );
  *flash*.o(.text* );
  *my_printf*.o(.text* );
  *patch*.o(.text* );
  *OSAL_SimpleBLEPeripheral*.o(.text* );
  *peripheral*.o(.text* );
  *(.got.plt)
  *(.got)
  *(.gnu.linkonce.r*)
  *(.data)
  *(.data*)
  *(.data1)
  *(.data.*)
  *(.gnu.linkonce.d*)
  *(.data1)
  *(.gcc_except_table)
  *(.gcc_except_table*)
  __start_init_call = .;
  *(.initcall.init)
  __stop_init_call = .;
  __start_cmd = .;
  *(.bootloaddata.cmd)
  . = ALIGN(4) ;
  __stop_cmd = .;
  *(.sdata)
  *(.sdata.*)
  *(.gnu.linkonce.s.*)
  *(__libc_atexit)
  *(__libc_subinit)
  *(__libc_subfreeres)
  *(.note.ABI-tag)
  __edata = .;
  __data_end__ = .;
  . = ALIGN(0x4) ;
 } > REGION_DATA AT > REGION_RODATA
 .eh_frame : ONLY_IF_RW {
  KEEP (*(.eh_frame))
 } > REGION_DATA AT > REGION_RODATA
 .gcc_except_table : ONLY_IF_RW {
  *(.gcc_except_table .gcc_except_table.*)
  __edata = .;
  __data_end__ = .;
 } > REGION_DATA AT > REGION_RODATA
 .bss : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_start__ = . ;
  *(.dynsbss)
  *(.sbss)
  *(.sbss.*)
  *(.scommon)
  *(.dynbss)
  *(.bss)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __bss_end__ = .;
  __end = . ;
  end = . ;
 } > REGION_BSS AT > REGION_BSS
 ._user_heap : {
  . = ALIGN(0x4) ;
  __heap_start = .;
  . += __min_heap_size;
  . = ALIGN(0x4) ;
 } > REGION_BSS AT > REGION_BSS
 .text : {
  . = ALIGN(0x4) ;
  /*
  KEEP(*startup.o(.vectors))
  */
  __stext = . ;
  *(.text)
  *(.text*)
  *(.text.*)
  *(.gnu.warning)
  *(.stub)
  *(.gnu.linkonce.t*)
  *(.glue_7t)
  *(.glue_7)
  *(.jcr)
  KEEP (*(.init))
  KEEP (*(.fini))
  . = ALIGN (4) ;
  PROVIDE(__ctbp = .);
  *(.call_table_data)
  *(.call_table_text)
  . = ALIGN(0x10) ;
  __etext = . ;
 } > REGION_TEXT
 .eh_frame_hdr : {
  *(.eh_frame_hdr)
 } > REGION_TEXT
 .eh_frame : ONLY_IF_RO {
  KEEP (*(.eh_frame))
 } > REGION_TEXT
 .gcc_except_table : ONLY_IF_RO {
  *(.gcc_except_table .gcc_except_table.*)
 } > REGION_TEXT
}
