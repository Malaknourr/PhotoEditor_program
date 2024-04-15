/*
File Name : CS112_A3_part2B_S19_20230052_20230419_20230137.cpp
Authors :
(1) Malak Nour-Elhussien Samir , ID: 20230419 , S19 , Filter taken : 3 , 6 , 9 , 12 & 16 for bouns.
 email : nourrm218@gmail.com

(2) Esraa Hussien Mohamed , ID : 20230052  ,  S19  ,  Filter taken : 1 , 4 , 7 , 10 & 17 for bouns.
email : ehussien143@gmail.com

(3) Rahma salah Eldin Mahmoud ,ID : 20230137 , S19 , Filter taken :  2 , 5 , 8 , 11 & 13 for bouns.
 email : rahmasalah072@gmail.com

Date : 15 April 2024.

Drive link for System Diagram : https://drive.google.com/file/d/1I-woxuw5vkQH3SWrb0a-zMObIaM2ASXs/view?usp=sharing
*/

#include "Image_Class.h" //the new version
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace std;

// Function to check if the file extension is valid
bool Valid_Extension(const string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos && dotPos != filename.length() - 1) {
        string extension = filename.substr(dotPos + 1);
        const string supportedExtensions[] = { "jpg", "jpeg", "png", "bmp", "tga" };
        for (const string& ext : supportedExtensions) {
            if (extension == ext) {
                return true; // Extension is valid
            }
        }
    }
    return false; // Extension is not valid
}

// Function to get a valid filename from the user
string validfilename_() {
    string filename;
    while (true) {
        cout <<"Please insert the filename of your photo: "<<endl;
        cin >> filename;
        if (!Valid_Extension(filename)) {
            cerr << "Unsupported file format. Please provide a filename with a supported extension (jpg, jpeg, png, bmp, tga)." << endl;
        } else {
            try {
                // Load the photo into the Image object
                Image original(filename);
                if (original.imageData == nullptr) {
                    cerr << "Failed to load the image! Please check the filename." << endl;
                    continue; // Prompt for input again
                }
                return filename;
            } catch (const invalid_argument& e) {
                cerr << e.what() << endl; // Print the error message
                cerr << "Please provide a valid filename." << endl;
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
            }
        }
    }
}

// Function to validate menu choices
int Validmenu_Choice(int menu_size) {
    int choice;
    while (true) {
        cout << "Enter your choice: ";
        if (!(cin >> choice) || choice < 1 || choice > menu_size) {
            cout << "Invalid input. Please enter a valid number between 1 and " << menu_size << "." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the input buffer
        } else {
            break;
        }
    }
    return choice;
}

//filter 1

// Function to apply Filter 1: Convert image to grayscale
void applyGrayscaleFilter (Image& image) {
    Image result = image; // Create a copy of the original image
    // Image image(filename);
    int width = image.width;
    int height = image.height;
    int channels = image.channels;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            unsigned int avg = 0;
            for (int k = 0; k < channels; k++) {
                avg += image(i, j, k);
            }
            avg /= channels;
            for (int k = 0; k < channels; k++) {
                image(i, j, k) = avg;
            }
        }
    }

}

//filter 2

// Function to convert colored image to black and white
void convertToBlackAndWhite(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0; // Initialize average value

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k); // Accumulate pixel values
            }

            unsigned int gray = 128; // gray for binary conversion

            avg /= 3; // Calculate average

            if (avg < gray) {
                // Set pixel to black
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            } else {
                // Set pixel to white
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
        }
    }
}

//filter 3

void Inverted_Image(Image& image) {
    Image result = image;
    for (int i = 0; i < image.width; i++) {
        for (int n = 0; n < image.height; n++) {
            for (int k = 0; k < 3; k++) {
                result.setPixel(i, n, k, 255 - image.getPixel(i, n, k));
            }
        }
    }
    image = result;
}

//filter 4

