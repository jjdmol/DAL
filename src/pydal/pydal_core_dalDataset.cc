/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

/*!
  \file pydal_core_dalDataset.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalDataset class

  \author Lars B&auml;hren
*/

#include "pydal.h"

using DAL::dalDataset;
using DAL::dalTable;
using DAL::dalArray;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                      ct1_boost

/*!
  \param a -- The name of the table to be created
*/
dalTable * dalDataset::ct1_boost (std::string a)
{
  dalTable * ret;
  ret = dalDataset::createTable(a);
  return ret;
}

//_____________________________________________________________________________
//                                                                    ct2_boost

/*!
  \param a -- The name of the group within which the new table is created.
  \param b -- The name of the table to be created
*/
dalTable * dalDataset::ct2_boost (std::string a,
				  std::string b)
{
  dalTable * ret;
  ret = dalDataset::createTable(a,b);
  return ret;
}

//_____________________________________________________________________________
//                                                                    ot1_boost

/*!
  \param a -- The name of the table to be opened
*/
dalTable * dalDataset::ot1_boost (std::string a)
{
  dalTable * ret;
  ret = openTable(a);
  return ret;
}

//_______________________________________________________________________________
//                                                                      ot2_boost

/*
  \param a -- The name of the group within which the table resides.
  \param b -- The name of the table to be opened.
*/
dalTable * dalDataset::ot2_boost (std::string a,
				  std::string b)
{
  dalTable * ret;
  ret = openTable(a,b);
  return ret;
}

//_______________________________________________________________________________
//                                                                     cia_boost1

dalArray * dalDataset::cia_boost1 (std::string arrayname,
				   bpl::list pydims,
				   bpl::list pydata )
{
  bpl::list cdims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    cdims.append(10);
  }
  
  dalArray * array;
  array = cia_boost2( arrayname, pydims, pydata, cdims );
  
  /* Return pointer to the created array */
  return array;
}

//_______________________________________________________________________________
//                                                                     cia_boost2

dalArray * dalDataset::cia_boost2 (std::string arrayname,
				   bpl::list pydims,
				   bpl::list pydata,
				   bpl::list cdims )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    dims.push_back(bpl::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<bpl::len(cdims); ii++) {
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));
  }
  
  long size = bpl::len(pydata);
  int * data = NULL;
  data = new int[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = bpl::extract<int>(pydata[ii]);
  }
  
  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

// ----------------------------------------------------- cia_boost_numarray1

dalArray * dalDataset::cia_boost_numarray1 (std::string arrayname,
					    bpl::list pydims,
					    bpl::numeric::array pydata )
{
  bpl::list cdims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    cdims.append(10);
  }
  
  dalArray * array;
  array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);
  
  /* Return pointer to created array */
  return array;
}

// ----------------------------------------------- cia_boost_numarray2

dalArray * dalDataset::cia_boost_numarray2 (std::string arrayname,
					    bpl::list pydims,
					    bpl::numeric::array pydata,
					    bpl::list cdims )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    dims.push_back(bpl::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<bpl::len(cdims); ii++) {
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));
  }
  
  int size = num_util::size(pydata);
  int * data = NULL;
  data = new int[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = bpl::extract<int>(pydata[ii]);
  }
  
  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

// ---------------------------------------------------------- cfa_boost

dalArray * dalDataset::cfa_boost (std::string arrayname,
				  bpl::list pydims,
				  bpl::list pydata,
				  bpl::list cdims)
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    dims.push_back(bpl::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<bpl::len(cdims); ii++) {
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));
  }
  
  long size = bpl::len(pydata);
  float * data = NULL;
  data = new float[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = bpl::extract<float>(pydata[ii]);
  }
  
  dalArray * array = createFloatArray( arrayname, dims, data, chnkdims );
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

// ------------------------------------------------------ cfa_boost_numarray

