/***************************************************************************
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

#ifndef TBB_H
#define TBB_H

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fstream>
#include <string>

#include <core/dalDataset.h>

#define ETHEREAL_HEADER_LENGTH = 46;
#define FIRST_EXTRA_HDR_LENGTH = 40;
#define EXTRA_HDR_LENGTH = 16;

//!size of the buffer for the UDP-datagram
// 1 byte larger than the frame size (subband-frames are smaller)
#define UDP_PACKET_BUFFER_SIZE 2141

// switch for large input buffer
// define this to activate the input buffer
#define USE_INPUT_BUFFER
// number of frames in the input buffer (50000 is ca. 100MB)
//(the vBuf of the system on the storage nodes can store ca. 3600 frames!)
#define INPUT_BUFFER_SIZE 50000

namespace DAL {
  
  /*!
    \class TBB
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface between TBB data and the DAL
    
    \author Joseph Masters, Lars B&auml;hren, Andreas Horneffer
    
    \test tTBB.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
    <li>
    </ul>
    
    <h3>Synopsis</h3>


    <h3>Example(s)</h3>

  */
  class TBB {
    
    //___________________________________________________________________________
    // Constant values and structures
    
    //! Structure for the storage of data for an individual antenna
    struct AntennaStruct
    {
      //! Counter for the frame number
      unsigned int frameno;
      //! Identifier for the RSP board
      unsigned int rsp_id;
      //! Identifier for the RCU board
      unsigned int rcu_id;
      //! Time for the first sample of the dipole dataset
      unsigned int time;
      //! Offset in number of samples
      unsigned int sample_nr;
      //! Number of samples sent within a frame of data
      unsigned int samples_per_frame;
      //! Type of antenna feed (dipole type)
      char feed[16];
      //! Antenna position
      double ant_position[ 3 ];
      //! Antenna orientation
      double ant_orientation[ 3 ];
      //! The data themselves
      hvl_t data[1];
    };
    
    //! Structure storing the buffer which is getting written to output
    struct writebuffer
    {
      AntennaStruct antenna;
    };
    
    //! Sample when using TBB in time-series mode
    struct TransientSample
    {
      Int16 value;
    };
    
    //! Sample when using TBB in spectral mode
    struct SpectralSample
    {
      std::complex<Int16> value;
    };
    
    //! Structure storing metadata stored in the TBB header block
    struct TBB_Header
    {
      //! The identifier to the station
      unsigned char stationid;
      //! The identifier for the RSP board
      unsigned char rspid;
      //! The identifier for the RCU board
      unsigned char rcuid;
      //! The sample frequency in the A/D conversion
      unsigned char sample_freq;
      UInt32 seqnr;
      Int32 time;
      UInt32 sample_nr;
      //! The number of samples per frame
      UInt16 n_samples_per_frame;
      UInt16 n_freq_bands;
      char bandsel[64];
      Int16 spare;
      UInt16 crc;
    };
    
    //! Name of the output HDF5 file
    std::string name;
    //! Name of the telescope
    std::string telescope_p;
    //! Name of the observer
    std::string observer_p;
    //! Name of the project
    std::string itsProject;
    //! Observation ID
    std::string observation_id_p;
    //! Telescope observation mode
    std::string observationMode_p;
    //! Status tracking
    int status;
    UInt32 seqnrLast_p;
    //! Is the system big-endian?
    bool bigendian_p;
    time_t sampleTime_p;  // For date
    dalDataset * dataset;
    std::vector<dalGroup> station;
    fd_set readSet;
    struct timeval timeoutStart_p;
    struct timeval timeoutRead_p;
#ifdef USE_INPUT_BUFFER
    //!pointers (array indices) for the last buffer processed and the last buffer written
    int inBufProcessID,inBufStorID;
    //!the Input Buffer
    //    char inputBuffer_P[INPUT_BUFFER_SIZE][UDP_PACKET_BUFFER_SIZE];
    char * inputBuffer_P;
    //!pointer to the UDP-datagram
    char *udpBuff_p;
    //!maximum number of frames waiting in the vBuf while reading
    int maxWaitingFrames;
#else
    //!buffer for the UDP-datagram
    char udpBuff_p[UDP_PACKET_BUFFER_SIZE];
#endif
    //!header of the TBB-frame
    TBB_Header header;
    //!header of the TBB-frame
    TBB_Header *headerp_p;
    int rr;
    int main_socket;
    struct sockaddr_in incoming_addr;
    unsigned int socklen;
    std::vector<std::string> stations;
    dalGroup * stationGroup_p;
    dalArray * dipoleArray_p;
    std::vector<std::string> dipoles;
    //! Definition of array dimensions (shape)
    std::vector<int> dims;
    int offset_p;
    std::vector<int> cdims;
    //! Name of the HDF5 group storing data for a station
    //char * stationstr;
    //! Unique identifier for an individual dipole
    char uid[10];
#ifdef USE_INPUT_BUFFER
    //! Read data from the socket and/or set udpBuff_p to next frame in buffer
    int readSocketBuffer();
#else
    //! Read data from a socket
    int readsocket( unsigned int nbytes,
		    char* buf);
#endif
    UInt32 payload_crc;
    TransientSample tran_sample;
    SpectralSample spec_sample;
    // for file i/o
    std::ifstream::pos_type size;
    unsigned char * memblock;
    std::fstream * rawfile_p;
    Int16 real_part, imag_part;
    
  public:
    
    bool first_sample;
#ifdef USE_INPUT_BUFFER
    int noFramesDropped;
#endif
    //___________________________________________________________________________
    // Construction/Destruction
    
    //! Constructor
    TBB (std::string const &outfile);
    //! Constructor
    TBB (std::string const &outfile,
	 std::string const &telescope,
	 std::string const &observer,
	 std::string const &project);
    //! Destructor
    ~TBB();
    
    //___________________________________________________________________________
    // Parameter access
    
    /*!
      \brief Get the name of the telescope
      \return telescope -- The name of the telescope
    */
    inline std::string telescope () const {
      return telescope_p;
    }
    /*!
      \brief Get the name of the observer
      \return observer -- The name of the observer
    */
    inline std::string observer () const {
      return observer_p;
    }
    /*!
      \brief Get the name of the project
      \return project -- The name of the project
    */
    inline std::string project () const {
      return itsProject;
    }
    //! Get the identifier for the observation
    inline std::string observation_id () const {
      return observation_id_p;
    }
    //! Set the telescope observation mode
    inline std::string observation_mode () const {
      return observationMode_p;
    }
    /*!
      \brief Set the name of the telescope
      \param telescope -- The name of the telescope
    */
    inline void setTelescope (std::string const &telescope) {
      telescope_p = telescope;
    }
    /*!
      \brief Set the name of the observer
      \param observer -- The name of the observer
    */
    inline void setObserver (std::string const &observer) {
      observer_p = observer;
    }
    /*!
      \brief Set the name of the project
      \param project -- The name of the project
    */
    inline void setProject (std::string const &project) {
      itsProject = project;
    }
    //! Set the identifier for the observation
    inline void setObservation_id (std::string const &observation_id) {
      observation_id_p = observation_id;
    }
    //! Set the telescope observation mode
    inline void setObservation_mode (std::string const &observation_mode) {
      observationMode_p = observation_mode;
    }
    
    //! Get the time-out before dropping socket connection at connect
    inline void timeoutStart (unsigned int &time_sec,
			      unsigned int &time_usec)
    {
      time_sec  = timeoutStart_p.tv_sec;
      time_usec = timeoutStart_p.tv_usec;
    }
    //! Set the time-out before dropping socket connection at connect
    void setTimeoutStart (double const &timeout=-1.0);
    
    //! Set the time-out before dropping socket connection at connect
    inline void setTimeoutStart (unsigned int const &time_sec,
				 unsigned int const &time_usec)
    {
      timeoutStart_p.tv_sec  = time_sec;
      timeoutStart_p.tv_usec = time_usec;
    }
    
    /*!
      \brief Get the time-out before dropping socket connection while reading
      \retval time_sec  --
      \retval time_usec --
    */
    inline void timeoutRead (unsigned int &time_sec,
			     unsigned int &time_usec)
    {
      time_sec  = timeoutRead_p.tv_sec;
      time_usec = timeoutRead_p.tv_usec;
    }
    
    //! Set the time-out before dropping socket connection while reading
    void setTimeoutRead (double const &timeout=1.0);
    
    //! Set the time-out before dropping socket connection while reading
    inline void setTimeoutRead (unsigned int const &time_sec,
				unsigned int const &time_usec)
    {
      timeoutRead_p.tv_sec  = time_sec;
      timeoutRead_p.tv_usec = time_usec;
    }
    
      //___________________________________________________________________________
      // Methods

      //! Set up the socket connection to the server
      void connectsocket( const char* ipaddress,
                          const char* portnumber);
      //! Open file containing data resulting from a TBB dump
      bool openRawFile( const char* filename );
      bool readRawSocketBlockHeader();
      void readRawFileBlockHeader();
      //! Print the contents of a raw TBB frame header
      void printRawHeader();
      //! Check the CRC of a TBB frame header
      uint16_t CRC16(uint16_t * buffer, uint32_t length);
      bool headerCRC();
      //! Check if the group for a given station exists within the HDF5 file
      void stationCheck();
      void makeOutputHeader();
      //! Create new station group
      void makeNewStation(char *, TBB_Header *);
      //! Create new dipole dataset
      void makeNewDipole (std::string, dalGroup *, TBB_Header *);
      //! Was the data recorded in transients mode
      bool transientMode();
      //! Fix the date
      void fixDate();
      //! Fix the date
      void fixDateNew();
      bool processTransientSocketDataBlock();
      bool processSpectralSocketDataBlock();
      void discardFileBytes(uint bytes);
      void processTransientFileDataBlock();
      void processSpectralFileDataBlock();
      void cleanup();

      //! Check for the end-of-file
      bool eof();

      //! Provide a summary of the internal status
      inline void summary ()
      {
        summary (std::cout);
      }
      //! Provide a summary of the internal status
      void summary (std::ostream &os);

    private:

      //! Initialize internal data
      void init ();
      //! Initialize the values within the TBB_Header struct
      void init_TBB_Header ();

    }; // class TBB

} // DAL namespace


#endif // TBB_H
