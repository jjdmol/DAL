/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha ()                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "dalFITS.h"

namespace DAL
{
  // ============================================================================
  //
  //  Construction / Destruction
  //
  // ============================================================================
  
  dalFITS::dalFITS () {}
  
  dalFITS::~dalFITS () {}

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void dalFITS::summary (std::ostream &os)
  {
    os << "[dalFITS] Summary of object properties"       << std::endl;
  }
  
}