Image Enlarge_To(Image &originalImage, int w, int h) {
    float dw = (1.0*w)/originalImage.width;
    float dh = (1.0*h)/originalImage.height;
    int io = 0,jo = 0;
    Image new_img(w,h);
    for (int i = 0; i < w; ++i) {
        jo = 0;
        if( i > io*dw ) io++;
        for (int j = 0; j < h; ++j) {
            while( j > jo*dh ) jo++;
            for (int k = 0; k < 3; ++k)
            {
                new_img(i, j, k) = originalImage(min(io,originalImage.width-1), min(jo,originalImage.height-1) , k);
            }
        }
    }
    return new_img;
}

// Function to apply Filter 4: Blend two images
void blendImages(Image& toy1, Image& toy2 ) {
    int choice3;
    while(true) {
        cout << "1) Resize smallest image to largest image and merge"<< endl;
        cout << "2) Merge common area"<< endl;
        cout << "enter your choice : "<<endl;
        cin >> choice3;

        if (choice3 == 1) {
            toy1 = Enlarge_To(toy1, max(toy1.width, toy2.width), max(toy1.height, toy2.height));
            toy2 = Enlarge_To(toy2, max(toy1.width, toy2.width), max(toy1.height, toy2.height));
            int maxWidth = max(toy1.width, toy2.width);
            int maxHeight = max(toy1.height, toy2.height);
            Image img2(maxWidth, maxHeight);

            for (int i = 0; i < maxWidth; i++) {
                for (int j = 0; j < maxHeight; j++) {
                    for (int k = 0; k < img2.channels; k++) {
                        img2(i, j, k) = (toy1(i, j, k) + toy2(i, j, k)) / 2;
                        if (img2(i, j, k) > 255) {
                            img2(i, j, k) = 255;
                        }

                    }
                }
            }
            toy1 = img2;
            break;
        }
        else if (choice3 == 2) {
            int minWidth = min(toy1.width, toy2.width);
            int minHeight = min(toy1.height, toy2.height);
            Image img2(minWidth, minHeight);
            for (int i = 0; i < minWidth; i++) {
                for (int j = 0; j < minHeight; j++) {
                    for (int k = 0; k < img2.channels; k++) {
                        img2(i, j, k) = (toy1(i, j, k) + toy2(i, j, k)) / 2;
                        if (img2(i, j, k) > 255) {
                            img2(i, j, k) = 255;
                        }

                    }
                }
            }
            toy1 = img2;
            break;
        }
        else {
            cout << "INVALID CHOICE !" << endl;
        }
    }
}

//filter 5

// Function to flip the image horizontally
void flipHorizontally(Image& image) {
    for (int i = 0; i < image.width / 2; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                swap(image(i, j, k), image(image.width - i - 1, j, k));
            }
        }
    }
}
// Function to flip the image vertically
void flipVertically(Image& image) {
    for (int j = 0; j < image.height / 2; ++j) {
        for (int i = 0; i < image.width; ++i) {
            for (int k = 0; k < 3; ++k) {
                swap(image(i, j, k), image(i, image.height - j - 1, k));
            }
        }
    }
}
//flip menu
void flip_menu(Image& original) {
    while (true) {
        cout << "Choose a flip :" << endl;
        cout << "1) Vertically" << endl;
        cout << "2) Flip horizontally" << endl;

        int choice = Validmenu_Choice(2); // Accepts numbers 1 and 2 only

        if (choice == 1) {
            flipVertically(original);
            original.saveImage("arrowVertically_image.jpg");
            break;
        } else if (choice == 2) {
            flipHorizontally(original);
            original.saveImage("arrowhorizontally_image.jpg");
            break;
        }
        else {
            cout << "INVALID CHOICE !" << endl;
        }
    }
}


//filter 6

void Rotate_270(Image& original) {
    // Create a new image and swapped for dimentioals 90-degree clockwise rotation
    Image rotated(original.height, original.width);

    // Iterate over each pixel in the original image
    for (int y = 0; y < original.height; ++y) {
        for (int x = 0; x < original.width; ++x) {
            // Assign pixel value from original image to rotated image with swapped coordinates
            for (int c = 0; c < original.channels; ++c) {
                rotated(y, original.width - 1 - x, c) = original(x, y, c);
            }
        }
    }
    original = rotated;
}

