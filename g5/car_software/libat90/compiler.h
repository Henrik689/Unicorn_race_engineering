//******************************************************************************
//! @file $RCSfile: compiler.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file (re)defines ATMEL-App.Lab compiler keywords in order to
//! to ensure that any source file can be processed. The device targeted belongs
//! to AT90CANxx family.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _COMPILER_H_
#define _COMPILER_H_

// Max(a, b): Take the max between a and b
// Min(a, b): Take the min between a and b
#define Max(a, b)          ( (a)>(b) ? (a) : (b) )
#define Min(a, b)          ( (a)<(b) ? (a) : (b) )

#endif // _COMPILER_H_
