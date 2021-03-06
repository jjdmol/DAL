/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

/* Standard header files */
#include <iostream>
#include <string>
#include <sstream>

/* casacore header files */
#include <casa/Arrays.h>
#include <coordinates/Coordinates/CoordinateSystem.h>
#include <coordinates/Coordinates/Projection.h>
#include <coordinates/Coordinates/DirectionCoordinate.h>
#include <images/Images/HDF5Image.h>
#include <images/Images/PagedImage.h>
#include <images/Images/ImageFITSConverter.h>

using std::cout;
using std::cerr;
using std::endl;

/*!
  \file tcasacore_images.cc

  \ingroup DAL

  \brief A collection of tests to work with casacore images

  \author Lars B&auml;hren

  \date 2009/09/04
  
  <h3>Prerequisite</h3>

  - casacore classes for encapsulating coordinate information
  - casacore image classes (ImageInterface, PagedImage, HDF5Image)
  
  <h3>Synopsis</h3>

  Required tests:
  - Combinations of celestial projection and reference system
  - Type of image axes (Direction, Linear, Tabular, Spectral, Stokes)
  - Size of image during conversion to FITS
*/

//_______________________________________________________________________________
//                                                               projections_list

/*!
  \brief Get list of known celestial projections
  
  Get list of known celestial map projections, as defined in
  <tt>coordinates/Coordinates/Projection.h</tt>
  
  \return names -- List of names of known celestial map projections.
*/
std::vector<std::string> projections_list () 
{
  std::vector<std::string> names;
  
  names.push_back("AZP");    //  Zenithal/Azimuthal perspective
  names.push_back("SZP");    //  Slant zenithal perspective
  names.push_back("TAN");    //  Gnomonic
  names.push_back("SIN");    //  Orthographics/synthesis
  names.push_back("STG");    //  Stereographic
  names.push_back("ARC");    //  zenith/azimuthal equidistant
//   names.push_back("ZPN");    //  zenithal/azimuthal polynomial
  names.push_back("ZEA");    //  zenithal/azimuthal equal area
  names.push_back("AIR");    //  Airy
  names.push_back("CYP");    //  Cylindrical perspective
  names.push_back("CAR");    //  Plate carree
  names.push_back("MER");    //  Mercator

  return names;
}

//_______________________________________________________________________________
//                                                               mdirections_list