void Rotate_90(Image& original) {
    // Create a new image with swapped dimensions for 90-degree rotation
    Image rotated(original.height, original.width);

    // Iterate over each pixel in the original image
    for (int j = 0; j < original.height; ++j) {
        for (int i = 0; i < original.width; ++i) {
            for (int k = 0; k < 3; k++) {
                // Rotate the image 90 degrees clockwise
                rotated(original.height - 1 - j, i, k) = original(i, j, k);
            }
        }
    }
    original = rotated;
}


void Rotate_180(Image& original) {
    // Create a new image with the same dimensions for the rotated image
    Image rotated(original.width, original.height);

    // Iterate over each pixel in the original image
    for (int i = 0; i < rotated.height; ++i) {
        for (int j = 0; j < rotated.width; ++j) {
            for (int k = 0; k < 3; k++) {
                // Rotate the image 180 degrees clockwise
                rotated(j, i, k) = original(original.width - 1 - j, original.height - 1 - i, k);
            }
        }
    }
    original = rotated;
}

// filter 7   Dark
void Darken_Image(Image& image) {
    int width = image.width;
    int height = image.height;
    int channels = image.channels;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < channels; k++) {
                image (i,j,k)= (0.5)*image(i,j,k);
                if (image(i, j, k) > 255) {
                    image(i, j, k) = 255;
                }
            }
        }
    }

}

// filter 7  light
void lighten_Image(Image & image){
    int width = image.width;
    int height = image.height;
    int channels = image.channels;
    float factor = 1.5;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < channels; k++) {
                image(i, j, k) = min(255, (int)(image(i, j, k) * factor));
            }
        }
    }
}

// filter 8

// Function to crop the image
void cropImage(Image& image) {

    int x, y, width, height;
    // Prompt user for crop coordinates and dimensions
    cout << "Enter the coordinates of the crop area (x and y): ";
    cin >> x >> y;
    cout << "Enter the dimensions of the crop area (width and height): ";
    cin >> width >> height;


    // Create a new image with the specified dimensions
    Image croppedImage(width, height);

    // Copy the cropped region from the original image to the new one
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < 3; ++k) {
                croppedImage(i, j, k) = image(x + i, y + j, k);
            }
        }
    }
    image = croppedImage;
}

// filter 9

void simple_frame(Image& photo , int R , int G , int B) {

    //multiplying by 0.01 to make the thickness of the frame flexible with the photos' dimensions.

    for (int i = 0; i < photo.width * 0.01 ; ++i) {  //  left side
        for (int j = 0; j < photo.height; ++j) {
            photo(i, j, 0) = R;
            photo(i, j, 1) = G;
            photo(i, j, 2) = B;
        }
    }

    int width = photo.width; // Get the width of the image
    for (int i = 0; i < photo.width * 0.01 ; ++i) {  // right side
        for (int j = 0; j < photo.height; ++j) {
            photo(width - 1 - i, j, 0) = R;
            photo(width - 1 - i, j, 1) = G;
            photo(width - 1 - i, j, 2) = B;
        }
    }


    for(int i = 0 ; i < photo.width ; ++i){ //top
        for(int j = 0 ; j < photo.height * 0.01 ; ++j ){
            photo(i, j, 0) = R;
            photo(i, j, 1) = G;
            photo(i, j, 2) = B;
        }
    }
    //bottom
    int lastrow = photo.height;
    for(int i = 0 ; i < photo.width ; ++i){
        for(int j = 0 ; j < photo.height * 0.01 ; ++j ){
            photo(i, lastrow - 1 - j , 0) = R;
            photo(i, lastrow - 1 - j, 1) = G;
            photo(i, lastrow - 1 - j, 2) = B;
        }
    }
}


