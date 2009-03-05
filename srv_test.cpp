/* For SITPL - Internal Only */
#include "SRV.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    #if 0
    SRV srv;    
    srv.open();
   
    // Change the resolution of the SRV-1 camera to 640x480
    if(srv.connected())
    {
        srv.write("c");
        char *resol = new char[10];
        if(srv.read(resol,10))
           cout << resol << endl;
        
        delete [] resol;
        
        srv.close();
        
    }
    else
    {
        cout << "Failed to change the resolution"<<endl;
    }
    
    sleep(10); //Hold on - though the time can be reduced.
    #endif

    //Get the image.
    ofstream img_file;
    img_file.open("Img.jpg",ios::out|ios::binary);

    if(img_file.is_open())
    {
        if(!readImage(img_file))
            cout << "Failed to test Image Read" << endl;

        img_file.close();
    }   
    else
    {
        cout << "Failed to create the Image File" <<endl;
    }

    return 0;

}
