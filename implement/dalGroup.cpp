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

#ifndef dalGROUP_H
#include "dalGroup.h"
#endif

dalGroup::dalGroup() {
    H5::Group group;
//    hid_t foo = group.getLocId();
//    cout << "group id == " << foo << endl;
}

dalGroup::dalGroup( void * voidfile, string gname ) {

	H5::H5File * lclfile = (H5::H5File*)voidfile; // H5File object
	file = lclfile;
	file_id = lclfile->getLocId();  // get the file handle

	name = gname;
	H5::Group * lclgroup = new H5::Group( lclfile->createGroup( "/" + gname ));
	group = lclgroup;  // set local to global
}

bool dalGroup::setName ( string gname ) {
   if ( gname.length() > 0 )
   {
     name = gname;
     return SUCCESS;
   }
   else
   {
     cout << "Error:  Group name must not be empty." << endl;
     return FAIL;
   }
}

string dalGroup::getName () {
   return name;
}

void dalGroup::open( void * voidfile, string groupname ) {
	name = groupname;

	H5::H5File * lclfile = (H5::H5File*)voidfile; // H5File object
	file = lclfile;
	file_id = lclfile->getLocId();  // get the file handle
	
	string fullgroupname = "/" + groupname;
	group_id = H5Gopen( file_id, fullgroupname.c_str() );
}

void dalGroup::setAttribute_string( string attrname, string data ) {
	status = H5LTset_attribute_string( file_id, name.c_str(), attrname.c_str(), data.c_str() );
}

void dalGroup::setAttribute_int( string attrname, int * data, int size ) {
	status = H5LTset_attribute_int( file_id, name.c_str(),
			attrname.c_str(), data, size );
}

void dalGroup::setAttribute_uint( string attrname, unsigned int * data, int size ) {
	status = H5LTset_attribute_uint( file_id, name.c_str(), attrname.c_str(), data, size );
}

void dalGroup::setAttribute_double( string attrname, double * data, int size ) {
	status = H5LTset_attribute_double( file_id, name.c_str(), attrname.c_str(), data, size );
}

void dalGroup::getAttribute( string attrname ) {

	hsize_t * dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute(group_id, attrname.c_str()) <= 0 ) {
		cout << "Attribute " << attrname << " not found." << endl;
		return;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(file_id, fullname.c_str(), attrname.c_str(), &rank );

	dims = (hsize_t *)malloc(rank * sizeof(hsize_t));

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		double data[*dims];
		H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data);
		cout << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cout << data[ii];
		  if (ii < (*dims)-1)
		    cout << ',';
		  else
		    cout << endl;
		}
	}
	else if ( H5T_INTEGER == type_class ) {
		int data[*dims];
		H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_INT, data);
		cout << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cout << data[ii];
		  if (ii < (*dims)-1)
		    cout << ',';
		  else
		    cout << endl;
		}
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		H5LTget_attribute_string( file_id, fullname.c_str(), attrname.c_str(),data);
		cout << attrname << " = " << data << endl;
	}
	else {
		cout << "Attribute " << attrname << " type unknown." << endl;
	}
}

/*
void dalGroup::setAttribute( string attrname, void * data, string coltype )
{
	const int size = 1;
	if ( dal_INT == coltype ) {
			status = H5LTset_attribute_int( file_id, name.c_str(), attrname.c_str(), (const int*)data, size );
	}
	else if ( dal_FLOAT == coltype ) {
			status = H5LTset_attribute_float( file_id, name.c_str(), attrname.c_str(), (const float*)data, size );
	}
	else if ( dal_DOUBLE == coltype ) {
			status = H5LTset_attribute_double( file_id, name.c_str(), attrname.c_str(), (const double*)data, size );
	}
	else {
			cout << "ERROR: datatype " << coltype << " not supported for setAttribute." << endl;
	}			
}
*/