void fancy_frame(Image& photo , int R , int G , int B) {

    simple_frame(photo,R,G,B);

    //adding white fancy frame

    // Adding fancy frame for left side
    for (int i = photo.width * 0.01 + 13 ; i < photo.width * 0.01 + 19 ; ++i) {
        for (int j = 0; j < photo.height; ++j) {
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, j, k) = 255;
            }
        }
    }

    for (int i = photo.width * 0.01 + 20 ; i < photo.width * 0.01 + 26 ; ++i) {
        for (int j = 0; j < photo.height; ++j) {
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, j, k) = 255;
            }
        }
    }

    int width = photo.width; // Get the width of the image
    // Adding fancy frame for right side
    for (int i = photo.width * 0.01 + 13 ; i < photo.width * 0.01 + 19 ; ++i) {
        for (int j = 0; j < photo.height; ++j) {
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(width - 1 - i, j, k) = 255;
            }
        }
    }
    for (int i = photo.width * 0.01 + 20 ; i < photo.width * 0.01 + 26 ; ++i) {
        for (int j = 0; j < photo.height; ++j) {
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(width - 1 - i, j, k) = 255;
            }
        }
    }

    //adding extra frame for top
    for(int i = 0 ; i < photo.width ; ++i){
        for(int j = photo.height * 0.01 + 13   ; j < photo.height * 0.01  + 19 ; ++j ){
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, j, k) = 255;
            }
        }
    }
    for(int i = 0 ; i < photo.width ; ++i){
        for(int j = photo.height * 0.01 + 20  ; j < photo.height * 0.01 + 26 ; ++j ){
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, j, k) = 255;
            }
        }
    }

    int lastrow = photo.height;
    //adding extra frame for top
    for(int i = 0 ; i < photo.width ; ++i){
        for(int j = photo.height * 0.01 + 13  ; j < photo.height * 0.01 + 19 ; ++j ){
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, lastrow - 1 - j, k) = 255;
            }
        }
    }
    for(int i = 0 ; i < photo.width ; ++i){
        for(int j = photo.height * 0.01 + 20  ; j < photo.height * 0.01 + 26 ; ++j ){
            for(int k = 0 ; k < 3 ; ++k ) {
                photo(i, lastrow - 1 - j, k) = 255;
            }
        }
    }


    // Save the modified image
    photo.saveImage("fancy-frame.jpg");
}

// menu
void frame_menu(Image& photo) {
    while (true) {
        cout << "Choose a frame :" << endl;
        cout << "1)Simple" << endl;
        cout << "2)Fancy" << endl;

        int choice = Validmenu_Choice(2); // Accepts numbers 1 and 2 only
        if (choice == 1) {
            cout << "Choose a color for your frame" << endl;
            cout << "1)Red" << endl;
            cout << "2)Green" << endl;
            cout << "3)Blue" << endl;

            int choice2 = Validmenu_Choice(3);
            if (choice2 == 1){
                simple_frame(photo , 225 , 0 , 0);
            }
            else if (choice2 == 2){
                simple_frame(photo , 0 , 225 , 0);
            }
            else if(choice2 == 3){
                simple_frame(photo , 0 , 0 , 255);
            }
            break;
        }
        else if (choice == 2) {
            cout << "Choose a color for your frame" << endl;
            cout << "1)Red" << endl;
            cout << "2)Green" << endl;
            cout << "3)Blue" << endl;

            int choice2 = Validmenu_Choice(3);
            if (choice2 == 1){
                fancy_frame(photo , 225 , 0 , 0);
            }
            else if (choice2 == 2){
                fancy_frame(photo , 0 , 225 , 0);
            }
            else if(choice2 == 3){
                fancy_frame(photo , 0 , 0 , 255);
            }
            break;
        }
        else {
            cout << "INVALID CHOICE !" << endl;
        }
    }

}

