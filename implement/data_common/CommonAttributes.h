/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#ifndef COMMONATTRIBUTES_H
#define COMMONATTRIBUTES_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

// DAL header files
#include <Filename.h>
#include <HDF5CommonInterface.h>
#include <AttributesInterface.h>
#include <CommonAttributesProject.h>
#include <CommonAttributesObservation.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CommonAttributes
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Collection of attributes common to all LOFAR datasets
    
    \author Lars B&auml;hren

    \date 2009/08/31

    \test tCommonAttributes.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICDs:
      <ul>
        <li>\ref dal_icd_001
        <li>\ref dal_icd_002
        <li>\ref dal_icd_003
        <li>\ref dal_icd_004
        <li>\ref dal_icd_005
        <li>\ref dal_icd_006
        <li>\ref dal_icd_007
        <li>\ref dal_icd_008
      </ul>
      <li>Components of the LOFAR user software:
      <ul>
        <li>Filename -- Class to generate filenames matching the LOFAR convention
      </ul>
      <li>Components of the LOFAR system software:
      <ul>
        <li>LCS/ApplCommon/include/ApplCommon/Observation.h
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    This class implements the set of <i>LOFAR Common Root Attributes</i>, as they
    are listed in the LOFAR Data Format ICDs.
    
    \image html lofar_common_metadata.png
    
    <h3>Example(s)</h3>

  */  
  class CommonAttributes : public AttributesInterface {

    //! LOFAR group type ("Root")
    std::string itsGroupType;
    //! File name
    std::string itsFilename;
    //! File creation date, YYYY-MM-DDThh:mm:ss.s
    std::string itsFiledate;
    //! File type
    std::string itsFiletype;
    //! Name of the telescope ("LOFAR")
    std::string itsTelescope;
    //! Name(s) of the observer(s)
    std::string itsObserver;

    //! Common LOFAR attributes for description of project
    CommonAttributesProject itsAttributesProject;
    //! Common LOFAR attributes for description of observation
    CommonAttributesObservation itsAttributesObservation;

    //! Clock frequency (LOFAR: 200.0 or 160.0)
    double itsClockFrequency;
    //! Clock frequency physical unit
    std::string itsClockFrequencyUnit;
    //! Antenna set specification of observation
    std::string itsAntennaSet;
    //! Filter selection
    std::string itsFilterSelection;
    //! Single or list of observation targets/sources
    std::string itsTarget;
    //! Processing system name/version
    std::string itsSystemVersion;
    //! Processing pipeline name
    std::string itsPipelineName;
    //! Processing pipeline version
    std::string itsPipelineVersion;
    //! Notes or comments
    std::string itsNotes;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CommonAttributes ();
    
    //! Argumented constructor
    CommonAttributes (Filename const &filename,
		      std::string const &filetype,
		      std::string const &filedate);

    //! Argumented constructor
    CommonAttributes (CommonAttributesProject const &attributesProject);

    //! Argumented constructor
    CommonAttributes (CommonAttributesObservation const &attributesObservation);

#ifdef HAVE_HDF5
    //! Argumented constructor
    CommonAttributes (hid_t const &locationID);
#endif
    
    /*!
      \brief Copy constructor
      
      \param other -- Another CommonAttributes object from which to create this new
             one.
    */
    CommonAttributes (CommonAttributes const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~CommonAttributes ();

    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another CommonAttributes object from which to make a copy.
    */
    CommonAttributes& operator= (CommonAttributes const &other); 
    
    // === Parameter access =====================================================

    /*!
      \brief Get the LOFAR group type
      \return groupName -- The name of the LOFAR group
    */
    inline std::string groupType () const {
      return itsGroupType;
    }
    
    //! Get the name of the file
    inline std::string filename () const {
      return itsFilename;
    }
    
    //! Set the name of the file
    inline void setFilename (Filename const &name) {
      Filename tmp = name;
      itsFilename = tmp.filename();
    }
    
    //! Get the type of the file
    inline std::string filetype () const {
      return itsFiletype;
    }
    
    //! Set the type of the file
    inline void setFiletype (std::string const &filetype) {
      itsFiletype = filetype;
    }
    
    //! Get the file creation date
    inline std::string filedate () const {
      return itsFiledate;
    }

    //! Set the file creation date
    inline void setFiledate (std::string const &filedate) {
      itsFiledate = filedate;
    }

    //! Get the name of the telescope
    inline std::string telescope () const {
      return itsTelescope;
    }

    //! Set the name of the telescope
    inline void setTelescope (std::string const &telescope) {
      itsTelescope = telescope;
    }

    //! Get the name(s) of the observer(s)
    inline std::string observer () const {
      return itsObserver;
    }
    
    //! Name(s) of the observer(s)
    inline void setObserver (std::string const &observer) {
      itsObserver = observer;
    }

    //! Common LOFAR attributes for description of project
    CommonAttributesProject attributesProject () const {
      return itsAttributesProject;
    }

    //! Common LOFAR attributes for description of project
    void setAttributesProject (CommonAttributesProject const &attributesProject);
    
    //! Common LOFAR attributes for description of project
    void setAttributesProject (std::string const &projectID,
			       std::string const &projectTitle,
			       std::string const &projectPI,
			       std::string const &projectCoI,
			       std::string const &projectContact);
    
    //! Get common LOFAR attributes for description of observation
    CommonAttributesObservation attributesObservation () const {
      return itsAttributesObservation;
    }

    //! Set observation start date
    void setObservationStart (std::string const &startMJD,
			      std::string const &startTAI,
			      std::string const &startUTC);

    //! Set observation end date
    void setObservationEnd (std::string const &endMJD,
			    std::string const &endTAI,
			    std::string const &endUTC);

    //! Get clock frequency (LOFAR: 200.0 or 160.0)
    inline double clockFrequency () const {
      return itsClockFrequency;
    }
    //! Set clock frequency (LOFAR: 200.0 or 160.0)
    inline void setClockFrequency (double const &clockFrequency) {
      itsClockFrequency = clockFrequency;
    }

    //! Get clock frequency physical unit
    inline std::string clockFrequencyUnit () const {
      return itsClockFrequencyUnit;
    }
    //! Set clock frequency physical unit
    inline void setClockFrequencyUnit (std::string const &clockFrequencyUnit) {
      itsClockFrequencyUnit = clockFrequencyUnit;
    }

    //! Get antenna set specification of observation
    inline std::string antennaSet () const {
      return itsAntennaSet;
    }
    //! Set antenna set specification of observation
    inline void setAntennaSet (std::string const &antennaSet) {
      itsAntennaSet = antennaSet;
    }

    //! Get filter selection
    inline std::string filterSelection () const {
      return itsFilterSelection;
    }
    //! Set filter selection
    inline void setFilterSelection (std::string const &filterSelection) {
      itsFilterSelection = filterSelection;
    }

    //! Get single or list of observation targets/sources
    inline std::string target () const {
      return itsTarget;
    }
    //! Set single or list of observation targets/sources
    inline void setTarget (std::string const &target) {
      itsTarget = target;
    }

    //! Get processing system name/version
    inline std::string systemVersion () const {
      return itsSystemVersion;
    }
    //! Set processing system name/version
    inline void setSystemVersion (std::string const &systemVersion) {
      itsSystemVersion = systemVersion;
    }

    //! Processing pipeline name
    inline std::string pipelineName () const {
      return itsPipelineName;
    }
    //! Set processing pipeline name
    inline void setPipelineName (std::string const &pipelineName) {
      itsPipelineName = pipelineName;
    }

    //! Processing pipeline version
    inline std::string pipelineVersion () const {
      return itsPipelineVersion;
    }
    //! Set processing pipeline version
    inline void setPipelineVersion (std::string const &pipelineVersion) {
      itsPipelineVersion = pipelineVersion;
    }

    //! Notes or comments
    inline std::string notes () const {
      return itsNotes;
    }
    //! Notes or comments
    inline void setNotes (std::string const &notes) {
      itsNotes = notes;
    }
    
    // === Methods ==============================================================
    
    //! Get the value of an attribute
    template<class T >
      bool getAttribute (std::string const &name, T &val);

    //! Get the value of an attribute
    template<class T >
      bool getAttribute (std::string const &name, std::vector<T> &val);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CommonAttributes.
    */
    inline std::string className () const {
      return "CommonAttributes";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // Methods which require HDF5 __________________________

#ifdef HAVE_HDF5
    //! Write the attributes to a HDF5 file
    bool h5write (hid_t const &groupID);
    
    //! Write the attributes to a HDF5 file
    bool h5write (hid_t const &locationID,
		  std::string const &name);
    
    //! Read the attributes from a HDF5 file
    bool h5read (hid_t const &groupID);
    
    //! Read the attributes from a HDF5 file
    bool h5read (hid_t const &locationID,
		 std::string const &name);
#endif    

    // Methods which require casacore ______________________

#ifdef DAL_WITH_CASA
    //! Retrieve common attributes using csac::Record as container
    bool getAttributes (casa::Record &rec);
#endif 
    
  private:

    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Unconditional copying
    void copy (CommonAttributes const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CommonAttributes -- end
  
} // Namespace DAL -- end

#endif /* COMMONATTRIBUTES_H */
  
