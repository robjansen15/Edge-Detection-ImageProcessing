//
// Created by pd-laptop on 2/16/16.
//

#include "Data.h"

Data::Data(void)
        :   file_name_(),
            buffer_(nullptr),
            size_(0)
{
    file_name_ = getFileName();
    this->buffer_ = dataBuffer();
}

Data::Data(Data &img)
        :   file_name_(),
            buffer_(nullptr),
            size_(0)
{
    setBuffer(buffer_);
}

Data::Data(std::string fName)
        :   file_name_(),
            buffer_(nullptr),
            size_(0)
{
    this->file_name_ = fName;
    this->buffer_ = dataBuffer();
}

Data::~Data(void){
    if(this->buffer_ != nullptr)
        delete this->buffer_;
}


std::string Data::getFileName(){
    std::string fileName = "";
    std::cout<<" Please enter a file name: ";
    std::cin>>fileName;
    return fileName;
}

unsigned char * Data::dataBuffer(){
    std::ifstream inputFile;
    unsigned char * data;
    size_t count = 0;

    inputFile.open(this->file_name_, std::ios::binary);
    inputFile.seekg (0, inputFile.end);
    size_t length = inputFile.tellg();
    inputFile.seekg (0, inputFile.beg);
    setSize(length);
    unsigned char * buffer = new  unsigned char [length];

    inputFile >> buffer;
    inputFile.close();

    return buffer;
}

size_t Data::getSize() const{
    return this->size_;
}

void Data::setSize(size_t size){
    this->size_ = size;
}

unsigned char * Data::getBuffer() const{
    return this->buffer_;
}

void Data::setBuffer(unsigned char* buff){
    unsigned char* temp = buff;
    unsigned char* newBuffer = new  unsigned char [this->size_];

    for(size_t i = 0; i < getSize(); ++i){
        newBuffer[i] = temp[i];
    }
    this->buffer_ = newBuffer;
    delete [] temp;
}

void Data::writeData(std::string location){
    std::ofstream outputFile;
    unsigned char * data = this->getBuffer();
    size_t size = this->getSize();

    outputFile.open(location, std::ios::binary);

    for(size_t i = 0; i < size; ++i){
        outputFile.put(data[i]);
    }

    outputFile.close();
}