// filter 10
void detectEdgesSobel(Image &image) {
    // To black and white
    convertToBlackAndWhite(image);
    int boarder = max(image.width, image.height) * 0.5 / 100;
    for (int i = 0; i < image.width-1; i++){
        for (int j = 0; j < image.height-1; j++){
            if (image(i, j, 0) != image(i, j + 1, 0) || image(i + 1, j, 0) != image(i, j + 1, 0)  ){
                for (int new_j =  j; new_j < min(boarder + j,image.height); new_j++){
                    for (int rgb = 0; rgb < 3; rgb++){
                        image(i, new_j, rgb) = 0;
                    }
                }
                j = j + boarder;
                j--;
            }else{
                for (int rgb = 0; rgb < 3; rgb++){
                    image(i, j, rgb) = 255;
                }
            }
        }
    }
}

// filter 11
void resizeImage(Image& image) {

    // Prompt user for new dimensions
    int newWidth, newHeight;
    cout << "Enter the new width: ";
    cin >> newWidth;
    cout << "Enter the new height: ";
    cin >> newHeight;

    float scaleX = static_cast<float>(newWidth) / image.width;
    float scaleY = static_cast<float>(newHeight) / image.height;
    Image resizedImage(newWidth, newHeight);
    for (int i = 0; i < newWidth; ++i) {
        for (int j = 0; j < newHeight; ++j) {
            int origX = static_cast<int>(i / scaleX);
            int origY = static_cast<int>(j / scaleY);

            for (int k = 0; k < 3; ++k) {
                resizedImage(i, j, k) = image(origX, origY, k);
            }
        }
    }
    image = resizedImage;

}
//filter 12

// Function to calculate prefix sum for a given color channel.
void calculatePrefixSum(Image& photo,vector<vector<int>>& prefixSumR,
                        vector<vector<int>>& prefixSumG,
                        vector<vector<int>>& prefixSumB) {
    int width = photo.width;
    int height = photo.height;
    //
    prefixSumR.assign(width, vector<int>(height, 0));
    prefixSumG.assign(width, vector<int>(height, 0));
    prefixSumB.assign(width, vector<int>(height, 0));

    // Calculate the prefix sum for each color channel
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // Access pixel values for each channel
            prefixSumR[x][y] = photo(x, y, 0);
            prefixSumG[x][y] = photo(x, y, 1);
            prefixSumB[x][y] = photo(x, y, 2);

            // Calculate prefix sum for each channel
            if (x > 0) {
                prefixSumR[x][y] += prefixSumR[x - 1][y];
                prefixSumG[x][y] += prefixSumG[x - 1][y];
                prefixSumB[x][y] += prefixSumB[x - 1][y];
            }
            if (y > 0) {
                prefixSumR[x][y] += prefixSumR[x][y - 1];
                prefixSumG[x][y] += prefixSumG[x][y - 1];
                prefixSumB[x][y] += prefixSumB[x][y - 1];
            }
            if (x > 0 && y > 0) {
                prefixSumR[x][y] -= prefixSumR[x - 1][y - 1];
                prefixSumG[x][y] -= prefixSumG[x - 1][y - 1];
                prefixSumB[x][y] -= prefixSumB[x - 1][y - 1];
            }
        }
    }
}
void blur_filter(Image& photo , int radius) {
    vector<vector<int>> prefixSumR;
    vector<vector<int>> prefixSumG;
    vector<vector<int>> prefixSumB;

    int diameter = (2 * radius + 1) * (2 * radius + 1);
    int width = photo.width;
    int height = photo.height;
    calculatePrefixSum(photo, prefixSumR, prefixSumG, prefixSumB);
    // Apply the blur filter
    for (int x = radius; x < width - radius; ++x) {
        for (int y = radius; y < height - radius; ++y) {
            // Check if the indices are within bounds
            if (x + radius < width && y + radius < height && x - radius - 1 >= 0 && y - radius - 1 >= 0) {
                // Calculate the sum of pixel values within the filter window using prefix sums
                int sum_red = prefixSumR[x + radius][y + radius] - prefixSumR[x - radius - 1][y + radius] -
                              prefixSumR[x + radius][y - radius - 1] + prefixSumR[x - radius - 1][y - radius - 1];
                int sum_green = prefixSumG[x + radius][y + radius] - prefixSumG[x - radius - 1][y + radius] -
                                prefixSumG[x + radius][y - radius - 1] + prefixSumG[x - radius - 1][y - radius - 1];
                int sum_blue = prefixSumB[x + radius][y + radius] - prefixSumB[x - radius - 1][y + radius] -
                               prefixSumB[x + radius][y - radius - 1] + prefixSumB[x - radius - 1][y - radius - 1];

                // Calculate the average values using floating-point division
                int average_red = sum_red / diameter;
                int average_green = sum_green / diameter;
                int average_blue = sum_blue / diameter;

                // Update the result image with the average values
                photo(x, y, 0) = average_red;
                photo(x, y, 1) = average_green;
                photo(x, y, 2) = average_blue;
            }
        }
    }
    photo.saveImage("blur.jpg");
}