dalArray * dalDataset::cfa_boost_numarray (std::string arrayname,
					   bpl::list pydims,
					     bpl::numeric::array pydata,
					     bpl::list cdims )
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    int size = num_util::size(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  // ---------------------------------------------------------- ria_boost

  bpl::numeric::array dalDataset::ria_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t  status;

    // get the dataspace
    lclfile = H5Dopen (h5fh_p, arrayname.c_str(), H5P_DEFAULT);
    hid_t filespace = H5Dget_space(lclfile);

    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];
    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;
    for (int ii=0; ii<data_rank; ii++) {
      size *= dims[ii];
      dimensions.push_back(dims[ii]);
    }
    
    int * data = NULL;
    data = new int[size];
    /* Read data from HDF5 file */
    status = H5LTread_dataset_int( h5fh_p, arrayname.c_str(), data );
    /* Convert data array */
    bpl::numeric::array nadata = num_util::makeNum( (int*)data, dimensions );
    
    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }
  
  // ---------------------------------------------------------- rfa_boost
  
  bpl::numeric::array dalDataset::rfa_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t status;
    
    // get the dataspace
    lclfile = H5Dopen (h5fh_p, arrayname.c_str(), H5P_DEFAULT);
    hid_t filespace = H5Dget_space(lclfile);
    
    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];

    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;

    for (int ii=0; ii<data_rank; ii++)
      {
        size *= dims[ii];
        dimensions.push_back(dims[ii]);
      }

    float * data = NULL;
    data = new float[size];

    status = H5LTread_dataset_float( h5fh_p, arrayname.c_str(), data );
    bpl::numeric::array nadata = num_util::makeNum( (float*)data, dimensions );

    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }

  /******************************************************
   * wrappers for setFilter
   ******************************************************/

  // -------------------------------------------------------- setFilter_boost1

  void dalDataset::setFilter_boost1 (std::string columns )
  {
    setFilter( columns );
  }

  // -------------------------------------------------------- setFilter_boost2

  void dalDataset::setFilter_boost2 (std::string columns,
				     std::string conditions)
  {
    setFilter( columns, conditions );
  }

  // -------------------------------------------------------- listTables_boost

  bpl::list dalDataset::listTables_boost()
  {
    std::vector<std::string> lcltabs = listTables();
    bpl::list lcllist;
    for (uint idx=0; idx<lcltabs.size(); idx++) {
      lcllist.append( lcltabs[idx] );
    }
    return lcllist;
  }

  
  dalArray * dalDataset::open_array_boost( std::string arrayname )
  {
    dalArray * ret;
    ret = openArray( arrayname );
    return ret;
  }

  /*  setAttribute calls for all types, for single and vector values */
  bool dalDataset::setAttribute_char (std::string attrname, char data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_short (std::string attrname, short data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_int (std::string attrname, int data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_uint (std::string attrname, uint data )
  {
    return setAttribute (attrname, &data );
  }
#ifndef WORDSIZE_IS_64
  bool dalDataset::setAttribute_long (std::string attrname, int64_t data )
  {
    return setAttribute (attrname, &data );
  }
#else
  bool dalDataset::setAttribute_long (std::string attrname, long data )
  {
    return setAttribute (attrname, &data );
  }
#endif
  bool dalDataset::setAttribute_float (std::string attrname, float data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_double (std::string attrname, double data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_string_boost(std::string attrname, std::string data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_char_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<char> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<char>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_short_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<short> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<short>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_int_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<int> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<int>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_uint_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<uint> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<uint>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_long_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<long> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<long>(data[ii]));

#ifndef WORDSIZE_IS_64
    return setAttribute (attrname, reinterpret_cast<int64_t*>(&mydata[0]), size );
#else
    return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
#endif
  }
  bool dalDataset::setAttribute_float_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<float> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<float>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_double_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<double> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<double>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_string_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<std::string> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<std::string>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
  }

  /*  getAttribute calls for all types, for single values */
  bpl::numeric::array dalDataset::getAttribute_float_boost ( std::string attrname )
  {
  	 std::vector<float> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
//  	 float * values = NULL;


//     std::vector<int> mydims;
//     mydims.push_back( value.size() );
//
//     for (int idx=0; idx++; idx<value.size() )
//        values
//     
//      bpl::numeric::array narray = num_util::makeNum( values, mydims );
//      delete [] values;
//      values = NULL;
//      return narray;


     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<float*>(&value[0]), dims );
     return arr;
  }
  bpl::numeric::array dalDataset::getAttribute_double_boost ( std::string attrname )
  {
  	 std::vector<double> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<double*>(&value[0]), dims );
     return arr;
  }
  bpl::numeric::array dalDataset::getAttribute_long_boost ( std::string attrname )
  {
  	 std::vector<long> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<long*>(&value[0]), dims );
     return arr;
  }
  bpl::numeric::array dalDataset::getAttribute_short_boost ( std::string attrname )
  {
  	 std::vector<short> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<short*>(&value[0]), dims );
     return arr;
  }
  bpl::numeric::array dalDataset::getAttribute_int_boost ( std::string attrname )
  {
  	 std::vector<int> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<int*>(&value[0]), dims );
     return arr;
  }
  bpl::numeric::array dalDataset::getAttribute_uint_boost ( std::string attrname )
  {
  	 std::vector<uint> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<uint*>(&value[0]), dims );
     return arr;
  }
  bpl::list dalDataset::getAttribute_string_boost ( std::string attrname )
  {
     bpl::list data;
  	 std::vector<string> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	        
     for ( uint ii=0; ii < value.size() ; ii++ )
        data.append( value[ii].c_str() );

     return data;
  }
  
// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalDataset ()
{
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
}

