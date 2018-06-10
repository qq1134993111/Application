//https://opensource.org/licenses/MIT:
/*Copyright 2017 canosw.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
//c_predefined.h

#ifndef C_PREDEFINED_H
#define C_PREDEFINED_H

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

//Architecture
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define C_ARCHITECTURE_X86_64 1
#define C_ARCHITECTURE "x86_64"
//#endif //defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64 || defined(_M_AMD64)
#elif defined(__aarch64__)
#define C_ARCHITECTURE_ARM64 1
#define C_ARCHITECTURE "ARM64"
//#endif //defined(__aarch64__)
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__i386) || defined(__i386) || defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__I86__) || defined(__INTEL__) || defined(__386) || defined(_M_IX86) || defined(__I86__)
#define C_ARCHITECTURE_I386 1
#define C_ARCHITECTURE "i386"
//#endif //defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__i386) || defined(__i386) || defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__I86__) || defined(__INTEL__) || defined(__386) || defined(_M_IX86) || defined(__I86__)
#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__arm) || defined(_M_ARM)defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T) || defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define C_ARCHITECTURE_ARM 1
#define C_ARCHITECTURE "ARM"
//#endif //defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__arm) || defined(_M_ARM)defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T) || defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#elif defined(__mips__) || defined(mips) || defined(_R3000) || defined(_R4000) || defined(_R5900) || defined(__mips) || defined(__mips) || defined(__MIPS__) || defined(_MIPS_ISA)defined(__mips)defined(_MIPS_ISA_MIPS1) || defined(_MIPS_ISA_MIPS1) || defined(_R3000) || defined(_MIPS_ISA_MIPS2) || defined(__MIPS_ISA2__) || defined(_R4000) || defined(_MIPS_ISA_MIPS3) || defined(__MIPS_ISA3__) || defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__) || defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__)
#define C_ARCHITECTURE_MIPS 1
#define C_ARCHITECTURE "MIPS"
//#endif //defined(__mips__) || defined(mips) || defined(_R3000) || defined(_R4000) || defined(_R5900) || defined(__mips) || defined(__mips) || defined(__MIPS__) || defined(_MIPS_ISA)defined(__mips)defined(_MIPS_ISA_MIPS1) || defined(_MIPS_ISA_MIPS1) || defined(_R3000) || defined(_MIPS_ISA_MIPS2) || defined(__MIPS_ISA2__) || defined(_R4000) || defined(_MIPS_ISA_MIPS3) || defined(__MIPS_ISA3__) || defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__) || defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__)
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(_M_PPC) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc) || defined(_M_PPC)defined(_ARCH_440) || defined(_ARCH_450) || defined(__ppc601__) || defined(_ARCH_601) || defined(__ppc603__) || defined(_ARCH_603) || defined(__ppc604__) || defined(_ARCH_604)
#define C_ARCHITECTURE_POWERPC 1
#define C_ARCHITECTURE "PowerPC"
//#endif //defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(_M_PPC) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc) || defined(_M_PPC)defined(_ARCH_440) || defined(_ARCH_450) || defined(__ppc601__) || defined(_ARCH_601) || defined(__ppc603__) || defined(_ARCH_603) || defined(__ppc604__) || defined(_ARCH_604)
#elif defined(__sparc) || defined(__sparc_v8__) || defined(__sparc_v9__) || defined(__sparcv8) || defined(__sparcv9) || defined(__sparcv8) || defined(__sparc_v8__) || defined(__sparcv9) || defined(__sparc_v9__)
#define C_ARCHITECTURE_SPARC 1
#define C_ARCHITECTURE "SPARC"
#endif //defined(__sparc) || defined(__sparc_v8__) || defined(__sparc_v9__) || defined(__sparcv8) || defined(__sparcv9) || defined(__sparcv8) || defined(__sparc_v8__) || defined(__sparcv9) || defined(__sparc_v9__)

#if C_PREDEFINED_OBSCURE_ARCHITECTURES == 1 //Vintage or obscure architectures
#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA) || defined(__alpha_ev4__) || defined(__alpha_ev5__) || defined(__alpha_ev6__)
#define C_ARCHITECTURE_ALPHA 1
#define C_ARCHITECTURE "Alpha"
#endif //defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA) || defined(__alpha_ev4__) || defined(__alpha_ev5__) || defined(__alpha_ev6__)
#if defined(__bfin) || defined(__BFIN__)
#define C_ARCHITECTURE_BLACKFIN 1
#define C_ARCHITECTURE "Blackfin"
#endif //defined(__bfin) || defined(__BFIN__)
#if defined(__convex__) || defined(__convex_c1__) || defined(__convex_c2__) || defined(__convex_c32__) || defined(__convex_c34__) || defined(__convex_c38__)
#define C_ARCHITECTURE_CONVEX 1
#define C_ARCHITECTURE "Convex"
#endif //defined(__convex__) || defined(__convex_c1__) || defined(__convex_c2__) || defined(__convex_c32__) || defined(__convex_c34__) || defined(__convex_c38__)
#if defined(__epiphany__)
#define C_ARCHITECTURE_EPIPHANY 1
#define C_ARCHITECTURE "Epiphany"
#endif //defined(__epiphany__)
#if defined(__hppa__) || defined(__HPPA__) || defined(__hppa) || defined(_PA_RISC1_0) || defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__) || defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || defined(__PA8000__)
#define C_ARCHITECTURE_HPPA 1
#define C_ARCHITECTURE "HP/PA RISC"
#endif //defined(__hppa__) || defined(__HPPA__) || defined(__hppa) || defined(_PA_RISC1_0) || defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__) || defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || defined(__PA8000__)
#if defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(_M_IA64) || defined(_M_IA64) || defined(__itanium__) || defined(_M_IA64)
#define C_ARCHITECTURE_IA64 1
#define C_ARCHITECTURE "IA64"
#endif //defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(_M_IA64) || defined(_M_IA64) || defined(__itanium__) || defined(_M_IA64)
#if defined(__m68k__) ||  || defined(M68000) || defined(__MC68K__) || defined(__mc68000__) || defined(__MC68000__) || defined(__mc68010__) || defined(__mc68020__) || defined(__MC68020__) || defined(__mc68030__) || defined(__MC68030__) || defined(__mc68040__) || defined(__mc68060__) 
#define C_ARCHITECTURE_M68K 1
#define C_ARCHITECTURE "M68K"
#endif //defined(__m68k__) ||  || defined(M68000) || defined(__MC68K__) || defined(__mc68000__) || defined(__MC68000__) || defined(__mc68010__) || defined(__mc68020__) || defined(__MC68020__) || defined(__mc68030__) || defined(__MC68030__) || defined(__mc68040__) || defined(__mc68060__) 
#if defined(pyr)
#define C_ARCHITECTURE_PYRAMID 1
#define C_ARCHITECTURE "Pyramid 9810"
#endif //defined(pyr)
#if defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || defined(_ARCH_PWR4) || defined(__sparc__)
#define C_ARCHITECTURE_RS6000 1
#define C_ARCHITECTURE "RS/6000"
#endif //defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || defined(_ARCH_PWR4) || defined(__sparc__)
#if defined(__sh__) || defined(__sh1__) || defined(__sh2__) || defined(__sh3__) || defined(__SH3__) || defined(__SH4__) || defined(__SH5__)
#define C_ARCHITECTURE_SUPERH 1
#define C_ARCHITECTURE "SuperH"
#endif //defined(__sh__) || defined(__sh1__) || defined(__sh2__) || defined(__sh3__) || defined(__SH3__) || defined(__SH4__) || defined(__SH5__)
#if defined(__370__) || defined(__THW_370__) || defined(__s390__) || defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#define C_ARCHITECTURE_SYSTEMZ 1
#define C_ARCHITECTURE "SystemZ"
#endif //defined(__370__) || defined(__THW_370__) || defined(__s390__) || defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#if defined(_TMS320C2XX) || defined(__TMS320C2000__) || defined(_TMS320C5X) || defined(__TMS320C55X__) || defined(_TMS320C6X) || defined(__TMS320C6X__) || defined(_TMS320C28X) || defined(_TMS320C5XX) || defined(__TMS320C55X__) || defined(_TMS320C6200) || defined(_TMS320C6400) || defined(_TMS320C6400_PLUS) || defined(_TMS320C6600) || defined(_TMS320C6700) || defined(_TMS320C6700_PLUS) || defined(_TMS320C6740)
#define C_ARCHITECTURE_TMS320 1
#define C_ARCHITECTURE "TMS320"
#endif //defined(_TMS320C2XX) || defined(__TMS320C2000__) || defined(_TMS320C5X) || defined(__TMS320C55X__) || defined(_TMS320C6X) || defined(__TMS320C6X__) || defined(_TMS320C28X) || defined(_TMS320C5XX) || defined(__TMS320C55X__) || defined(_TMS320C6200) || defined(_TMS320C6400) || defined(_TMS320C6400_PLUS) || defined(_TMS320C6600) || defined(_TMS320C6700) || defined(_TMS320C6700_PLUS) || defined(_TMS320C6740)
#if defined(__TMS470__)
#define C_ARCHITECTURE_TMS470 1
#define C_ARCHITECTURE "TMS470"
#endif //defined(__TMS470__)
#endif //C_PREDEFINED_OBSCURE_ARCHITECTURES == 1 //Vintage or obscure architectures.

//COMPILER
#if defined(__MINGW32__) || defined(__MINGW32_MAJOR_VERSION) || defined(__MINGW32_MINOR_VERSION) || defined(__MINGW64__) || defined(__MINGW64_VERSION_MAJOR) || defined(__MINGW64_VERSION_MINOR)
#define C_COMPILER_MINGW 1
#define C_COMPILER "MinGW"
//#endif //defined(__MINGW32__) || defined(__MINGW32_MAJOR_VERSION) || defined(__MINGW32_MINOR_VERSION) || defined(__MINGW64__) || defined(__MINGW64_VERSION_MAJOR) || defined(__MINGW64_VERSION_MINOR)
#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#define C_COMPILER_MICROSOFT_VISUAL 1
#define C_COMPILER "Microsoft Visual C++"
//#endif //defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#elif defined(__xlc__) || defined(__xlC__) || defined(__xlC_ver__)
#define C_COMPILER_IBM_XL 1
#define C_COMPILER "IBM XL C/C++"
//#endif //defined(__xlc__) || defined(__xlC__) || defined(__xlC_ver__)
#elif defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC) || defined(__ICL) || defined(__INTEL_COMPILER_BUILD_DATE)
#define C_COMPILER_INTEL 1
#define C_COMPILER "Intel C/C++"
//#endif //defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC) || defined(__ICL) || defined(__INTEL_COMPILER_BUILD_DATE)
#elif defined(__HP_aCC)
#define C_COMPILER_HP_ACPP 1
#define C_COMPILER "HP aC++"
//#endif //defined(__HP_aCC)
#if defined(__PGI) || defined(__PGIC__) || defined(__PGIC_MINOR__) || defined(__PGIC_PATCHLEVEL__)
#define C_COMPILER_PORTLAND_GROUP 1
#define C_COMPILER "Portland Group"
#endif //defined(__PGI) || defined(__PGIC__) || defined(__PGIC_MINOR__) || defined(__PGIC_PATCHLEVEL__)
#elif defined(__clang__) || defined(__clang_major__) || defined(__clang_minor__) || defined(__clang_patchlevel__)
#define C_COMPILER_CLANG 1
#define C_COMPILER "Clang"
//#endif //defined(__clang__) || defined(__clang_major__) || defined(__clang_minor__) || defined(__clang_patchlevel__)
#elif defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__) || defined(__GNUC_VERSION__)
#define C_COMPILER_GCC 1
#define C_COMPILER "GCC"
//#endif //defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__) || defined(__GNUC_VERSION__)
#elif defined(ORA_PROC)
#define C_COMPILER_ORACLE_PRO 1
#define C_COMPILER "Oracle Pro C"
//#endif //defined(ORA_PROC)
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#define C_COMPILER_SUN_PRO 1
#define C_COMPILER "Sun Pro"
//#endif //defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#elif defined(__llvm__)
#define C_COMPILER_LLVM 1
#define C_COMPILER "llvm"
//#endif //defined(__llvm__)
#elif defined(__HP_cc)
#define C_COMPILER_HP_ANSI 1
#define C_COMPILER "HP ANSI C"
//#endif //defined(__HP_cc)
#elif defined(__TINYC__)
#define C_COMPILER_TINYC 1
#define C_COMPILER "Tiny C"
//#endif //defined(__TINYC__)
#elif defined(__MWERKS__) || defined(__CWCC__)
#define C_COMPILER_METROWERKS_CODEWARRIOR 1
#define C_COMPILER "Metrowerks CodeWarrior"
//#endif //defined(__MWERKS__) || defined(__CWCC__)
#elif defined(__BORLANDC__) || defined(__CODEGEARC__)
#define C_COMPILER_BORLAND 1
#define C_COMPILER "Borland C++"
#endif //defined(__BORLANDC__) || defined(__CODEGEARC__)

#if C_PREDEFINED_OBSCURE_COMPILERS == 1 //Vintage and esoteric C compilers
#if defined(_ACC_)
#define C_COMPILER_ACC 1
#define C_COMPILER "ACC"
#endif //defined(_ACC_)
#if defined(__CMB__)
# defined(__VERSION__) || defined(__REVISION__) || defined(__BUILD__)
#define C_COMPILER_ALTIUM_MICROBLAZE 1
#define C_COMPILER "Altium MicroBlaze C"
#endif //defined(__CMB__) || defined(__VERSION__) || defined(__REVISION__) || defined(__BUILD__)
#if defined(__CHC__)
# defined(__VERSION__) || defined(__REVISION__) || defined(__BUILD__)
#define C_COMPILER_ALTIUM_C_TO_HARDWARE 1
#define C_COMPILER "Altium C-to-Hardware"
#endif //defined(__CHC__) || defined(__VERSION__) || defined(__REVISION__) || defined(__BUILD__)
#if defined(__ACK__)
#define C_COMPILER_AMSTERDAM_COMPILER_KIT 1
#define C_COMPILER "Amsterdam Compiler Kit"
#endif //defined(__ACK__)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define C_COMPILER_ARM_COMPILER 1
#define C_COMPILER "ARM Compiler"
#endif //defined(__CC_ARM) || defined(__ARMCC_VERSION)
#if defined(AZTEC_C) || defined(__AZTEC_C__)
# defined(__VERSION)
#define C_COMPILER_AZTEC 1
#define C_COMPILER "Aztec C"
#endif //defined(AZTEC_C) || defined(__AZTEC_C__) || defined(__VERSION)
#if defined(__CC65__)
#define C_COMPILER_CC65 1
#define C_COMPILER "CC65"
#endif //defined(__CC65__)
#if defined(__COMO__) || defined(__COMO_VERSION__)
#define C_COMPILER_COMEAU 1
#define C_COMPILER "Comeau C++"
#endif //defined(__COMO__) || defined(__COMO_VERSION__)
#if defined(__DECC) || defined(__DECC_VER) || defined(__DECCXX) || defined(__DECCXX_VER) || defined(__VAXC) || defined(VAXC)
#define C_COMPILER_COMPAQ 1
#define C_COMPILER "Compaq C/C++"
#endif //defined(__DECC) || defined(__DECC_VER) || defined(__DECCXX) || defined(__DECCXX_VER) || defined(__VAXC) || defined(VAXC)
#if defined(__convexc__)
#define C_COMPILER_CONVEX 1
#define C_COMPILER "Convex C"
#endif //defined(__convexc__)
#if defined(__COMPCERT__)
#define C_COMPILER_COMPCERT 1
#define C_COMPILER "CompCert"
#endif //defined(__COMPCERT__)
#if defined(__COVERITY__)
#define C_COMPILER_COVERITY 1
#define C_COMPILER "Coverity C/C++"
#endif //defined(__COVERITY__)
#if defined(_CRAYC)
# defined(_RELEASE) || defined(_RELEASE_MINOR)
#define C_COMPILER_CRAY 1
#define C_COMPILER "Cray C"
#endif //defined(_CRAYC) || defined(_RELEASE) || defined(_RELEASE_MINOR)
#if defined(__DCC__)
# defined(__VERSION_NUMBER__)
#define C_COMPILER_DIAB 1
#define C_COMPILER "Diab C/C++"
#endif //defined(__DCC__) || defined(__VERSION_NUMBER__)
#if defined(_DICE)
#define C_COMPILER_DICE 1
#define C_COMPILER "DICE C"
#endif //defined(_DICE)
#if defined(__DMC__)
#define C_COMPILER_DIGITAL_MARS 1
#define C_COMPILER "Digital Mars"
#endif //defined(__DMC__)
#if defined(__SYSC__) || defined(__SYSC_VER__)
#define C_COMPILER_DIGNUS_SYSTEMS 1
#define C_COMPILER "Dignus Systems C++"
#endif //defined(__SYSC__) || defined(__SYSC_VER__)
#if defined(__DJGPP__) || defined(__DJGPP_MINOR__) || defined(__GO32__)
#define C_COMPILER_DJGPP 1
#define C_COMPILER "DJGPP"
#endif //defined(__DJGPP__) || defined(__DJGPP_MINOR__) || defined(__GO32__)
#if defined(__EDG__) || defined(__EDG_VERSION__)
#define C_COMPILER_EDG 1
#define C_COMPILER "EDG C++"
#endif //defined(__EDG__) || defined(__EDG_VERSION__)
#if defined(__PATHCC__) || defined(__PATHCC_MINOR__) || defined(__PATHCC_PATCHLEVEL__)
#define C_COMPILER_EKOPATH 1
#define C_COMPILER "EKOPath"
#endif //defined(__PATHCC__) || defined(__PATHCC_MINOR__) || defined(__PATHCC_PATCHLEVEL__)
#if defined(__FCC_VERSION)
#define C_COMPILER_FUJITSU 1
#define C_COMPILER "Fujitsu C++"
#endif //defined(__FCC_VERSION)
#if defined(__ghs__) || defined(__GHS_VERSION_NUMBER__) || defined(__GHS_REVISION_DATE__) || defined(__GHS_VERSION_NUMBER__)
#define C_COMPILER_GREEN_HILL 1
#define C_COMPILER "Green Hill C/C++"
#endif //defined(__ghs__) || defined(__GHS_VERSION_NUMBER__) || defined(__GHS_REVISION_DATE__) || defined(__GHS_VERSION_NUMBER__)
#if defined(__IAR_SYSTEMS_ICC__) || defined(__VER__)
#define C_COMPILER_IAR 1
#define C_COMPILER "IAR C/C++"
#endif //defined(__IAR_SYSTEMS_ICC__) || defined(__VER__)
#if defined(__IBMC__) || defined(__IBMCPP__)
# defined(__COMPILER_VER__)
#define C_COMPILER_IBM_ZOS
#define C_COMPILER "IBM z/OS C/C++"
#endif //defined(__IBMC__) || defined(__IBMCPP__) || defined(__COMPILER_VER__)
#if defined(__IMAGECRAFT__)
#define C_COMPILER_IMAGECRAFT 1
#define C_COMPILER "Imagecraft C"
#endif //defined(__IMAGECRAFT__)
#if defined(__KCC) || defined(__KCC_VERSION)
#define C_COMPILER_KAI 1
#define C_COMPILER "KAI C++"
#endif //defined(__KCC) || defined(__KCC_VERSION)
#if defined(__CA__) || defined(__KEIL__)
#define C_COMPILER_KEIL_CARM 1
#define C_COMPILER "KEIL CARM"
#endif //defined(__CA__) || defined(__KEIL__)
#if defined(__C166__)
#define C_COMPILER_KEIL_C166 1
#define C_COMPILER "KEIL C166"
#endif //defined(__C166__)
#if defined(__C51__) || defined(__CX51__)
#define C_COMPILER_KEIL_C51 1
#define C_COMPILER "KEIL C51"
#endif //defined(__C51__) || defined(__CX51__)
#if defined(__LCC__)
#define C_COMPILER_LLC 1
#define C_COMPILER "LLC"
#endif //defined(__LCC__)
#if defined(__HIGHC__)
#define C_COMPILER_METAWARE_HIGH 1
#define C_COMPILER "MetaWare High C/C++"
#endif //defined(__HIGHC__)
#if defined(_MRI)
#define C_COMPILER_MICROTEC 1
#define C_COMPILER "Microtec C/C++"
#endif //defined(_MRI)
#if defined(__NDPC__) || defined(__NDPX__)
#define C_COMPILER_MICROWAY_NDP 1
#define C_COMPILER "Microway NDP C"
#endif //defined(__NDPC__) || defined(__NDPX__)
#if defined(__sgi) || defined(sgi) || defined(_SGI_COMPILER_VERSION)
# defined(_COMPILER_VERSION)
#define C_COMPILER_MIPSPRO 1
#define C_COMPILER "MIPSPro"
#endif //defined(__sgi) || defined(sgi) || defined(_COMPILER_VERSION) || defined(_SGI_COMPILER_VERSION)
#if defined(MIRACLE)
#define C_COMPILER_MIRACLE 1
#define C_COMPILER "Miracle"
#endif //defined(MIRACLE)
#if defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS)
#define C_COMPILER_MPW 1
#define C_COMPILER "MPW"
#endif //defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS)
#if defined(__CC_NORCROFT) || defined(__ARMCC_VERSION)
#define C_COMPILER_NORCROFT 1
#define C_COMPILER "Norcroft"
#endif //defined(__CC_NORCROFT) || defined(__ARMCC_VERSION)
#if defined(__NWCC__)
#define C_COMPILER_NWCC 1
#define C_COMPILER "NWCC"
#endif //defined(__NWCC__)
#if defined(__OPEN64__) || defined(__OPENCC__) || defined(__OPENCC_MINOR__) || defined(__OPENCC_PATCHLEVEL__)
#define C_COMPILER_OPEN64 1
#define C_COMPILER "Open64"
#endif //defined(__OPEN64__) || defined(__OPENCC__) || defined(__OPENCC_MINOR__) || defined(__OPENCC_PATCHLEVEL__)
#if defined(__PACIFIC__) || defined(_PACC_VER)
#define C_COMPILER_PACIFIC 1
#define C_COMPILER "Pacific"
#endif //defined(__PACIFIC__) || defined(_PACC_VER)
#if defined(__POCC__) 
#define C_COMPILER_PALM 1
#define C_COMPILER "Palm C"
#endif //defined(__POCC__) 
#if defined(__RENESAS__) || defined(__HITACHI__) || defined(__RENESAS_VERSION__) || defined(__HITACHI_VERSION__)
#define C_COMPILER_RENESAS 1
#define C_COMPILER "Renesas"
#endif //defined(__RENESAS__) || defined(__HITACHI__) || defined(__RENESAS_VERSION__) || defined(__HITACHI_VERSION__)
#if defined(SASC) || defined(__SASC) || defined(__SASC__)
# defined(__VERSION__) || defined(__REVISION__)
#define C_COMPILER_SAS 1
#define C_COMPILER "SAS/C"
#endif //defined(SASC) || defined(__SASC) || defined(__SASC__) || defined(__VERSION__) || defined(__REVISION__)
#if defined(_SCO_DS)
#define C_COMPILER_SCO 1
#define C_COMPILER "SCO OpenServer"
#endif //defined(_SCO_DS)
#if defined(SDCC)
#define C_COMPILER_SDCC 1
#define C_COMPILER "Small Device C Compiler"
#endif //defined(SDCC)
#if defined(__SNC__)
#define C_COMPILER_SN 1
#define C_COMPILER "SN Compiler"
#endif //defined(__SNC__)
#if defined(__VOSC__)
#define C_COMPILER_STRATOS 1
#define C_COMPILER "Stratos VOS"
#endif //defined(__VOSC__)
#if defined(__SC__)
#define C_COMPILER_SYMANTEC 1
#define C_COMPILER "Symantec C"
#endif //defined(__SC__)
#if defined(__TenDRA__)
#define C_COMPILER_TENDRA 1
#define C_COMPILER "TenDRA C/C++"
#endif //defined(__TenDRA__)
#if defined(__TI_COMPILER_VERSION__) || defined(_TMS320C6X)
#define C_COMPILER_TEXAS_INSTRUMENTS 1
#define C_COMPILER "Texas Instruments C/C++"
#endif //defined(__TI_COMPILER_VERSION__) || defined(_TMS320C6X)
#if defined(THINKC3) || defined(THINKC4)
#define C_COMPILER_THINKC 1
#define C_COMPILER "THINK C"
#endif //defined(THINKC3) || defined(THINKC4)
#if defined(__TURBOC__)
#define C_COMPILER_TURBOS 1
#define C_COMPILER "Turbo C/C++"
#endif //defined(__TURBOC__)
#if defined(_UCC) || defined(_MAJOR_REV) || defined(_MINOR_REV)
#define C_COMPILER_ULTIMATE 1
#define C_COMPILER "Ultimate C/C++"
#endif //defined(_UCC) || defined(_MAJOR_REV) || defined(_MINOR_REV)
#if defined(__USLC__) || defined(__SCO_VERSION__) 
#define C_COMPILER_USL 1
#define C_COMPILER "USL C"
#endif //defined(__USLC__) || defined(__SCO_VERSION__) 
#if defined(__VBCC__)
#define C_COMPILER_VBCC 1
#define C_COMPILER "VBCC"
#endif //defined(__VBCC__)
#if defined(__WATCOMC__)
#define C_COMPILER_WATCOM 1
#define C_COMPILER "Watcom C++"
#endif //defined(__WATCOMC__)
#if defined(__ZTC__)
#define C_COMPILER_ZORTECH 1
#define C_COMPILER "Zortech C++"
#endif //defined(__ZTC__)
#endif //C_PREDEFINED_OBSCURE_COMPILERS == 1 //Vintage and esoteric C Compilers

//SYSTEM
#if defined(__CYGWIN__)
#define C_SYSTEM_CYGWIN 1
#define C_SYSTEM "Cygwin"
//#endif //defined(__CYGWIN__)
#elif defined(_WIN32_WCE) || defined(WIN32_PLATFORM_HPC2000) || defined(WIN32_PLATFORM_HPCPRO) || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define C_SYSTEM_WINDOWSCE 1
#define C_SYSTEM "Windows CE"
//#endif //defined(_WIN32_WCE) || defined(WIN32_PLATFORM_HPC2000) || defined(WIN32_PLATFORM_HPCPRO) || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define C_SYSTEM_WINDOWS 1
#define C_SYSTEM "Windows"
//#endif //defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#elif defined(__APPLE__) && defined(__MACH__)
#define C_SYSTEM_APPLE 1
#define C_SYSTEM "Apple (mac/i/tv)OS"
//#endif //defined(__APPLE__) && defined(__MACH__)
#elif defined(__ANDROID__) || defined(__ANDROID_API__)
#define C_SYSTEM_ANDROID 1
#define C_SYSTEM "Android"
//#endif //defined(__ANDROID__) || defined(__ANDROID_API__)
#elif defined(__OpenBSD__) || defined(OpenBSD3_1) || defined(OpenBSD3_9)
#define C_SYSTEM_OPENBSD 1
#define C_SYSTEM "OpenBSD"
//#endif //defined(__OpenBSD__) || defined(OpenBSD3_1) || defined(OpenBSD3_9)
#elif defined(__NetBSD__) || defined(__NetBSD_Version__) || defined(NetBSD0_8) || defined(NetBSD0_9) || defined(NetBSD1_0) || defined(NetBSD1_0)
#define C_SYSTEM_NETBSD 1
#define C_SYSTEM "NetBSD"
//#endif //defined(__NetBSD__) || defined(__NetBSD_Version__) || defined(NetBSD0_8) || defined(NetBSD0_9) || defined(NetBSD1_0) || defined(NetBSD1_0)
#elif defined(__gnu_hurd__)
#define C_SYSTEM_GNU_HURD 1
#define C_SYSTEM "GNU/Hurd"
//#endif //defined(__gnu_hurd__)
#elif defined(sun) || defined(__sun) || defined(__SunOS_5_7) || defined(__SunOS_5_8)
#define C_SYSTEM_SOLARIS 1
#define C_SYSTEM "Sun Solaris"
//#endif //defined(sun) || defined(__sun) || defined(__SunOS_5_7) || defined(__SunOS_5_8)
#elif defined(__FreeBSD_kernel__) && defined(__GLIBC__)
#define C_SYSTEM_FREEBSD_KERNEL 1
#define C_SYSTEM "GNU/FreeBSD Kernel"
//#endif //defined(__FreeBSD_kernel__) && defined(__GLIBC__)
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__FreeBSD_version)
#define C_SYSTEM_FREEBSD 1
#define C_SYSTEM "FreeBSD"
//#endif //defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__FreeBSD_version)
#elif defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#define C_SYSTEM_MSDOS 1
#define C_SYSTEM "MSDOS"
//#endif //defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#elif defined(macintosh) || defined(Macintosh)
#define C_SYSTEM_MACINTOSH 1
#define C_SYSTEM "Macintosh"
//#endif //defined(macintosh) || defined(Macintosh)
#elif defined(__Lynx__)
#define C_SYSTEM_LYNX 1
#define C_SYSTEM "Lynx"
//#endif //defined(__Lynx__)
#elif defined(__minix)
#define C_SYSTEM_MINIX 1
#define C_SYSTEM "Minix"
//#endif //defined(__minix)
#elif defined(__QNX__) || defined(__QNXNTO__) || defined(_NTO_VERSION) ||defined(BBNDK_VERSION_CURRENT)
#define C_SYSTEM_QNX 1
#define C_SYSTEM "QNX"
//#endif //defined(__QNX__) || defined(__QNXNTO__) || defined(_NTO_VERSION) ||defined(BBNDK_VERSION_CURRENT)
#elif defined(__bsdi__)
#define C_SYSTEM_BSDI 1
#define C_SYSTEM "BSD/OS"
//#endif //defined(__bsdi__)
#elif defined(__gnu_linux__)
#define C_SYSTEM_GNU_LINUX 1
#define C_SYSTEM "GNU/Linux"
//#endif //defined(__gnu_linux__)
#elif defined(BSD) || defined(BSD4_2) || defined(BSD4_3) || defined(BSD4_4) || defined(_SYSTYPE_BSD)
#define C_SYSTEM_BSD 1
#define C_SYSTEM "BSD"
//#endif //defined(BSD) || defined(BSD4_2) || defined(BSD4_3) || defined(BSD4_4) || defined(_SYSTYPE_BSD)
#elif defined(__GNU__) || defined(__GLIBC__)
#define C_SYSTEM_GNU 1
#define C_SYSTEM "GNU"
//#endif //defined(__GNU__) || defined(__GLIBC__)
#elif defined(__linux__) || defined(linux) || defined(__linux)
#define C_SYSTEM_LINUX 1
#define C_SYSTEM "Linux"
//#endif //defined(__linux__) || defined(linux) || defined(__linux)
#elif defined(__unix__) || defined(__unix)
#define C_SYSTEM_UNIX 1
#define C_SYSTEM "Unix"
#endif //defined(__unix__) || defined(__unix)

#if C_PREDEFINED_OBSCURE_SYSTEMS == 1 //Obscure systems
#if defined(_AIX) || defined(__TOS_AIX__) || defined(_AIX3) || defined(_AIX32) || defined(_AIX41) || defined(_AIX43)
#define C_SYSTEM_AIX 1
#define C_SYSTEM "AIX"
#endif //defined(_AIX) || defined(__TOS_AIX__) || defined(_AIX3) || defined(_AIX32) || defined(_AIX41) || defined(_AIX43)
#if defined(UTS)
#define C_SYSTEM_AMDAHL_UTS 1
#define C_SYSTEM "Amdahl UTS"
#endif //defined(UTS)
#if defined(AMIGA) || defined(__amigaos__)
#define C_SYSTEM_AMIGA 1
#define C_SYSTEM "AmigaOS"
#endif //defined(AMIGA) || defined(__amigaos__)
#if defined(aegis)
#define C_SYSTEM_AEGIS 1
#define C_SYSTEM "Apollo AEGIS"
#endif //defined(aegis)
#if defined(apollo)
#define C_SYSTEM_APOLLO 1
#define C_SYSTEM "Apollo Domain/OS"
#endif //defined(apollo)
#if defined(__BEOS__)
#define C_SYSTEM_BEOS 1
#define C_SYSTEM "BeOS"
#endif //defined(__BEOS__)
#if defined(__bg__) || defined(__bgq__) || defined(__THW_BLUEGENE__) || defined(__TOS_BGQ__)
#define C_SYSTEM_BLUE_GENE 1
#define C_SYSTEM "Blue Gene"
#endif //defined(__bg__) || defined(__bgq__) || defined(__THW_BLUEGENE__) || defined(__TOS_BGQ__)
#if defined(__convex__)
#define C_SYSTEM_CONVEX 1
#define C_SYSTEM "ConvexOS"
#endif //defined(__convex__)
#if defined(DGUX) || defined(__DGUX__) || defined(__dgux__)
#define C_SYSTEM_DGUX 1
#define C_SYSTEM "DG/UX"
#endif //defined(DGUX) || defined(__DGUX__) || defined(__dgux__)
#if defined(__DragonFly__)
#define C_SYSTEM_DRAGONFLY 1
#define C_SYSTEM "DragonFly"
#endif //defined(__DragonFly__)
#if defined(_SEQUENT_) || defined(sequent)
#define C_SYSTEM_DYNIX 1
#define C_SYSTEM "DYNIX/ptx (SEQUENT)"
#endif //defined(_SEQUENT_) || defined(sequent)
#if defined(__ECOS)
#define C_SYSTEM_ECOS 1
#define C_SYSTEM "eCos"
#endif //defined(__ECOS)
#if defined(__EMX__)
#define C_SYSTEM_EMX 1
#define C_SYSTEM "EMX Environment"
#endif //defined(__EMX__)
#if defined(__hiuxmpp)
#define C_SYSTEM_HIUXMPP 1
#define C_SYSTEM "HI-UX MPP"
#endif //defined(__hiuxmpp)
#if defined(_hpux) || defined(hpux) || defined(__hpux)
#define C_SYSTEM_HPUX 1
#define C_SYSTEM "HP-UX"
#endif //defined(_hpux) || defined(hpux) || defined(__hpux)
#if defined(__OS400__)
#define C_SYSTEM_IBM_OS_400 1
#define C_SYSTEM "IBM OS 400"
#endif //defined(__OS400__)
#if defined(__INTEGRITY)
#define C_SYSTEM_INTEGRITY 1
#define C_SYSTEM "INTEGRITY"
#endif //defined(__INTEGRITY)
#if defined(__INTERIX)
#define C_SYSTEM_INTERIX 1
#define C_SYSTEM "Interix Environment"
#endif //defined(__INTERIX)
#if defined(sgi) || defined(__sgi)
#define C_SYSTEM_IRIX 1
#define C_SYSTEM "IRIX (sgi)"
#endif //defined(sgi) || defined(__sgi)
#if defined(__OS9000) || defined(_OSK)
#define C_SYSTEM_OS9000 1
#define C_SYSTEM "Microware OS9"
#endif //defined(__OS9000) || defined(_OSK)
#if defined(__MORPHOS__)
#define C_SYSTEM_MORPHOS 1
#define C_SYSTEM "Morph OS"
#endif //defined(__MORPHOS__)
#if defined(mpeix) || defined(__mpexl)
#define C_SYSTEM_MPEIX 1
#define C_SYSTEM "MPE/ix"
#endif //defined(mpeix) || defined(__mpexl)
#if defined(__TANDEM)
#define C_SYSTEM_TANDEM 1
#define C_SYSTEM "NonStop (TANDEM)"
#endif //defined(__TANDEM)
#if defined(__nucleus__)
#define C_SYSTEM_NUCLEUS 1
#define C_SYSTEM "Nucleus RTOS"
#endif //defined(__nucleus__)
#if defined(OS2) || defined(_OS2) || defined(__OS2__) || defined(__TOS_OS2__)
#define C_SYSTEM_OS2 1
#define C_SYSTEM "OS/2"
#endif //defined(OS2) || defined(_OS2) || defined(__OS2__) || defined(__TOS_OS2__)
#if defined(__palmos__)
#define C_SYSTEM_PALMOS 1
#define C_SYSTEM "PalmOS"
#endif //defined(__palmos__)
#if defined(EPLAN9)
#define C_SYSTEM_PLAN9 1
#define C_SYSTEM "Plan9 (EPLAN9)"
#endif //defined(EPLAN9)
#if defined(pyr)
#define C_SYSTEM_PYRAMID 1
#define C_SYSTEM "Pyramid DC/OSx"
#endif //defined(pyr)
#if defined(sinux)
#define C_SYSTEM_RELIANT 1
#define C_SYSTEM "Reliant Unix (sinux)"
#endif //defined(sinux)
#if defined(M_I386) || defined(M_XENIX) || defined(_SCO_DS)
#define C_SYSTEM_M_I386 1
#define C_SYSTEM "SCO OpenServer (M_I386)"
#endif //defined(M_I386) || defined(M_XENIX) || defined(_SCO_DS)
#if defined(__VOS__)
#define C_SYSTEM_STRATUS 1
#define C_SYSTEM "Stratus VOS"
#endif //defined(__VOS__)
#if defined(__sysv__) || defined(__SVR4) || defined(__svr4__) || defined(_SYSTYPE_SVR4)
#define C_SYSTEM_SVR4 1
#define C_SYSTEM "SVR4 Environment (sysv)"
#endif //defined(__sysv__) || defined(__SVR4) || defined(__svr4__) || defined(_SYSTYPE_SVR4)
#if defined(__SYLLABLE__)
#define C_SYSTEM_SYLLABLE 1
#define C_SYSTEM "Syllable"
#endif //defined(__SYLLABLE__)
#if defined(__SYMBIAN32__)
#define C_SYSTEM_SYMBIAN 1
#define C_SYSTEM "Symbian 32"
#endif //defined(__SYMBIAN32__)
#if defined(__osf__) || defined(__osf)
#define C_SYSTEM_TRU64 1
#define C_SYSTEM "Tru64 OSF/1"
#endif //defined(__osf__) || defined(__osf)
#if defined(ultrix) || defined(__ultrix) || defined(__ultrix__) || (defined(unix) && defined(vax))
#define C_SYSTEM_ULTRIX 1
#define C_SYSTEM "Ultrix"
#endif //defined(ultrix) || defined(__ultrix) || defined(__ultrix__) || (defined(unix) && defined(vax))
#if defined(_UNICOS)
#define C_SYSTEM_UNICOS 1
#define C_SYSTEM "UNICOS"
#endif //defined(_UNICOS)
#if defined(_CRAY) || defined(__crayx1)
#define C_SYSTEM_CRAY 1
#define C_SYSTEM "UNICOS/mp (CRAY)"
#endif //defined(_CRAY) || defined(__crayx1)
#if defined(sco) || defined(_UNIXWARE7)
#define C_SYSTEM_UNIXWARE 1
#define C_SYSTEM "UnixWare (SCO)"
#endif //defined(sco) || defined(_UNIXWARE7)
#if defined(_UWIN)
#define C_SYSTEM_UWIN 1
#define C_SYSTEM "U/WIN Environment"
#endif //defined(_UWIN)
#if defined(VMS) || defined(__VMS) ||defined(__VMS_VER)
#define C_SYSTEM_VMS 1
#define C_SYSTEM "VMS"
#endif //defined(VMS) || defined(__VMS) ||defined(__VMS_VER)
#if defined(__VXWORKS__) || defined(__vxworks) || defined(_WRS_VXWORKS_MAJOR) || defined(_WRS_VXWORKS_MINOR) || defined(_WRS_VXWORKS_MAINT) || defined(__RTP__) || defined(_WRS_KERNEL)
#define C_SYSTEM_VXWORKS 1
#define C_SYSTEM "VxWorks"
#endif //defined(__VXWORKS__) || defined(__vxworks) || defined(_WRS_VXWORKS_MAJOR) || defined(_WRS_VXWORKS_MINOR) || defined(_WRS_VXWORKS_MAINT) || defined(__RTP__) || defined(_WRS_KERNEL)
#if defined(_WINDU_SOURCE)
#define C_SYSTEM_WINDU 1
#define C_SYSTEM "Wind/U Environment (SOURCE)"
#endif //defined(_WINDU_SOURCE)
#if defined(__MVS__) || defined(__HOS_MVS__) || defined(__TOS_MVS__)
#define C_SYSTEM_ZOS 1
#define C_SYSTEM "z/OS (MVS)"
#endif //defined(__MVS__) || defined(__HOS_MVS__) || defined(__TOS_MVS__)
#endif //C_PREDEFINED_OBSCURE_SYSTEMS == 1 obscure systems

//STANDARDS
#if defined(__STDC__)
#define C_STANDARD_C90 1
#endif //defined(__STDC__)
#if __STDC_VERSION__ >= 199409L
#define C_STANDARD_C94 1
#endif //__STDC_VERSION__ == 199409L
#if __STDC_VERSION__ >= 199901L
#define C_STANDARD_C99 1
#endif //__STDC_VERSION__ == 199901L
#if __STDC_VERSION__ >= 201112L
#define C_STANDARD_C11 1
#endif //__STDC_VERSION__ == 201112L
#if __STDC_HOSTED__
#define C_STANDARD_HOSTED 1
#endif //__STDC_HOSTED__
#if __cplusplus >= 199711L
#define C_STANDARD_CPP98 1
#endif //__cplusplus == 199711L
#if __cplusplus >= 201103L
#define C_STANDARD_CPP11 1
#endif //__cplusplus == 201103L
#if __cplusplus >= 201402L
#define C_STANDARD_CPP14 1
#endif //__cplusplus == 201402L
#if __cplusplus_cli >= 200406L
#define C_STANDARD_CLI 1
#endif //__cplusplus_cli == 200406L
#if __embedded_cplusplus
#define C_STANDARD_EMBEDDED 1
#endif //__embedded_cplusplus
#if C_PREDEFINED_UNIX_STNDARDS == 1 //unistd.h: Unix Standards
#include <unistd.h>
#if _POSIX_VERSION >= 198808L
#define C_STANDARD_POSIX_1_1988 1
#endif //POSIX_VERSION == 198808L
#if _POSIX_VERSION >= 199009L
#define C_STANDARD_POSIX_1_1990 1
#endif //POSIX_VERSION == 199009L
#if _POSIX2_C_VERSION >= 199209L
#define C_STANDARD_POSIX_2 1
#endif //POSIX2_C_VERSION == 199209L
#if _POSIX_VERSION >= 199309L
#define C_STANDARD_POSIX_1B_1993 1
#endif //POSIX_VERSION == 199309L
#if _POSIX_VERSION >= 199506L
#define C_STANDARD_POSIX_1_1996 1
#endif //POSIX_VERSION == 199506L
#if _POSIX_VERSION >= 200112L
#define C_STANDARD_POSIX_1_2001 1
#endif //POSIX_VERSION == 200112L
#if _POSIX_VERSION >= 200809L
#define C_STANDARD_POSIX_1_2008 1
#endif //POSIX_VERSION == 200809L
#if _XOPEN_VERSION >= 3
#define C_STANDARD_XPG3 1 //1989
#endif //XOPEN_VERSION == 3
#if _XOPEN_VERSION >= 4
#define C_STANDARD_XPG4 1 //1992
#endif //XOPEN_VERSION == 4
#if _XOPEN_UNIX
#define C_STANDARD_SUS 1
#endif //XOPEN_UNIX
#if _XOPEN_VERSION >= 500
#define C_STANDARD_SUSV2 1
#endif //XOPEN_VERSION == 500
#if _XOPEN_VERSION >= 600
#define C_STANDARD_SUSV3 1
#endif //__XOPEN_VERSION == 600
#if _XOPEN_VERSION >= 700
#define C_STANDARD_SUSV4 1
#endif //__XOPEN_VERSION == 700
#endif //C_PREDEFINED_UNIX_STNDARDS == 1
#if C_PREDEFINED_HELPFUL_MACROS == 1
#if __ELF__
#define C_STANDARD_ELF 1
#endif //__ELF__
#if __VERSION__
#define C_COMPILER_VERSION 1
#endif //__VERSION__
#if __CHAR_UNSIGNED__
#define C_STANDARD_CHAR_UNSIGNED 1
#endif //__CHAR_UNSIGNED__
#if __WCHAR_UNSIGNED__
#define C_STANDARD_WCHAR_UNSIGNED 1
#endif //__WCHAR_UNSIGNED__
#if __CHAR_BIT__
#define C_STANDARD_BYTE_SIZE 1
#endif //__CHAR_BIT__
#if __ORDER_LITTLE_ENDIAN__
#define C_STANDARD_LITTLE_ENDIAN 1
#endif //__ORDER_LITTLE_ENDIAN__
#if __ORDER_BIG_ENDIAN__
#define C_STANDARD_BIG_ENDIAN 1
#endif //__ORDER_BIG_ENDIAN__
#if __ORDER_PDP_ENDIAN__
#define C_STANDARD_PDP_ENDIAN 1
#endif //__ORDER_PDP_ENDIAN__
#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define C_STANDARD_FLOAT_WORD_ORDER_LITTLE_ENDIAN 1
#elif __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__
#define C_STANDARD_FLOAT_WORD_ORDER_BIG_ENDIAN 1
#endif //__FLOAT_WORD_ORDER__
#if __OBJC__
#define C_STANDARD_OBJECTIVEC 1
#endif //__OBJC__
#if __ASSEEMBLER__
#define C_STANDARD_ASSEMBLY 1
#endif //__ASSEEMBLER__
#if __STRICT_ANSI__
#define C_STANDARD_STRICT_ANSI 1
#endif //__STRICT_ANSI__
#if __DEPRECATED
#define C_STANDARD_DEPRECATED 1
#endif //__DEPRECATED
#if __EXCEPTIONS
#define C_STANDARD_EXCEPTIONS 1
#endif //__EXCEPTIONS
#if __LP64__ && _LP64
#define C_STANDARD_64BIT_LONG_INT_AND_POINTER 1
#endif //__LP64__ && _LP64
#endif //C_PREDEFINED_HELPFUL_MACROS == 1
/*#if __OPTIMIZE__
#define __OPTIMIZE__ 1
#endif //__OPTIMIZE__
#if __OPTIMIZE_SIZE__
#define __OPTIMIZE_SIZE__ 1
#endif //__OPTIMIZE_SIZE__
#if __NO_INLINE__
#define __NO_INLINE__ 1
#endif //__NO_INLINE__*/
/*#if __REGISTER_PREFIX__
#define __REGISTER_PREFIX__ 1
#endif //__REGISTER_PREFIX__
#if __USER_LABEL_PREFIX__
#define __USER_LABEL_PREFIX__ 1
#endif //__USER_LABEL_PREFIX__*/
/*#if __BYTE_ORDER__
#define __BYTE_ORDER__ 1
#endif //__BYTE_ORDER__*/

