/***************************************************************************
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

#ifdef DAL_WITH_CASACORE
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Matrix.h>
#include <casa/BasicSL/String.h>
#include <casa/HDF5/HDF5File.h>
#include <casa/HDF5/HDF5Record.h>
#endif

#include <data_hl/TBB_Timeseries.h>

using std::cerr;
using std::cout;
using std::endl;
using DAL::TBB_Timeseries;

/*!
  \file tTBB_Timeseries.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the TBB_Timeseries class

  \author Lars B&auml;hren

  \date 2008/02/06

  <h3>Usage</h3>

  To run the test program use:
  \verbatim
  tTBB_Timeseries <filename>
  \endverbatim
  where the <i>filename</i> points to an existing HDF5 time-series dataset.
*/

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test constructors for a new TBB_Timeseries object

  \return nofFailedTests -- The number of failed tests.
*/
int test_construction ()
{
  cout << "\n[tTBB_Timeseries::test_construction]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tTBB_Timeseries.h5");

  //__________________________________________________________________
  // Test the default constructor

  cout << "[1] Testing TBB_Timeseries() ..." << endl;
  try {
    TBB_Timeseries ts;
    //
    ts.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing TBB_Timeseries(std::string) ..." << endl;
  try {
    TBB_Timeseries ts (filename);
    //
    ts.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test constructors for a new TBB_Timeseries object

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_construction (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_construction]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Testing argumented constructor ..." << endl;
  try {
    TBB_Timeseries ts (filename);
    //
    ts.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing copy constructor ..." << endl;
  try {
    cout << "--> creating original object ..." << endl;
    TBB_Timeseries ts (filename);
    ts.summary();
    //
    cout << "--> creating new object by copy ..." << endl;
    TBB_Timeseries tsCopy (ts);
    tsCopy.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests encountered within this 
          function.
*/
int test_attributes (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_attributes]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  std::cout << "[1] Attributes attached to the root group ..." << endl;
  try {
    std::string telescope;
    std::string observer;
    std::string observationID;
    std::string projectID;
    std::string projectTitle;
    std::string projectPI;
    std::string projectContact;
    //
    ts.getAttribute("TELESCOPE",telescope);
    ts.getAttribute("OBSERVER",observer);
    ts.getAttribute("OBSERVATION_ID",observationID);
    ts.getAttribute("PROJECT_ID",projectID);
    ts.getAttribute("PROJECT_TITLE",projectTitle);
    ts.getAttribute("PROJECT_PI",projectPI);
    ts.getAttribute("PROJECT_CONTACT",projectContact);
    //
    std::cout << "-- TELESCOPE       = " << telescope      << endl;
    std::cout << "-- OBSERVER        = " << observer       << endl;
    std::cout << "-- OBSERVATION_ID  = " << observationID  << endl;
    std::cout << "-- PROJECT_ID      = " << projectID      << endl;
    std::cout << "-- PROJECT_TITLE   = " << projectTitle   << endl;
    std::cout << "-- PROJECT_PI      = " << projectPI      << endl;
    std::cout << "-- PROJECT_CONTACT = " << projectContact << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

#ifdef DAL_WITH_CASACORE
  std::cout << "[2] Attributes attached to the station group ..." << endl;
  try {
    casa::Matrix<double>       stationPositionValue;
    casa::Matrix<casa::String> stationPositionUnit;
    casa::Vector<casa::String> stationPositionFrame;
    casa::Matrix<double>       beamDirectionValue;
    casa::Matrix<casa::String> beamDirectionUnit;
    casa::Vector<casa::String> beamDirectionFrame;
    //
    stationPositionValue = ts.station_position_value();
    stationPositionUnit  = ts.station_position_unit();
    stationPositionFrame = ts.station_position_frame();
    beamDirectionValue   = ts.beam_direction_value();
    beamDirectionUnit    = ts.beam_direction_unit();
    beamDirectionFrame   = ts.beam_direction_frame();
    //
    cout << "-- STATION_POSITION_VALUE = " << stationPositionValue.row(0) << endl;
    cout << "-- STATION_POSITION_UNIT  = " << stationPositionUnit.row(0)  << endl;
    cout << "-- STATION_POSITION_FRAME = " << stationPositionFrame(0)     << endl;
    cout << "-- BEAM_DIRECTION_VALUE   = " << beamDirectionValue.row(0)   << endl;
    cout << "-- BEAM_DIRECTION_UNIT    = " << beamDirectionUnit.row(0)    << endl;
    cout << "-- BEAM_DIRECTION_FRAME   = " << beamDirectionFrame(0)       << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
#endif

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

int test_parameters (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_parameters]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  // Perform the tests _____________________________________

  cout << "[1] Testing objectName() ..." << endl;
  try {
    cout << "-- HDF5 object name = " << ts.objectName() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing attributes() ..." << endl;
  try {
    cout << "-- TBB_StationGroup attributes = " << ts.attributes() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing selectedDipoles() ..." << endl;
  try {
    cout << "-- Selected dipoles = " << ts.selectedDipoles() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various methods 

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests encountered within this 
          function.
*/
int test_methods (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_methods]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  cout << "[1] Access to embedded StationGroup objects ..." << endl;
  try {
    uint nofStations              = ts.nofStationGroups();
    std::set<std::string> names   = ts.stationGroupNames();
    DAL::TBB_StationGroup station = ts.stationGroup(10);
    // Methods of the embedded StationGroup object
    std::string groupName = station.group_name();
    std::string groupType;
    uint nofDipoles;
    // Retrieve attribute values
    station.getAttribute ("GROUPTYPE", groupType);
    station.getAttribute ("NOF_DIPOLES", nofDipoles);
    //
    cout << "-- nof. station groups = " << nofStations << endl;
    cout << "-- Station group names = " << names       << endl;
    cout << "-- Station group name  = " << groupName   << endl;
    cout << "-- GROUPTYPE           = " << groupType   << endl;
    cout << "-- NOF_DIPOLES         = " << nofDipoles  << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

#ifdef DAL_WITH_CASACORE

/*!
  \brief Test retrieval of TBB data

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_data]\n" << endl;

  int nofFailedTests = 0;
  int start          = 0;
  int nofSamples     = 1024;
  TBB_Timeseries ts (filename);
  std::set<std::string> dipoles = ts.selectedDipoles();
  
  cout << "[1] Retrieve time-series data without channel selection" << endl;
  try {
    casa::Matrix<double> data;
    
    ts.readData (data, start, nofSamples);

    cout << "-- Data start     = " << start        << endl;
    cout << "-- Data blocksize = " << nofSamples   << endl;
    cout << "-- Data array     = " << data.shape() << endl;
    cout << "-- Data [0,]      = " << data.row(0)  << endl;
    cout << "-- Data [1,]      = " << data.row(1)  << endl;
    cout << "-- Data [2,]      = " << data.row(2)  << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Retrieve data for selected dipoles ..." << endl;
  try {
    casa::Matrix<double> data;
    uint nofDipoles = dipoles.size();
    std::set<std::string>::iterator it;

    for (uint n(0); n<(nofDipoles-1); ++n) {
      // remove the first element from the selection
      it = dipoles.begin();
      cout << "-- removing dipole " << *it << " from the selection ..." << endl;
      dipoles.erase(it);
      // set the new selection ...
      ts.selectDipoles(dipoles);
      // ... and retrieve the data
      ts.readData (data, start, nofSamples);
      //
      cout << " --> Dipoles     = " << ts.selectedDipoles() << endl;
      cout << " --> shape(data) = " << data.shape()         << endl;
      cout << " --> data [0,]   = " << data.row(0)          << endl;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }

  cout << "[3] Retrieve data after selecting all dipoles ..." << endl;
  try {
    casa::Matrix<double> data;
    // Reset the dipole selection and retrive the data
    ts.selectAllDipoles();
    ts.readData (data, start, nofSamples);
    //
    cout << " --> Dipoles     = " << ts.selectedDipoles() << endl;
    cout << " --> shape(data) = " << data.shape()         << endl;
    cout << " --> data [0,]   = " << data.row(0)          << endl;
    cout << " --> data [1,]   = " << data.row(1)          << endl;
    cout << " --> data [2,]   = " << data.row(2)          << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

#endif

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("UNDEFINED");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  nofFailedTests += test_construction ();

  if (haveDataset) {
    // Test constructors for TBB_Timeseries object
    nofFailedTests += test_construction (filename);
    // Test access to the attributes
    nofFailedTests += test_attributes (filename);
    // Test access to the parameters
    nofFailedTests += test_parameters (filename);
    // Test test various methods provided by the class
    nofFailedTests += test_methods (filename);
#ifdef DAL_WITH_CASACORE
    // Test access to the data stored within the dipole datasets
    nofFailedTests += test_data (filename);
#endif
  } else {
    std::cout << "\n[tTBB_Timeseries] Skipping tests with input dataset.\n"
	      << endl;
  }
  
  return nofFailedTests;
}
