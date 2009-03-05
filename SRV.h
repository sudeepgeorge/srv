/* For SITPL - Internal Only */
#ifndef _SRV_H
#define _SRV_H

#include <fstream>



const unsigned int SRV_PORT= 10001;
const char* const SRV_IP  = "172.16.0.156";


const unsigned char DEBUG=1;

class SRV
{
    public:

    /**************************************************************************
     Constructor: Does the basic initialisation for the private stuff.
    **************************************************************************/
    SRV();


    /**************************************************************************
     Destructor: Cleans up the private stuff
    **************************************************************************/
    ~SRV();

    /**************************************************************************
    open(): Opens up a TCP connection to the SRV-1
    **************************************************************************/
    bool open();


    /**************************************************************************
     close(): Closes the TCP connection to the SRV-1
    **************************************************************************/
    bool close();


    /**************************************************************************
     read(): Reads as much as it can upto datalen into the buffer 'data'.
             No guarantee that the requested amount will be read
    **************************************************************************/
    bool read(char* data,int datalen);


     /**************************************************************************
     readlen(): Reads upto datalen into the buffer 'data'.
                Guarantees that the requested amount will be read
                or will die trying. 
    **************************************************************************/
    bool readlen(char* data,int datalen);


    /**************************************************************************
    write(): Writes out a string to the SRV-1
    **************************************************************************/
    bool write(const char *data);

    /**************************************************************************
    connected(): Informs if we are connected to the SRV-1
    **************************************************************************/
    inline bool connected() {return is_connected;}


    private:
    int srv_sock;
    bool is_connected;
};

/**************************************************************************
readImage(): Reads an Image from the SRV-1 and saves it to the file.
             Make sure *you* close the file.
**************************************************************************/
bool readImage(std::ofstream& img_file);

#endif //_SRV_H