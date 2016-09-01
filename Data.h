//
// Created by pd-laptop on 2/16/16.
//

#ifndef DATA_H
#define DATA_H

#include <vector>
#include <stddef.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class Data {
public:
    /*
     * The default constructor, it prompts the user for the input image.
     */
    Data(void);

    /*
     * The copy constructor
     */
    Data(Data &img);

    /*
     * Destructor
     */
    ~Data(void);

    /*
     * File name constructor, create a construct and inputs the image given.
     */
    Data(std::string);

    /*
     * This method gets the file name from the user.
     */
    static std::string getFileName();

    /*
    * This method gets the data of the raw image and imports it into a long buffer.
     * It reads the data as unsigned character bytes.
    */
    unsigned char * dataBuffer();

    /*
     * Saves the image to a file in the location and file type specified.
     * It writes the data as unsigned character bytes.
     */
    void writeData(std::string);

    /*
     * Getter / setter size of buffer
     */
    size_t getSize() const;
    void setSize(size_t);

    /*
     * Getter / setter buffer
     */
    unsigned char * getBuffer() const;
    void setBuffer(unsigned char *);



private:

};

#include "Data.cpp"
#endif //IMAGE_PROCESSING_COUNTING_STARS_IMAGE_H
