/***************************************************************************
 *   Copyright (C) 2006                                                    *
 *   Andreas Horneffer (<mail>)                                            *
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

#include <data_hl/LOPES_EventFile.h>

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  LOPES_EventFile::LOPES_EventFile()
  {
    init();
  }
  
  /*!
    \param filename -- name (incl. path) of the lopes-eventfile to be read.
  */
  LOPES_EventFile::LOPES_EventFile (std::string filename)
  {
    init();
    attachFile(filename);
  }
  
  void LOPES_EventFile::init()
  {
    nofAntennas_p = 0;
    itsFilename   = "";
    attached_p    = false;
    itsHeaderData = (lopesevent_v1*)malloc(LOPESEV_HEADERSIZE);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  LOPES_EventFile::~LOPES_EventFile()
  {
    destroy();
  }
  
  void LOPES_EventFile::destroy()
  {
    // release the memory allocated for the header structure
    free(itsHeaderData);
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   attachFile
  
  /*!
    \param filename -- name (incl. path) of the lopes-eventfile to be read.
    
    \return ok -- True if successfull
  */
  bool LOPES_EventFile::attachFile (std::string filename)
  {
    int i                = 0;
    unsigned int ui      = 0;
    unsigned int tmpchan = 0;
    unsigned int tmplen  = 0;
    short *tmppoint;
    
    FILE *fd = fopen(filename.c_str(),"r");

    if (fd == NULL) {
      cerr << "LOPESEventIn:attachFile: Can't open file: " << filename << endl;
      return false;
    };
    attached_p = false;
    ui = fread(itsHeaderData, 1, LOPESEV_HEADERSIZE, fd);
    if ( (ui != LOPESEV_HEADERSIZE ) || (itsHeaderData->type != TIM40) ) {
      cerr << "LOPESEventIn:attachFile: Inconsitent file: " << filename << endl;
      fclose(fd);
      return false;
    };
    nofAntennas_p =0;
    fread(&tmpchan, 1, sizeof(unsigned int), fd);
    while (!feof(fd)) {
      fread(&tmplen, 1, sizeof(unsigned int), fd);
      if (itsHeaderData->blocksize==0) {
	itsHeaderData->blocksize = tmplen;
      };
      if (itsHeaderData->blocksize != tmplen) {
	cerr << "LOPESEventIn:attachFile: Inconsitent file (different blocksizes): "
	     << filename
	     << endl;
	fclose(fd);
	return false;
      };
      i = fseek(fd,tmplen*sizeof(short),SEEK_CUR);
      if (i != 0) {
	cerr << "LOPESEventIn:attachFile: Inconsitent file (unexpected end): "
	     << filename
	     << " in channel " << tmpchan
	     << " len:"        << tmplen
	     << endl;
	fclose(fd);
	return false;
      };
      nofAntennas_p++;
      
      //this will trigger eof condition after last channel;
      fread(&tmpchan, 1, sizeof(unsigned int), fd);
    };
    try {
      AntennaIDs_p.resize(nofAntennas_p);
#ifdef DAL_WITH_CASACORE
      channeldata_p.resize(itsHeaderData->blocksize,nofAntennas_p);
#endif
      tmppoint = (short*)malloc(itsHeaderData->blocksize*sizeof(short));
      if (tmppoint == NULL) {
	cerr << "LOPESEventIn:attachFile: Error while allocating temporary memory " << endl;
	fclose(fd);
	return false;
	
      };
      
      int antenna (0);
      
      // this should reset the eof condition
      fseek(fd,LOPESEV_HEADERSIZE,SEEK_SET);
      for (antenna=0; antenna<nofAntennas_p; antenna++) {
	fread(&tmpchan, 1, sizeof(unsigned int), fd);
	fread(&tmplen, 1, sizeof(unsigned int), fd);
	AntennaIDs_p[antenna] = (int)tmpchan;
	fread(tmppoint, sizeof(short),itsHeaderData->blocksize , fd);
	// channel data for antenna i go into column i of the data array
#ifdef DAL_WITH_CASACORE
	for (unsigned int sample=0; sample<itsHeaderData->blocksize; sample++)
	  {
	    channeldata_p(sample,antenna) = tmppoint[sample];
	  }
#endif
      };
      itsFilename = filename;
      attached_p = true;
    }
    catch (std::string message) {
      cerr << "LOPESEventIn:attachFile: " << message << endl;
      return false;
    };
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                         data
  
  /*!
    \return data -- Pointer to the array data; the number of elements in the 
            array is given by \f$ N_{\rm Antennas} \cdot N_{\rm Blocksize} \f$,
	    where \f$ N_{\rm Antennas} \f$ is the number of antennas in the data
	    set and \f$ N_{\rm Blocksize} \f$ is the number of samples per
	    antenna.

    \code
    LOPES_EventFile event (filename);         // new LOPES_EventFile object
    short *data;                              // array for the extracted data
    
    data = new short[event.nofDatapoints()];  // adjust array size
    
    data = event.data();                      // get the data from the object
    \endcode
  */
  short* LOPES_EventFile::data ()
  {
    short *data = NULL;

#ifdef DAL_WITH_CASACORE
    unsigned int nofElements = channeldata_p.nelements();
    
    try {
      data = new short[nofElements];
      data = channeldata_p.data();
    }
    catch (std::string message) {
      std::cerr << "[LOPES_EventFile::channeldata] " << message << std::endl;
    }    
#endif

    return data;
  }
  
  //_____________________________________________________________________________
  //                                                                         data
  
  void LOPES_EventFile::data (short *data,
			      unsigned int const &channel)
  {
#ifdef DAL_WITH_CASACORE
    // Retrieve the data for the selected channel
    casa::Vector<short> channeldata = LOPES_EventFile::channeldata(channel);
    
    unsigned int nofSamples (channeldata.nelements());
    
    for (unsigned int sample(0); sample<nofSamples; sample++)
      {
	data[sample] = channeldata(sample);
      }
#endif
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void LOPES_EventFile::summary (std::ostream &os)
  {
    // data available as individual data elements
    os << "-- Filename                        : " << itsFilename           << endl;
    os << "-- Object attached to file?          " << attached_p            << endl;
    os << "-- nof. antennas in the file       : " << nofAntennas()         << endl;
    os << "-- Antenna IDs                     : " << AntennaIDs_p          << endl;
#ifdef DAL_WITH_CASACORE
    os << "-- Shape of the channel data array : " << channeldata_p.shape() << endl;
#endif    
    // data stored within the header-data structure
    os << "-- LOPES-Event version             : " << version()             << endl;
    os << "-- Length of the dataset [Bytes]   : " << length()              << endl;
    os << "-- Timestamp (KASCADE)         JDR : " << timestampJDR()        << endl;
    os << "                                TL : " << timestampTL()         << endl;
    os << "-- Type of data                    : " << dataType()            << endl;
    os << "-- Type of stored event            : " << eventClass()          << endl;
    os << "-- Size of one channel             : " << blocksize()           << endl;
    os << "-- Presync                         : " << presync()             << endl;
    os << "-- Timestamp from the manable card : " << timestampLTL()        << endl;
    os << "-- ID of the observatory           : " << observatory()         << endl;
    
  }
  
}  //  end -- namespace DAL
