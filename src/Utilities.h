#pragma once
#include <stdio.h>
#include <stdlib.h>		// exit function

// Mark variable as used
#define	UNUSED(a)					(void)(a)
// Helper macro to align a value
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

//The PS3 compiler defines either of these
#if defined(__psp2__)
	#define _vita_
#elif defined(__PPU__) || defined(__SPU__)
	#define _PS3_
//This probably doesn’t exist, but it’s here for completness
#elif defined( XBOX360 )
	#define XBOX360
//Windows has a lot of variants.
//In production you would want to split this up,
// as win64 might have issues with win32 stuff
#elif defined(WIN32) || defined(_WIN32_)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64_) || defined(__CYGWIN__) || defined(MINGW32)
	#define _WINDOWS_
//This works, but it could be either an OSX or iOS device
#elif defined( APPLE )
	#define _MAC_
//This works, but isn’t a complete solution, google unix
#elif defined( linux )
	#define _LINUX_
#else
	#error unknown platform
#endif

#define DEBUG 1


// −− Platform specific halts
#if (DEBUG == 1)
	#if defined(_vita_)
		#include "libdbg.h"
		#define DBG_HALT _SCE_BREAK();
	#elif defined(_PS3_)
		//#define DBG_HALT { __asm__ volatile( "trap" ); }
		#define DBG_HALT { exit (1); }
	#elif defined(_XBOX360_) || defined(_WINDOWS_)
		//#define DBG_HALT  __debugbreak
		#define DBG_HALT __asm {int 3}
	#elif defined(_APPLE_)
		#define HALT builtin trap();
	#elif defined(_linux_)
		#define HALT raise(SIGINT);
	#else
		#error HALT: unknown platform
	#endif
#endif

// call DBG_HALT on assert fail
#define DBG_ASSERT(exp) { if ( !(exp) ) {DBG_HALT;} }
// Prints the suplied string on assert fail, then call DBG_HALT
#define DBG_ASSERT_MSG( exp, smsg ) { if ( !(exp) ) {puts (smsg); DBG_HALT;} }
// Calls the suplied function on assert fail, then call DBG_HALT
#define DBG_ASSERT_FUNC( exp, func) { if ( !(exp) ) {func; DBG_HALT;} }

// −− Platform specific Utility Libs
#if defined(_vita_)
	#define FILE_PATH "app0:"
#elif defined(_PS3_)
	#define FILE_PATH SYS_APP_HOME + std::string("/")
#elif defined(_XBOX360_) || defined(_WINDOWS_)
	//#define uint32_t unsigned int
	//#define uint16_t unsigned int
	#define FILE_PATH ""
#endif
