#include <iostream>

#include "Data.h"
#include "math.h"

using namespace std;

void edgeDetection(vector< vector <unsigned int> >, int, int, int);


class XYG{
public:
    void setDataMaps(int x, int y, unsigned int X, unsigned int Y, unsigned int Z){
        this->dataMapX[x][y] = X;
        this->dataMapY[x][y] = Y;
        this->dataMapGrad[x][y] = Z;
    }
    void setSize(int s){
        this->size = s;
    }
    vector< vector <unsigned int> > getDataX(){
        return dataMapX;
    }
    vector< vector <unsigned int> > getDataY(){
        return dataMapY;
    }
    vector< vector <unsigned int> > getDataGrad(){
        return dataMapGrad;
    }

private:
    vector< vector <unsigned int> > dataMapX;
    vector< vector <unsigned int> > dataMapY;
    vector< vector <unsigned int> > dataMapGrad;
    int size;


} xyg;

int main() {
    //variables / definition
    cout<<"Beginning Program..."<<endl;
    int height = 3456;
    int width = 5184;
    char choice;
    string path = "/home/pd-laptop/myDocuments/GitRepo/Image_processing_counting_stars/counting_stars.raw";
    /*
    cout<<"Would you like to change the path? (y or n): "<<endl;
    cin>>choice;
    if(choice == 'y' || choice == 'Y'){
        cout<<"Enter the new path (put path + file name.raw): "<<endl;
        cin>>path;
    }
    cout<<"Enter the height: "<<endl;
    cin>>height;
    cout<<"Enter the width: "<<endl;
    cin>>width;
    */


    //create the Image Object
    Data myImage(path);
    unsigned char* buff = myImage.getBuffer();
    int size = (height*width);
    myImage.setSize(size);

    int star_count = 0;

    //load the buffer into a datamap so we can semi-visualize
    vector< vector <unsigned int> > dataMap(width, vector <unsigned int> (height));
    int bufferCounter = 0;
    for(int a = 0; a < height; ++a){
        for(int b = 0; b < width; ++b){
            dataMap[a][b] = (unsigned int)buff[bufferCounter];
            bufferCounter++;
        }
    }



    //do the actual edge detection here
    int filterSize = 3; //3 and 5
    xyg.setSize(size);
    edgeDetection(dataMap,width,height,filterSize);


    //convert back to char *
    unsigned char * edgeBufferX = new unsigned char [size];
    unsigned char * edgeBufferY = new unsigned char [size];
    unsigned char * edgeBufferGrad = new unsigned char [size];
    vector< vector <unsigned int> > vX = xyg.getDataX();
    vector< vector <unsigned int> > vY = xyg.getDataY();
    vector< vector <unsigned int> > vG = xyg.getDataGrad();

    int counter = 0;
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            edgeBufferX[counter] = vX[width][height];
            edgeBufferY[counter] = vY[width][height];
            edgeBufferGrad[counter] = vG[width][height];
            ++counter;
        }
    }


    //output data
    myImage.setSize(size);
    myImage.setBuffer(edgeBufferGrad);
    myImage.writeData("/home/pd-laptop/myDocuments/GitRepo/Image_processing_counting_stars/outputgrad.raw");
    myImage.setBuffer(edgeBufferX);
    myImage.writeData("/home/pd-laptop/myDocuments/GitRepo/Image_processing_counting_stars/outputx.raw");
    myImage.setBuffer(edgeBufferY);
    myImage.writeData("/home/pd-laptop/myDocuments/GitRepo/Image_processing_counting_stars/outputy.raw");
    cout<<"finished writing..."<<endl;

    delete [] buff;

    //cin.get();
    return 0;
}


void edgeDetection(vector< vector <unsigned int> > dataMap, int width, int height, int filterSize){
    //define the matrices
    unsigned int GX;
    unsigned int GY;
    unsigned int Gradient;

    int filter[filterSize*filterSize];

    if(filterSize == 3){
        int filter_ [] = {1,0,-1,2,0,-2,1,0,-1};
        for(int i = 0; i  < filterSize*filterSize; ++i)
            filter[i] = filter_[i];

    }
    else if(filterSize == 5){
        int filter_[] = {1,2,0,-2,-1,4,8,0,-8,-4,6,12,0,-12,-6,4,8,0,-8,-4,1,2,0,-2,-1};
        for(int i = 0; i  < filterSize*filterSize; ++i)
            filter[i] = filter_[i];

    }
    else {
        cout << "Invalid filter size!!!";
        return;
    }

    //do calulations here
    for(int i = 0; i  < width; ++i){
        for(int j = 0; j < height; ++j){
            if(i != 0 && j != 0 && i != width && j != height) {
                if(filterSize == 3) {
                    for (int z = -1; z < 1; ++z) {
                        for (int w = -1; w < 1; ++w) {
                            GX += dataMap[i][j];
                            GY += dataMap[i][j];
                            cout<<z<<" ";
                        }
                    }
                }

            }
           // Gradient = (unsigned int)sqrt((double)GX*(double)GX + (double)GY*(double)GY);
            xyg.setDataMaps(i,j,GX,GY,Gradient);

            GX = 0;
            GY = 0;
            Gradient = 0;
        }
    }

    //if edge point
    //xyg.setDataMaps(x,y,X,Y,Z);

}
/*
 * (1) Computing differentiation by difference - using an edge operator such as Sobel Operator in x and y directions.
 * Display zero difference value in gray, e.g., I(x,y)=128,
 * and other positive and negative values scaled and shifted accordingly in the range of [0, 255].
 * Compute these horizontal and vertical differential images separately. For 5*5 size filter, the differential operator (filter)
 * can be designed as the weighted rows or columns of -1, -2, 0, 2, 1. These differential vectors can be further stacked
 * with weights 1, 2, 4, 2, 1 for 5*5 filter. Pay attention to positive edges and negative edges and possible overflow and underflow
 * in the filtering results. In the filtering, you will select certain constants to divide the results of convolutions in order to
 * prevent overflow of the filtered values.
(2) Computing a gradient image from the horizontal and vertical differential images.
 Display both results for 3*3 and 5*5 filters.
(3) Detecting edges at peaks of gradient values over a threshold.
 The threshold can be selected manually so that the edge results show good shape of scenes.
 Edge must be one pixel wide and marked in a bright intensity.
 */