/**********************************************************

RobotC Naturalizarition
naturalization.h

Forces all RobotC environments to use the same settings.
Include this file at the beginning of every program.

Define _DISABLE_JOYDISPLAY to disable game controller display
Define _ENABLE_LCDDISPLAY to enable live NXT LCD display
Define _FORCE_DEBUG to disable full code optimization

**********************************************************/

#ifndef __NATURALIZATION_
#define __NATURALIZATION_

#ifndef __FirmwareVersion_
    #include "firmwareVersion.h"
#endif
#ifdef __FirmwareVersion_
    #if (kRobotCVersionNumeric < 359)
        #error "These drivers are only supported on RobotC version 3.59 or higher"
    #endif
#endif

#ifndef _FORCE_DEBUG
    #ifdef _DEBUG
        #undef _DEBUG //force full code optimization
    #endif
#else
    #ifndef _DEBUG
        #define _DEBUG //force no code optimization
    #endif
#endif

#if (defined(NXT) || defined(TETRIX)) && defined(_Target_Robot_) && !defined(NaturalLanguage)
    //A OK
#elif (defined(VEX2) || defined(NXT) || defined(TETRIX)) && defined(_Target_VirtWorld_)
    #error "Not supported with Virtual Worlds!"
#elif defined(NaturalLanguage)
    #warning "You are using Natural Language.  Some errors may occur as a result."
#elif (defined(NXT) || defined(TETRIX)) && defined(_Target_Emulator_)
    #warning "You are using an Emulator.  Some errors may occur as a result."
#else
    #error "Not supported on this platform!"
#endif

#ifndef _DISABLE_JOYDISPLAY
    #pragma DebuggerWindows("Globals")
    #pragma DebuggerWindows("Locals")
    #ifdef FTC
        #pragma DebuggerWindows("joystickGame")
    #else
        #pragma DebuggerWindows("joystickSimple")
    #endif
#endif

#ifdef _ENABLE_LCDDISPLAY
    #pragma DebuggerWindows("nxtLCDscreen")
#endif

#endif /* __NATURALIZATION_ /*
