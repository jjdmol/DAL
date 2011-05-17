/***************************************************************************
 *   Copyright (C) 2007 by Lars B"ahren                                    *
 *   bahren@astron.nl                                                      *
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

#ifndef LOPES_EVENTFILE_H
#define LOPES_EVENTFILE_H

#include <string>
#include <iostream>

// Custom header files
#include <core/dalCommon.h>
#include <data_hl/lopesevent.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

namespace DAL {  // Namespace DAL -- begin
  
  //! Not more than 30 antennas available (yet).
#define MAX_NUM_ANTENNAS 31
  //! LOPES et al. have a (fixed) samplerate of 80 MSPS
#define LOPES_SAMPLERATE 80e6
  //! LOPES and LORUN work in the 2nd Nyquist zone.
#define LOPES_NYQUIST_ZONE 2
  
  /*!
    \class LOPES_EventFile
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Read in LOPES event files
    
    \author Andreas Horneffer, Lars B&auml;hren
    
    \date 2006/12/14
    
    \test tLOPES_EventFile.cc
    
    <h3>Prerequisite</h3>
    
    <h3>Synopsis</h3>
    
    This is a port of Andreas Horneffer's original LopesEventIn class as
    distributed with the LOPES-Tools software package. The main difference
    w.r.t. the original implementation is, that there is no longer any
    dependency on the DataReader class.
    
    <h3>Example(s)</h3>
    
    Here is a simple example fro creating a an LOPES_EventFile object to read in
    the data stored in a file:
    \code
    LOPES_EventFile event (filename);                    // Create new object
    casa::Matrix<short> data = event.channeldata();    // Retrieve the data
    \endcode
    If you do not want to retrieve the data themselves in the form of a CASA
    array, you can use the following:
    \code
    short *data;                              // array for the extracted data
    data = new short[event.nofDatapoints()];  // adjust array size
    data = event.data();                      // get the data from the object
    \endcode
    
  */
  class LOPES_EventFile {
    
  public:
    
    //! Type of event for which the data have been stored
    enum EvType
    {
      //! Unspecified event
      Unspecified,
      //! Cosmic ray
      Cosmic,
      //! Simulation output
      Simulation,
      //! Test event
      Test,
      //! Solar flare event
      SolarFlare,
      //! Other, not further specified event
      Other
    };
    
    //! List of observatories from which to expect data
    enum Observatory
    {
      //! LOFAR Prototype Station
      LOPES,
      //! LOFAR at Radboud Universiteit Nijmegen
      LORUN
    };
    
  private:
    
    //! filename (incl. path) of the file to be read
    std::string itsFilename;
    //! Is this object attached to a file?
    bool attached_p;
    //! Pointer to the (header-)data
    lopesevent *itsHeaderData;
    //! Number of antennas in the event
    int nofAntennas_p;
    //! Vector with the antenna IDs
    std::vector<int> AntennaIDs_p;
#ifdef DAL_WITH_CASACORE
    //! Matrix with the data itself
    casa::Matrix<short> channeldata_p;
#endif 
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    LOPES_EventFile();
    //! Augmented constructor
    LOPES_EventFile(std::string filename);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    virtual ~LOPES_EventFile();

    // === Parameter access =====================================================

    /*!
      \brief Get the name of the data file.
      \return filename -- Name of the data file
    */
    inline std::string filename () const {
      return itsFilename;
    }
    
    /*!
      \brief Get the samplerate of the A/D conversion
      \return samplerate -- The samplerate of the A/D conversion, [Hz]
    */
    inline double samplerate () {
      return LOPES_SAMPLERATE;
    }
    
    /*!
      \brief Get the Nyquist zone in which the data are sampled
      \return nyquistZone -- The Nyquist zone in which the data are sampled
    */
    inline unsigned int nyquistZone () {
      return LOPES_NYQUIST_ZONE;
    }
    
    /*!
      \brief Get the number of antennas in the data set
      \return nofAntennas -- The number of antennas in the data set
    */
    inline int nofAntennas () {
      return nofAntennas_p;
    }
    
#ifdef DAL_WITH_CASACORE
    /*!
      \brief Get the number of data points stored
      
      \return nofDatapoints -- The number of data points: \f$ N_{\rm Antennas} \cdot
      N_{\rm Blocksize} \f$, where \f$ N_{\rm Antennas} \f$
      is the number of antennas in the data set and
      \f$ N_{\rm Blocksize} \f$ is the number of samples
      per antenna
    */
    inline unsigned int nofDatapoints ()
    {
      return channeldata_p.nelements();
    }
    
    /*!
      \brief Get the channel data
      \return channeldata -- [sample,antenna] Data for the individual channels,
              i.e. dipoles
    */
    inline casa::Matrix<short> channeldata ()
    {
      return channeldata_p;
    }
    
    /*!
      \brief Get the channel data
      \param channel -- Channel/Antenna for which to return the data
      \return channeldata -- Data for the individual channels, i.e. dipoles
    */
    inline casa::Vector<short> channeldata (unsigned int const &channel)
    {
      return channeldata_p.column(channel);
    }
#endif
    
    // === Methods ==============================================================
    
    //! Attach to a (another) lopes-eventfile
    bool attachFile (std::string filename);
    
    //! Get the channel data
    short* data ();
    
    /*!
      \brief Get the channel data
      
      Keep in mind that memory has to be alloacted properly in advance:
      \code
      LOPES_EventFile event (filename);                // new object for data file
      
      unsigned int blocksize (event.blocksize());    // the nof. sampler per antenna
      unsigned int nofAntennas (event.nofAntennas()) // nof. antennas in the data set
      
      short *data;
      data = new short [blocksize];
      
      for (unsigned int antenna(0); antenna<nofAntennas; antenna++) {
      event.data (data,antenna);  // retreive the data for this antenna
      }
      
      \endcode
      
      \retval data   -- Pointer to the array of data for antenna <i>channel</i>
      \param channel -- Channel/Antenna for which to return the data
    */
    void data (short *data,
	       unsigned int const &channel);
    
    // ---------------------------------------------------------------- Header data
    
  /*!
    \brief Get the LOPES-Event version
    
    \return version -- The LOPES-Event version
  */
  inline unsigned int version ()
  {
    return itsHeaderData->version;
  }
  
  /*!
    \brief Get the length of the data set (in bytes)
    
    \return length -- The length of the data set (in bytes)
  */
  inline unsigned int length ()
  {
    return itsHeaderData->length;
  }
  
  /*!
    \brief Get the size of one channel of data
    
    \return blocksize -- Size of one channel (number of shorts)
  */
  inline unsigned int blocksize ()
  {
    return itsHeaderData->blocksize;
  }
  
  /*!
    \brief Get the presync of the first entry w.r.t. the sync signal
    
    \return presync -- First entry is presync before the sync signal
  */
  inline int presync ()
  {
    return itsHeaderData->presync;
  }
  
  /*!
    \brief Get the type of data stored in the file
    
      \return evclass -- The type of data stored in the file (currently only TIM-40
                         supported)
    */
    inline unsigned int dataType ()
    {
      return itsHeaderData->type;
    }

    /*!
      \brief Get the type of the stored event

      \return evclass -- The type of the stored event
    */
    inline unsigned int eventClass ()
    {
      return itsHeaderData->evclass;
    }

    /*!
      \brief Get the ID of the observatory

      \return observatory -- The ID of the observatory
    */
    inline unsigned int observatory ()
    {
      return itsHeaderData->observatory;
    }

    /*!
      \brief KASCADE-style timestamp (JDR)

      \return JDR -- KASCADE-style timestamp (JDR)
    */
    inline unsigned int timestampJDR ()
    {
      return itsHeaderData->JDR;
    }

    /*!
      \brief KASCADE-style timestamp (TL)

      \return TL -- KASCADE-style timestamp (TL)
    */
    inline unsigned int timestampTL ()
    {
      return itsHeaderData->TL;
    }

    /*!
      \brief Get the 40MHz timestamp from menable card

      \return LTL -- 40MHz timestamp from menable card
    */
    inline unsigned int timestampLTL ()
    {
      return itsHeaderData->LTL;
    }

    // ------------------------------------------------------------------- Feedback

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, LOPES_EventFile.
    */
    virtual std::string className () const {
      return "LOPES_EventFile";
    }
    
    //! Provide a summary of the objects's internal data
    inline void summary () {
      summary (std::cout);
    }
    
    /*!
      \brief Provide a summary of the objects's internal data
      
      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);

  protected:

    /*!
      \brief Fill the header information into a header record.

      \return status -- Status of the operation; returns <i>true</i> if
      everything went fine.
    */
    /*     bool generateHeaderRecord(); */

  private:

    //! Unconditional copying
    void copy (LOPES_EventFile const &other);
    //! Unconditional deletion
    void destroy(void);
    //! Initialization
    void init ();

  };  //  end -- class LOPES_EventFile

}  //  end -- namespace DAL

#endif /* LOPES_EVENTFILE_H */