void blur_menu(Image& photo) {
    cout << "choose a level of blur :" << endl;
    cout << "1)Week" << endl
         << "2)Medium" << endl
         << "3)Strong" << endl;
    int choice = Validmenu_Choice(3);
    while (true) {
        if (choice == 1) {
            blur_filter(photo , 11);
        } else if (choice == 2) {
            blur_filter(photo , 21);
        } else if (choice == 3) {
            blur_filter(photo , 31);
        }
        else {
            cout << "INVALID CHOICE !" << endl;
        }
        break;

    }
}

//Filter 13
// Function to apply the sunrise effect to the image
void applySunriseEffect(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            // Calculate new RGB values for sunrise effect
            unsigned int redIntensity = image(i, j, 0); // Red channel
            unsigned int greenIntensity = image(i, j, 1); // Green channel
            unsigned int blueIntensity = image(i, j, 2); // Blue channel

            // Adjust RGB values for sunrise effect
            redIntensity += 30; // Increase red for warm sunrise effect
            greenIntensity += 40; // Increase green for warm sunrise effect
            blueIntensity -= 30; // Decrease blue for warm sunrise effect

            // Ensure RGB values are within the valid range
            if (redIntensity > 255) redIntensity = 255;
            if (greenIntensity > 255) greenIntensity = 255;
            if (blueIntensity < 0) blueIntensity = 0;

            // Update the image with the new RGB values
            image(i, j, 0) = redIntensity;
            image(i, j, 1) = greenIntensity;
            image(i, j, 2) = blueIntensity;
        }
    }
}

// filter 16
void purple_filter(Image& photo){
    for (int i = 0 ; i < photo.width; ++i) {
        for (int j = 0; j < photo.height; ++j) {
            photo(i, j, 0) *= 1 ;
            photo(i, j, 1) *= 0.70 ;
            photo(i, j, 2) *=  1 ;
        }
    }
    photo.saveImage("purple-filter.jpg");
}

// filter 17
void Infrared(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            int temp =image(i,j,1);
            image(i,j,0) = 255;
            image(i,j,1) = 255 - image(i,j,2);
            image(i,j,2) = 255 - temp;
        }
    }
}

//routaion menu
void Rotation_menu(Image& original) {
    while (true) {
        cout << endl << "Choose a degree :" << endl;
        cout << "1) 90's degree" << endl;
        cout << "2) 180's degree" << endl;
        cout << "3) 270's clockwise" << endl;
        cout << "4) 360's clockwise" << endl;

        int choice = Validmenu_Choice(4); // Accepts numbers 1 to 4

        if (choice == 1) {
            Rotate_90(original);

        }
        else if (choice == 2) {
            Rotate_180(original);

        }
        else if (choice == 3) {
            Rotate_270(original);

        }
        else if (choice == 4) {

            continue;
        }
        else {
            cout << "INVALID CHOICE !" << endl;
            continue;
        }
        break;
    }


}

//main program

