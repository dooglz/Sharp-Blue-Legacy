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
	#define _PC_
//This works, but it could be either an OSX or iOS device
#elif defined( APPLE )
	#define _MAC_
	#define _PC_
//This works, but isn’t a complete solution, google unix
#elif defined( linux )
	#define _LINUX_
	#define _PC_
#else
	#error unknown platform
#endif

#define DEBUG 1

#if defined(_PC_)
	#include "sdl/sdl_assert.h"
#endif

// −− Platform specific halts
#if (DEBUG == 1)
	#if defined(_vita_)
		#include "libdbg.h"
		#define HALT _SCE_BREAK();
	#elif defined(_PS3_)
		//#define HALT { __asm__ volatile( "trap" ); }
		#define HALT { exit (1); }
	#elif defined(_XBOX360_)
		//#define HALT  __debugbreak
		#define HALT __asm {int 3}
	#elif defined(_PC_)
		#define HALT SDL_TriggerBreakpoint();
	#else
		#error HALT: unknown platform
	#endif
#endif

#if defined(_PC_)
	//Use the Radcool SDL assert
	#define ASSERT(exp) SDL_assert(exp)
	#define ASSERT_MSG( exp, smsg ) SDL_assert(exp)
	#define ASSERT_FUNC( exp, func) SDL_assert(exp)
#else
	// call HALT on assert fail
	#define ASSERT(exp) { if ( !(exp) ) {HALT;} }
	// Prints the suplied string on assert fail, then call HALT
	#define ASSERT_MSG( exp, smsg ) { if ( !(exp) ) {puts (smsg); HALT;} }
	// Calls the suplied function on assert fail, then call HALT
	#define ASSERT_FUNC( exp, func) { if ( !(exp) ) {func; HALT;} }

#endif

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