/*!
  \brief Get the list of known celectial reference codes
*/
std::vector<std::string> mdirections_list () 
{
  std::vector<std::string> names;

  names.push_back("J2000");      // mean equator and equinox at J2000.0 (FK5)
  names.push_back("JNAT");       // geocentric natural frame
  names.push_back("JMEAN");      // mean equator and equinox at frame epoch
  names.push_back("JTRUE");      // true equator and equinox at frame epoch
  names.push_back("APP");        // apparent geocentric position
  names.push_back("B1950");      // mean epoch and ecliptic at B1950.0
  names.push_back("B1950_VLA");  // mean epoch(1979.9)) and ecliptic at B1950.0
  names.push_back("BMEAN");      // mean equator and equinox at frame epoch
  names.push_back("BTRUE");      // true equator and equinox at frame epoch
  names.push_back("GALACTIC");   // galactic coordinates
  names.push_back("HADEC");      // topocentric HA and declination
  names.push_back("AZEL");       // topocentric Azimuth and Elevation (N through E)
  names.push_back("AZELSW");     // topocentric Azimuth and Elevation (S through W)
  names.push_back("AZELGEO");    // topocentric Azimuth and Elevation (N through E)
  names.push_back("AZELSWGEO");  // geodetic Azimuth and Elevation (N through E)
  names.push_back("ECLIPTIC");
  names.push_back("MECLIPTIC");
  names.push_back("TECLIPTIC");
  names.push_back("SUPERGAL");
  names.push_back("ITRF");
  names.push_back("TOPO");
  names.push_back("ICRS");
    
  return names;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Get a casa::DirectionCoordinate object

  \param shape      -- Number of element in the plane of the sky; the value is
         used to set the reference pixel.
  \param refcode    -- Reference code for the celestial coordinate system.
  \param projection -- Reference code for the spherical map projection.
*/
casa::DirectionCoordinate direction_coordinate (casa::IPosition const &shape,
						std::string const &refcode,
						std::string const &projection)
{
  casa::Vector<double> refPixel(2);
  casa::Vector<casa::Quantum<double> > refValue(2);
  casa::Vector<casa::Quantum<double> > increment(2);
  casa::Matrix<double> xform(2,2);
  casa::MDirection md;
  casa::MDirection::Types mdType;
  casa::Projection proj;
  casa::Projection::Type projType (casa::Projection::STG);
  
  // Reference pixel is center of map for directional components
  refPixel(0) = shape(0)/2.0;
  refPixel(1) = shape(1)/2.0;
  // Reference value: local zenith
  refValue(0) = casa::Quantum<double>( 0,"deg");
  refValue(1) = casa::Quantum<double>(90,"deg");
  // Coordinate increment
  increment(0) = casa::Quantum<double>(-1,"deg");
  increment(1) = casa::Quantum<double>( 1,"deg");
  // Linear transform
  xform            = 0.0;
  xform.diagonal() = 1.0;
  // Get the type for the reference code
  md.getType(mdType,refcode);
  // Get the typ for the spherical map projection
  projType = proj.type(projection);
  
  return casa::DirectionCoordinate (mdType,
				    projType,
				    refValue(0),
				    refValue(1),
				    increment(0),
				    increment(1),
				    xform,
				    refPixel(0),
				    refPixel(1));
}

//_______________________________________________________________________________
// 

/*!
  \brief Test combinations of celestial reference system and map projection

  \param nelem -- The number of elements per coordinate axis; for a simple
         two-dimensional image with only a longitudinal and and latitudinal
	 coordinate the total number of pixels therefore will be
	 <tt>nelem x nelem</tt>.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_direction_coordinate (int const &nelem=50)
{
  cout << "\n[tcasacore_images::test_direction_coordinate]\n" << endl;

  int nofFailedTests (0);

  bool status;
  casa::String error;
  casa::IPosition shape (2,nelem,nelem);
  casa::TiledShape tile (shape);
  casa::Array<float> pixels (shape,1.0);
  casa::IPosition where (2,0,0);
  casa::IPosition stride (2,1,1);
  std::vector<std::string> projections = projections_list();
  std::vector<std::string> mdirections = mdirections_list();
  
  unsigned int nofProjections = projections.size();
  unsigned int nofMDirections = mdirections.size();

  /* Go through the combinations of celestial reference code and spherial
     map projection and create a direction coordinate for it.
  */
  for (unsigned int n(0); n<nofMDirections; n++) {
    for (unsigned int proj(0); proj<nofProjections; proj++) {
      cout << "-- Testing combination ["
	   << mdirections[n]
	   << ","
	   << projections[proj]
	   << "] ..." << endl;
      // Create direction coordinate
      casa::DirectionCoordinate dir = direction_coordinate (shape,
							    mdirections[n],
							    projections[proj]);
      // Create coordinate system and add direction coordinate
      casa::CoordinateSystem csys;
      csys.addCoordinate (dir);
      // Set image filename
      std::string filename ("image-"+mdirections[n]+"-"+projections[proj]+".h5");
      std::string outfile  ("!image-"+mdirections[n]+"-"+projections[proj]+".fits");
      
      /* Create image and provide a short summary of its properties */
      {
	casa::HDF5Image<float> image (tile,
				      csys,
				      filename);
	image.doPutSlice (pixels,where,stride);
      }
      /* Convert the previously created image to FITS */
      {
	casa::HDF5Image<float> image (filename);
	status = casa::ImageFITSConverter::ImageToFITS (error,
							image,
							outfile);
      }
    }
  }  
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_image_size

/*!
  \brief Test how export to FITS behaves under increasing images sizes

  \param min  -- Minimum number of elements per coordinate axis.
  \param max  -- Maximum number of elements per coordinate axis.
  \param step -- Step width in which to increment the number of elements per
         coordinate axis.
*/
int test_image_size (int const &min=50,
		     int const &max=500,
		     int const &step=10)
{
  cout << "\n[tcasacore_images::test_image_size]\n" << endl;

  int nofFailedTests (0);

  bool status;
  std::string refcode ("J2000");
  casa::String error;
  casa::IPosition where (2,0,0);
  casa::IPosition stride (2,1,1);
  
  for (int nelem=min; nelem<max; nelem+=step) {
    
    std::ostringstream sidelength;
    sidelength << nelem;
    casa::IPosition shape (2,nelem,nelem);
    casa::TiledShape tile (shape);
    casa::Array<float> pixels (shape,1.0);
    
    cout << "-- Testing shape " << shape << " ..." << endl;
    
    // Create direction coordinate
    casa::DirectionCoordinate dir = direction_coordinate (shape,
							  refcode,
							  "STG");
    // Create coordinate system and add direction coordinate
    casa::CoordinateSystem csys;
    csys.addCoordinate (dir);
    // Set image filename
    std::string filename ("image-" + refcode + "-STG-"  + sidelength.str() + ".h5");
    std::string outfile  ("!image-" + refcode + "-STG-" + sidelength.str() + ".fits");
    
    /* Create image and provide a short summary of its properties */
    {
      cout << "--> creating image ..." << endl;
      casa::HDF5Image<float> image (tile,
				    csys,
				    filename);
      cout << "--> putting slice into image ..." << endl;
      image.doPutSlice (pixels,where,stride);
    }
    /* Convert the previously created image to FITS */
    {
      cout << "--> opening previously created image ..." << endl;
      casa::HDF5Image<float> image (filename);
      //
      cout << "--> converting image to FITS ..." << endl;
      status = casa::ImageFITSConverter::ImageToFITS (error,
						      image,
						      outfile);
    }
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  nofFailedTests += test_direction_coordinate ();

  nofFailedTests += test_image_size (60,150,5);
  
  return nofFailedTests;
}
