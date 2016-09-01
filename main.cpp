#include <vector>
#include <stddef.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

class MapObject{
public:
    MapObject() :
            data_(),
            size_(0),
            width_(0),
            height_(0)
        {}
    MapObject(size_t size, size_t width ,size_t height) :
            data_(),
            size_(size),
            width_(width),
            height_(height)
        {
            this->set_vector_size();
        }

    MapObject(const MapObject& item) :
            data_(item.data_),
            size_(item.size_),
            width_(item.width_),
            height_(item.height_)
        {}

    ~MapObject(){
        this->data_.clear();
    }

    void set_vector_size(){
        this->data_.resize(this->width_+1, vector <unsigned int>(this->height_+1));
    }

    void set_vector_element(size_t x, size_t y, unsigned int element){
        this->data_[x][y] = element;
    }

    unsigned int get_vector_element(size_t x, size_t y){
        return this->data_[x][y];
    }

    unsigned char * return_a_buffer(){
        unsigned char * buff = new unsigned char[this->size_];

        int count = 0;
        for(int i = 0; i < this->width_; ++i){
            for(int j = 0; j < this->height_; ++j){
                buff[count] = this->data_[i][j];
            }
        }
        return buff;
    }
private:
    vector < vector <unsigned int > > data_;
    size_t size_;
    size_t width_;
    size_t height_;
};

class BufferObject{
public:
    BufferObject(void) :
            file_name_(),
            buffer_(nullptr),
            size_(0)
        {
            file_name_ = getFileName();
            this->buffer_ = dataBuffer();
        }

    BufferObject(BufferObject &img):
            file_name_(),
            buffer_(nullptr),
            size_(0)
        {
            setBuffer(buffer_);
        }

    BufferObject(std::string fName) :
            file_name_(),
            buffer_(nullptr),
            size_(0)
        {
            this->file_name_ = fName;
            this->buffer_ = dataBuffer();
        }

    ~BufferObject(void){
        if(this->buffer_ != nullptr)
            delete this->buffer_;
    }

    std::string getFileName(){
        std::string fileName = "";
        std::cout<<" Please enter a file name: ";
        std::cin>>fileName;
        return fileName;
    }

    unsigned char * dataBuffer(){
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

    size_t getSize() const{
        return this->size_;
    }

    void setSize(size_t size){
        this->size_ = size;
    }

    unsigned char * getBuffer() const{
        return this->buffer_;
    }

    void setBuffer(unsigned char* buff){
        unsigned char* temp = buff;
        unsigned char* newBuffer = new  unsigned char [this->size_];

        for(size_t i = 0; i < getSize(); ++i){
            newBuffer[i] = temp[i];
        }
        this->buffer_ = newBuffer;
        delete [] temp;
    }

    void writeData(std::string location){
        std::ofstream outputFile;
        unsigned char * data = this->getBuffer();
        size_t size = this->getSize();

        outputFile.open(location, std::ios::binary);

        for(size_t i = 0; i < size; ++i){
            outputFile.put(data[i]);
        }

        outputFile.close();
    }

private:
    string file_name_;
    unsigned char * buffer_;
    size_t size_;
};

int main() {
    cout<<"Beginning program..."<<endl;

    size_t width = 500;
    size_t height = 500;
    size_t size = width * height;
    int filterSize = 3;
    int offset = 1;
    string path = "/home/pd-laptop/myDocuments/GitRepo/EdgeDetection/Input.raw";

    //get any necessary user input
    //

    MapObject dataMap(size,width,height);
    MapObject x_filter(size,width,height);
    MapObject y_filter(size,width,height);
    MapObject gradient_filter(size,width,height);

    for(size_t x = 0; x < width; ++x){
        for(size_t y = 0; y < height; ++y){
            x_filter.set_vector_element(x,y,0);
            y_filter.set_vector_element(x,y,0);
            gradient_filter.set_vector_element(x,y,0);
        }
    }

    //input data
    BufferObject b(path);
    unsigned char* buffer = b.getBuffer();
    b.setSize(size);

    //change to 2d vector
    size_t bufferCounter = 0;
    for(size_t i = 0; i < width; ++i){
        for(size_t j = 0; j < height; ++j){
            dataMap.set_vector_element(i,j,(unsigned int)buffer[bufferCounter]);
            ++bufferCounter;
        }
    }

    double GX [filterSize][filterSize];
    double GY [filterSize][filterSize];
    double xVal, yVal;
    unsigned int gVal;

    //compute the convolutions x, y
    switch (filterSize) {
        case 3 :
            GX[0][0] = 1; GX[0][1] = 0; GX[0][2] = -1;
            GX[1][0] = 2; GX[1][1] = 0; GX[1][2] = -2;
            GX[2][0] = 1; GX[2][1] = 0; GX[2][2] = -1;

            GY[0][0] = 1; GY[0][1] = 2; GY[0][2] = 1;
            GY[1][0] = 0; GY[1][1] = 0; GY[1][2] = 0;
            GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

            offset = 1;
        case 5 :

            offset = 2;
        default :
            cout<<"Not a valid size...Program will not run correctly"<<endl;
    }

    for(int i = 0; i < width; ++i){
        for(int j = 0; j < height; ++j){
            if(i != 0 && j != 0 && i != width-1 && j != height-1) {
                //compute convolution
                for(int x = 0; x < filterSize; ++x){
                    for(int y = 0; y < filterSize; ++y){
                        xVal += GX[x][y] * (double)x_filter.get_vector_element(width-offset+x,height-offset+y);
                        yVal += GY[x][y] * (double)y_filter.get_vector_element(width-offset+x,height-offset+y);
                    }
                }

                gVal = (unsigned int)sqrt(xVal*xVal + yVal*yVal);

                //set each pixel
                x_filter.set_vector_element(width, height, (unsigned int) xVal);
                y_filter.set_vector_element(width,height,(unsigned int)yVal);
                gradient_filter.set_vector_element(width,height,gVal);

                //reset the values to 0
                xVal = 0;
                yVal = 0;
                gVal = 0;
            }
        }
    }

    //output x image
    buffer = x_filter.return_a_buffer();
    b.setBuffer(buffer);
    b.writeData("/home/pd-laptop/myDocuments/GitRepo/EdgeDetection/outputx.raw");

    //output y image
    buffer = y_filter.return_a_buffer();
    b.setBuffer(buffer);
    b.writeData("/home/pd-laptop/myDocuments/GitRepo/EdgeDetection/outputy.raw");

    //output gradient image
    buffer = gradient_filter.return_a_buffer();
    b.setBuffer(buffer);
    b.writeData("/home/pd-laptop/myDocuments/GitRepo/EdgeDetection/outputgrad.raw");

    cout<<"Ending program..."<<endl;
    return 0;
}