//USEFUL
//__FILE__ Gives the name of the source/header file, at time of compilation, as a null-terminated byte-string; e.g. "predefined.h".
//__LINE__ Gives the line number; e.g. 809.
//__func__ Gives the name of the current function as a null-terminated byte-string; e.g. "main".
//__DATE__ Gives the build date as a null-terminated byte-string.
//__TIME__ Gives the build time as a null-terminated byte-string.
//__TIMESTAMP__ Gives the last-modification time of the current.
//__COUNTER__ Becomes a number which increments each time.
//__BASE_FILE__ Always returns the name of the file which contains the main function
//__INCLUDE_LEVEL__ "This macro expands to a decimal integer constant that represents the depth of nesting in include files. The value of this macro is incremented on every `#include' directive and decremented at the end of every included file. It starts out at 0, its value within the base file specified on the command line." -- https://gcc.gnu.org/onlinedocs/gcc-4.6.1/cpp/Common-Predefined-Macros.html#Common-Predefined-Macros

/*
A complete and fairly authoritative collection of predefined macros for architectures, compilers, systems, standards, and even libraries(!!); it was my primary resource for making this header: https://sourceforge.net/p/predef/wiki/Home/
A the list of shell commands for printing macro was copied from http://nadeausoftware.com/articles/2011/12/c_c_tip_how_list_compiler_predefined_macros#Commandlineoptions
The official GCC documentation also list some standard and commonly defined macros: https://gcc.gnu.org/onlinedocs/gcc-4.6.1/cpp/Macros.html#Macros
Git repository: https://github.com/Anadian/C_Predefined_H
*/

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //C_PREDEFINED_H