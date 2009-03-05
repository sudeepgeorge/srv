/* For SITPL - Internal Only */

#include "SRV.h"
#include <iostream>
#include <cstring>
#include <cerrno>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>



using namespace std;

/**************************************************************************/
SRV::SRV()
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;
           
    is_connected=false;
    srv_sock=0;
}

/**************************************************************************/
SRV::~SRV()
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;
    
    if(is_connected)
        cout<<"WARNING: Exiting without calling SRV::close()"<<endl;
}

/**************************************************************************/
bool SRV::open()
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;

    if((srv_sock = socket(PF_INET,SOCK_STREAM,0))<0)
    {
        cout<<strerror(errno)<<endl;
        return false;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SRV_PORT);

    int inet_ret=inet_pton(AF_INET,SRV_IP,&addr.sin_addr.s_addr);

    if(0==inet_ret)
    {
        cout << "SRV_IP is not in IP format"<<endl;
        return false;
    }
    else if (inet_ret<0)
    {
        cout<<strerror(errno)<<endl;
        return false;
    }    
    


    if(connect(srv_sock,(const struct sockaddr *)&addr,(socklen_t)sizeof(addr))<0)
    {
        cout<<strerror(errno)<<endl;
        return false;
    }
    else
    {
        if(DEBUG)
            cout<< "Connected to SRV"<<endl;
        is_connected=true;
    }

    return true;
}

/**************************************************************************/
bool SRV::close()
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;

    if(is_connected)
    {
        if( shutdown(srv_sock,SHUT_RDWR)<0)
        {
            cout<<strerror(errno)<<endl;
            return false;
        }
        
        is_connected=false;
        
    }
    return true;
}

/**************************************************************************/
bool SRV::write(const char *data)
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;
    
    if(!is_connected)
    {
        cout<<"Please call SRV::open()"<<endl;
        return false;
    }
    
    if(NULL==data)
    {
        cout << "Write command is NULL"<<endl;
        return false;
    }
    
    int send_ret=send(srv_sock,data,(size_t)sizeof(data),0);
    
    if(send_ret!=sizeof(data))
    {
            cout << "Full message was not transmitted"<<endl;
            return false;
    }
    
    if(send_ret<0)
    {
        cout<<strerror(errno)<<endl;
        return false;
    }

    return true;
}

/**************************************************************************/
bool SRV::read(char* data,int datalen)
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;
    
    if(!is_connected)
    {
        cout<<"Please call SRV::open()"<<endl;
        return false;
    }
    
    if(NULL==data)
    {
        cout << "Read buffer is NULL"<<endl;
        return false;
    }
    
    int recv_ret=recv(srv_sock,(void *)data,(size_t)datalen,0);
    
    if(0==recv_ret)
    {
        cout <<"The peer has shutdown"<<endl;
        return false;
    }
    
    if(recv_ret<0)
    {
        cout << strerror(errno)<<endl;
        return false;
    }

    return true;
}

/**************************************************************************/
bool SRV::readlen(char* data,int datalen)
{
    if(DEBUG)
        cout << __PRETTY_FUNCTION__ <<endl;
    
    if(!is_connected)
    {
        cout<<"Please call SRV::open()"<<endl;
        return false;
    }
    
    if(NULL==data)
    {
        cout << "Read buffer is NULL"<<endl;
        return false;
    }
    
    int recv_ret=0;
    int recv_now=0;
    
    do
    {
        recv_now=recv(srv_sock,(void *)(data+recv_ret),(size_t)(datalen-recv_ret),0);
	recv_ret+=recv_now;
	cout << "Recv: "<<recv_now<<"Expecting: "<<(datalen-recv_ret)<<endl;
        
        if(DEBUG)
            cout << recv_ret<<endl;
        
        
        
        if(0==recv_ret)
        {
            cout <<"The peer has shutdown"<<endl;
            return false;
        }
        
        if(recv_ret<0)
        {
            cout << strerror(errno)<<endl;
            return false;
        }
	
    }while(recv_ret<datalen);
    
    return true;
}

/**************************************************************************/
bool readImage(std::ofstream& img_file)
{
    SRV srv;
    bool retval = true;
    
    if(!srv.open())
    {
        cout << "Failed to open the connection"<<endl;
        return false;
    }
    
    if(!srv.write("I"))
    {
        cout << "Failed to write the command" << endl;
        return false;
    }
    
    // Read the Header to get the filesize. 
    //Header format is ##IMJ[1 Byte describing Image Resolution ][size(2Bytes)][2 Bytes of filler]
    char *header_data = new char[10];
    if(!srv.readlen(header_data,10))
    {
        if(DEBUG)
            cout << "Failed to read the header"<<endl;
        retval = false;
    }
    
    if( header_data[0]!='#' ||\
        header_data[1]!='#' ||\
        header_data[2]!='I' ||\
        header_data[3]!='M' ||\
        header_data[4]!='J' ||\
        !retval )
    {
        if(DEBUG)
            cout << "Invalid Header" <<endl;
        
        delete [] header_data;
        retval = false;
    }
    else
    {
        unsigned int len=0;

	//cout <<" Len is: " << hex << ((((header_data[6]<<8) & 0xFF00)|(header_data[7]& 0x00FF)))<<endl;

        len=ntohs((((header_data[6]<<8) & 0xFF00)|(header_data[7]& 0x00FF))); //Handle Endianess of n/w
	
        
        if(DEBUG)
            cout<<"Size of Image is: "<<dec<<len<<endl;
        
        char *img = new char[len];
        if(!srv.readlen(img,len))
        {
            if(DEBUG)
                cout << "Failed to read the Image Data" <<endl;
            retval = false;
        }
        
        
        if(img_file.is_open() && retval)
        {
            img_file.write(img,len);

        }
        else
        {
            retval=false;
        }
        
        delete [] img;
        delete [] header_data;
        
    }
    
    srv.close();
    
    
    return retval;
    
}

/**************************************************************************/
