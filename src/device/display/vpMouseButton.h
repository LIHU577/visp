/****************************************************************************
 *
 * $Id$
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Color definition.
 *
 * Author:
 * Fabien Spindler
 *
 *****************************************************************************/


#ifndef vpMouseButton_h
#define vpMouseButton_h

#include <visp/vpConfig.h>

/*!
  \class vpMouseButton
  \ingroup ImageGUI
  \brief Class that defines mouse button identiers.
*/
class VISP_EXPORT vpMouseButton
{
public:
  typedef enum {
    button1 = 1, /*!< Mouse left button. */
    button2 = 2, /*!< Mouse middle button, or roll. */
    button3 = 3,  /*!< Mouse right button. */
    none = 0  /*!< No button. */
  } vpMouseButtonType ;
} ;

#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
