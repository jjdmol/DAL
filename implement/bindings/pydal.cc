/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2007 by Joseph Masters                                  *
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

#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle

#include "dal.h"
#include "bindings/pydal.h"

/*!
  \file pydal.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the C++ layer of the Data Acccess Library

  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>

*/

BOOST_PYTHON_MODULE(pydal)
{
  bpl::scope().attr("__doc__") =
    "Routines for python bindings to the data access library (DAL)."
    ;
  
  import_array();
  bpl::numeric::array::set_module_and_type("numpy", "ndarray");
  
  def( "mjd2unix", &mjd2unix_boost,
       "Convert Modified Julian Date (mjd) to unix time.\n"
       "The Unix base date is MJD 40587 and 1 mjd Day = 24 hours \n"
       "or 1440 minutes or 86400 seconds so: \n"
       "(unix seconds) = (mjd seconds) - ( unix base date in seconds )." );
  
  // ============================================================================
  //
  //  [core] Core classes
  //
  // ============================================================================

  export_dalArray ();
  export_dalColumn ();
  export_dalData ();
  
  //_____________________________________________________________________________
  //                                                                   dalDataset
  
  bpl::class_<dalDataset>("dalDataset")
    .def( bpl::init<char*, string>() )
    .def( "setAttribute_char", &dalDataset::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_char", &dalDataset::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalDataset::setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_short", &dalDataset::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalDataset::setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_int", &dalDataset::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalDataset::setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_uint", &dalDataset::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalDataset::setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_long", &dalDataset::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalDataset::setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_float", &dalDataset::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalDataset::setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_double", &dalDataset::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalDataset::setAttribute_string_boost,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalDataset::setAttribute_string_vector,
	  "Set a string attribute" )
	.def( "getAttribute_float", &dalDataset::getAttribute_float_boost,
	  "Return a dalDataset float attribute into a numpy array." )
	.def( "getAttribute_double", &dalDataset::getAttribute_double_boost,
	  "Return a dalDataset double attribute into a numpy array." )
	.def( "getAttribute_long", &dalDataset::getAttribute_long_boost,
	  "Return a dalDataset long attribute into a numpy array." )
	.def( "getAttribute_short", &dalDataset::getAttribute_short_boost,
	  "Return a dalDataset short attribute into a numpy array." )
	.def( "getAttribute_int", &dalDataset::getAttribute_int_boost,
	  "Return a dalDataset int attribute into a numpy array." )
	.def( "getAttribute_uint", &dalDataset::getAttribute_uint_boost,
	  "Return a dalDataset uint attribute into a numpy array." )
	.def( "getAttribute_string", &dalDataset::getAttribute_string_boost,
	  "Return a dalDataset string attribute into a numpy array." )
    .def( "open", &dalDataset::open,
	  ( bpl::arg("dataset_name") ),
	  "Opens a dataset." )
    .def( "close", &dalDataset::close,
	  "Closes a dataset." )
    .def( "getType", &dalDataset::getType,
	  "Get the file type of dataset." )
    .def( "createTable", &dalDataset::ct1_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  ( bpl::arg("table_name") ),
	  "Create a new table in the dataset." )
    .def( "createTable", &dalDataset::ct2_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  ( bpl::arg("table_name"), bpl::arg("group_name") ),
	  "Create a new table in the dataset." )
    .def( "createGroup", &dalDataset::createGroup,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  ( bpl::arg("group_name") ),
	  "Create a new group in the dataset." )
    .def( "openTable", &dalDataset::ot1_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Open a table in the dataset.")
    .def( "openTable", &dalDataset::ot2_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Open a table in the dataset.")
    .def( "openGroup", &dalDataset::openGroup,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Open a group in the dataset.")
    .def( "openArray", &dalDataset::open_array_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Open an array in the dataset.")
    .def( "createIntArray", &dalDataset::cia_boost1,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost2,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost_numarray1,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost_numarray2,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createFloatArray", &dalDataset::cfa_boost,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an floating-point array in the dataset." )
    .def( "createFloatArray", &dalDataset::cfa_boost_numarray,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an floating-point array in the dataset." )
    .def( "readIntArray", &dalDataset::ria_boost,
	  "Read an integer array from the dataset." )
    .def( "readFloatArray", &dalDataset::rfa_boost,
	  "Read a floating-point array from the dataset." )
    .def( "createArray", &dalDataset::createArray,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an array from a dalData object" )
    .def( "setFilter", &dalDataset::setFilter_boost1,
	  ( bpl::arg("columns") ),
	  "Set a filter on the dataset (casa only)." )
    .def( "setFilter", &dalDataset::setFilter_boost2,
	  ( bpl::arg("columns"), bpl::arg("conditons") ),
	  "Set a filter on the dataset (casa only)." )
    .def( "listTables", &dalDataset::listTables_boost,
	  "Return a list of the tables in the dataset." )
    ;
  
  //-------------------------------------------------------------------- dalGroup
  
  bpl::class_<dalGroup>("dalGroup")
    .def( bpl::init<>())
    .def( bpl::init<char*, void*>())
    .def( bpl::init<hid_t,char*>())
    .def( "setAttribute_char", &dalGroup::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_char", &dalGroup::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalGroup::setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_short", &dalGroup::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalGroup::setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_int", &dalGroup::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalGroup::setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_uint", &dalGroup::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalGroup::setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_long", &dalGroup::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalGroup::setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_float", &dalGroup::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalGroup::setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_double", &dalGroup::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalGroup::setAttribute_string,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalGroup::setAttribute_string_vector,
	  "Set a string attribute" )
	.def( "getAttribute_float", &dalGroup::getAttribute_float_boost,
	  "Return a dalGroup float attribute into a numpy array." )
	.def( "getAttribute_double", &dalGroup::getAttribute_double_boost,
	  "Return a dalGroup double attribute into a numpy array." )
	.def( "getAttribute_long", &dalGroup::getAttribute_long_boost,
	  "Return a dalGroup long attribute into a numpy array." )
	.def( "getAttribute_short", &dalGroup::getAttribute_short_boost,
	  "Return a dalGroup short attribute into a numpy array." )
	.def( "getAttribute_int", &dalGroup::getAttribute_int_boost,
	  "Return a dalGroup int attribute into a numpy array." )
	.def( "getAttribute_uint", &dalGroup::getAttribute_uint_boost,
	  "Return a dalGroup uint attribute into a numpy array." )
	.def( "getAttribute_string", &dalGroup::getAttribute_string_boost,
	  "Return a dalGroup string attribute into a numpy array." )
    .def( "getName", &dalGroup::getName,
	  "Return the group name." )
    .def( "setName", &dalGroup::setName,
	  "Set the name of the group." )
    .def( "getId", &dalGroup::getId,
	  "Return the group identifier." )
    .def( "createShortArray", &dalGroup::csa_boost_list,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create a short array in the group." )
    .def( "createIntArray", &dalGroup::cia_boost_list,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create an integer array in the group." )
    .def( "createFloatArray", &dalGroup::cfa_boost_list,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Create a floating point array in the group." )
    .def( "readIntArray", &dalGroup::ria_boost,
	  "Read an integer array from the group." )
    ;
  
  //_____________________________________________________________________________
  //                                                                     dalTable
  
  bpl::class_<dalTable>("dalTable")
    .def( bpl::init<char*>())
    .def( "setAttribute_char", &dalTable::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_char", &dalTable::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalTable::setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_short", &dalTable::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalTable::setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_int", &dalTable::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalTable::setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_uint", &dalTable::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalTable::setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_long", &dalTable::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalTable::setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_float", &dalTable::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalTable::setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_double", &dalTable::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalTable::setAttribute_string,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalTable::setAttribute_string_vector,
	  "Set a string attribute" )
    .def( "openTable", &dalTable::ot_hdf5,
	  "Open an hdf5 table object." )
    .def( "createTable", &dalTable::createTable,
	  "Create a table object." )
    .def( "addColumn", &dalTable::addColumn,
	  "Add a column to the table." )
    .def( "addComplexColumn", &dalTable::addComplexColumn,
	  "Add a complex column to the table." )
    .def( "removeColumn", &dalTable::removeColumn,
	  "Remove a column from the table." )
    .def( "writeDataByColNum", &dalTable::writeDataByColNum,
	  "Write column data using an index." )
    .def( "appendRow", &dalTable::append_row_boost,
	  "Append a row to the table." )
    .def( "appendRows", &dalTable::append_rows_boost,
	  "Append multiple rows to the table." )
    .def("write_col_by_index_boost", &dalTable::write_col_by_index_boost,
	 "Write data to a column with a numpy array as input")
    .def( "printColumns", &dalTable::printColumns,
	  "Print the column tables to the screen." )
    .def( "listColumns", &dalTable::listColumns_boost,
	  "Return a list of the table columns." )
    .def( "readRows", &dalTable::readRows_boost,
    	  "Read table rows." )
    .def( "getAttribute", &dalTable::getAttribute_boost,
	  "Return the value of a column attribute." )
    .def( "findAttribute", &dalTable::findAttribute,
	  "Return true if the attribute exists for the table." )
    .def( "getNumberOfRows", &dalTable::getNumberOfRows,
	  "Return the number of rows in the table." )
#ifdef HAVE_CASA
    .def( "openTable", &dalTable::ot_nonMStable,
	  "Open a casa table." )
    .def( "getName", &dalTable::getName,
	  "Get the name of a casa table." )
    .def( "setFilter", &dalTable::setFilter_boost1,
	  "Set a filter on a casa table." )
    .def( "setFilter", &dalTable::setFilter_boost2,
	  "Set a filter on a casa table." )
    .def( "getColumn", &dalTable::getColumn,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Return a casa column object." )
    .def( "getColumn_Float32", &dalTable::getColumn_Float32,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Return a 32-bit floating point casa column." )
    .def( "getColumn_complexFloat32", &dalTable::getColumn_complexFloat32,
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Return a complex 32-bit floating point casa column." )
#endif
    ;

  // ============================================================================
  //
  //  [data_common] Common dataset support
  //
  // ============================================================================
  
  export_Filename();
  export_CommonAttributes();
  export_Timestamp();
  export_SAS_Settings();
  export_HDF5Hyperslab();
  
  // ============================================================================
  //
  //  [data_hl] High-level interfaces to specific data
  //
  // ============================================================================
  
  export_BeamFormed();
  export_BeamGroup();
  export_TBB_Timeseries ();
  export_TBB_StationGroup ();
  export_TBB_DipoleDataset ();  
  
}