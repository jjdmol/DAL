/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#include <core/dalFilter.h>

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  dalFilter::dalFilter()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type -- The type of file (i.e. HDF5, MSCASA).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
   */
  dalFilter::dalFilter (DAL::dalFileType const &type,
			std::string columns)
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */
    
    set (columns);
  }

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").

    Restrict the opening of a table to particular columns.
   */
  dalFilter::dalFilter (std::string type,
			std::string columns)
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */
    
    set (columns);
  }
  
  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type -- The type of file (i.e. HDF5, MSCASA).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
           (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter (DAL::dalFileType const &type,
                        std::string cols,
                        std::string conditions )
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */

    set (cols,conditions);
  }

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
           (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter( std::string type,
                        std::string cols,
                        std::string conditions )
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */

    set (cols,conditions);
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void dalFilter::init ()
  {
    itsFilterString = "";
    itsFiletype     = dalFileType();
    filterIsSet_p         = false;
  }

  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns A comma-separated list of the column names that you
           want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \return status -- Status of the operation; returns \e false in case an error
            was encountered, e.g. because the operation is not supported for the
	    file type.
  */
  bool dalFilter::set (std::string const &cols)
  {
    bool status         = true;
    std::string message = "[dalFilter::set]";
    
    switch (itsFiletype.type()) {
    case dalFileType::MSCASA:
      itsFilterString = "Select " + cols + " from $1";
      filterIsSet_p   = true;
      break;
    default:
      {
	status = false;
	/* Assemble error mesage */
	message += "Operation not yet supported for type: ";
	message += itsFiletype.name();
	message += ". Sorry.";
	/* Write error message */
	std::cerr << message << std::endl;
      }
      break;
    };
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns -- Names of the columns to select.
  */
  bool dalFilter::set (std::vector<std::string> const &columns)
  {
    if (columns.empty()) {
      std::cerr << "[dalFilter::set] Empty list of column names!" << std::endl;
      return false;
    } else {
      std::string names (columns[0]);
      /* Write column names into comma-separated list */
      for (unsigned int n(1); n<columns.size(); ++n) {
	names += ",";
	names += columns[n];
      }
      /* Set the column selection */
      return set (names);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
                      (i.e. "ANTENNA1=1 AND ANTENNA2=10")
   */
  void dalFilter::set (std::string const &cols,
		       std::string const &conditions)
  {
    switch (itsFiletype.type()) {
    case dalFileType::MSCASA:
      break;
        itsFilterString = "Select " + cols + " from $1 where " + conditions;
        filterIsSet_p         = true;
    default:
      {
      std::cerr << "Operation not yet supported for type: " 
		<< itsFiletype.name()
		<< ". Sorry.\n";
      }
      break;
    };
  }

  //_____________________________________________________________________________
  //                                                                  setFiletype
  
  /*!
    \param type The type of the file (i.e. "MSCAS", "HDF5", etc.)
   */
  bool dalFilter::setFiletype (std::string const &type)
  {
    itsFiletype = dalFileType (type);
    return true;
  }

  //_____________________________________________________________________________
  //                                                                  setFiletype
  
  bool dalFilter::setFiletype (DAL::dalFileType const &type)
  {
    itsFiletype = type;
    return true;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void dalFilter::summary (std::ostream &os)
  {
    os << "[dalFilter] Summary of internal parameters."   << std::endl;
    os << "-- Filter string = " << itsFilterString         << std::endl;
    os << "-- File type     = " << itsFiletype.name()      << std::endl;
    os << "-- Filter is set = " << filterIsSet_p          << std::endl;
  }
  
} // DAL namespace