void main_program(Image& original) {
    int choice;
    while (true) {
        cout << "==> CHOOSE A FILTER:" << endl;
        cout << "1) Grayscale Conversion" << endl;
        cout << "2) Black and White" << endl;
        cout << "3) Invert Image" << endl;
        cout << "4) Merge Images" << endl;
        cout << "5) Flip Image" << endl;
        cout << "6) Rotate Image" << endl;
        cout << "7) Darken and Lighten Image"<<endl;
        cout << "8) Crop Images"<<endl;
        cout << "9) Adding a Frame to the Picture"<<endl;
        cout << "10) Detect Image Edges"<<endl;
        cout << "11) Resizing Images"<<endl;
        cout << "12) Blur Images"<<endl;
        cout << "13) nature Sunlight" <<endl;
        cout << "14) purple Filter" <<endl;
        cout << "15) Infrared Filter"<<endl;
        cout << "16) EXIT" << endl;

        choice = Validmenu_Choice(16);

        if (choice == 1) {
            applyGrayscaleFilter(original);
        }
        else if (choice == 2) {
            convertToBlackAndWhite(original);
        }
        else if (choice == 3) {
            Inverted_Image(original);
        }
        else if (choice == 4) {
            string filename2 = validfilename_();
            Image image2(filename2);
            blendImages(original , image2);
        }
        else if (choice == 5) {
            flip_menu(original);
        }
        else if (choice == 6) {
            Rotation_menu(original);
        }
        else if (choice ==7){
            cout <<"1) Darken Image"<<endl<<"2) Lighten Image"<<endl<<"Enter Your choice :";
            int choice2;
            cin>>choice2;
            if (choice2 == 1){
                Darken_Image(original);
            }
            else if (choice2 ==2){
                lighten_Image(original);
            }
            else{
                cout <<"Invalid input, enter number from 1 or 2 : ";
            }
        }
        else if (choice ==8){
            cropImage(original);
        }
        else if (choice ==9){
            frame_menu(original);
        }
        else if (choice == 10){
            detectEdgesSobel(original);
        }
        else if (choice ==11){
            resizeImage(original);
        }
        else if (choice ==12){
            blur_menu(original);
        }
        else if (choice ==13){
            applySunriseEffect(original);
        }
        else if (choice ==14){
            purple_filter(original);
        }
        else if (choice ==15){
            Infrared(original);
        }
        else if (choice == 16) {
            cout << "PROGRAM HAS ENDED!" << endl;
            break; // End the program successfully
        }
        else {
            cout << "Invalid choice! Please choose a number between 1 and 7." << endl;
            continue; // Restart the loop to prompt for a valid choice
        }
        break;
    }
}

void MAIN() {
    cout << "WELCOME TO OUR PHOTO EDITOR PROGRAM" << endl;

    string filename;
    Image currentImage;
    Image editedImage;

    while (true) {
        cout << "==> CHOOSE AN OPTION" << endl;
        cout << "1) Load a new image" << endl;
        cout << "2) Apply a filter on the current image" << endl;
        cout << "3) Save the current image" << endl;
        cout << "4) Exit the program" << endl;

        int choice = Validmenu_Choice(4);

        if (choice == 1) {
            filename = validfilename_();
            // Load the photo into the Image object
            currentImage.loadNewImage(filename);
        }
        else if (choice == 2) {
            if (filename.empty()) {
                cerr << "No image is loaded. Please load an image before applying a filter." << endl;
                continue;
            }
            main_program(currentImage);
        }
        else if (choice == 3) {
            if (filename.empty()) {
                cerr << "No image is loaded. Please load an image before saving." << endl;
                continue;
            }
            // Edit filename for save functionality
            string output_filename;
            cout << "Enter the name of the output file, including the extension: ";
            cin >> output_filename;
            currentImage.saveImage(output_filename);
            cout << "Image saved as " << output_filename << endl;
            system(output_filename.c_str()); //display image for the user
        }
        else if (choice == 4) {
            cout << "PROGRAM HAS ENDED!" << endl;
            break;
        }
        else {
            cout << "Invalid choice! Please choose a number between 1 and 4." << endl;
        }
    }
}

int main(){
    MAIN();
}