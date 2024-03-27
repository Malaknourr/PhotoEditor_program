/*
File Name : CS112_A3_part1_S19_20230052_20230419_20230137.cpp
Authors :
(1) Malak Nour-Elhussien Samir , ID: 20230419 , S19 , Filter taken : 3 and 6
 email : nourrm218@gmail.com

(2) Esraa Hussien Mohamed , ID : 20230052  ,  S19  ,  Filter taken : 1 and 4
email  :  ehussien143@gmail.com

(3) Rahma salah Eldin Mahmoud ,ID : 20230137 , S19 , Filter taken :  2 and 5
 email  :  rahmasalah072@gmail.com

Date : 26 March 2024

      << System Diagram >>

 START

    |--- WELCOME MESSAGE
    |
    |--- LOOP UNTIL EXIT
    |       |
    |       |---> [1] Load a new image
    |       |
    |       |---> [2] Apply a filter on the current image
    |       |
    |       |---> [3] Save the current image
    |       |
    |       |---> [4] Exit the program
    |
    |--- IF [1] Load a new image
    |       |
    |       |---> PROMPT USER FOR FILENAME
    |       |
    |       |---> LOAD IMAGE
    |       |
    |       |---> GO BACK TO LOOP
    |
    |--- IF [2] Apply a filter on the current image
    |       |
    |       |--- LOOP UNTIL EXIT
    |       |       |
    |       |       |---> CHOOSE A FILTER:
    |       |       |       |
    |       |       |       |---> [1] Grayscale Conversion
    |       |       |       |
    |       |       |       |---> [2] Black and White
    |       |       |       |
    |       |       |       |---> [3] Invert Image
    |       |       |       |
    |       |       |       |---> [4] Merge Images
    |       |       |       |
    |       |       |       |---> [5] Flip Image
    |       |       |       |
    |       |       |       |---> [6] Rotate Image
    |       |       |       |
    |       |       |       |---> [7] EXIT
    |       |
    |       |--- IF [1] Grayscale Conversion
    |       |       |
    |       |       |---> APPLY GRAYSCALE FILTER
    |       |
    |       |--- IF [2] Black and White
    |       |       |
    |       |       |---> APPLY BLACK AND WHITE FILTER
    |       |
    |       |--- IF [3] Invert Image
    |       |       |
    |       |       |---> APPLY INVERTED IMAGE FILTER
    |       |
    |       |--- IF [4] Merge Images
    |       |       |
    |       |       |---> PROMPT USER FOR SECOND FILENAME
    |       |       |
    |       |       |---> LOAD SECOND IMAGE
    |       |       |
    |       |       |---> BLEND IMAGES
    |       |
    |       |--- IF [5] Flip Image
    |       |       |
    |       |       |---> CHOOSE FLIP TYPE
    |       |       |
    |       |       |---> FLIP IMAGE
    |       |
    |       |--- IF [6] Rotate Image
    |       |       |
    |       |       |---> CHOOSE ROTATION DEGREE
    |       |       |
    |       |       |---> ROTATE IMAGE
    |       |
    |       |--- IF [7] EXIT
    |               |
    |               |---> GO BACK TO MAIN MENU
    |
    |--- IF [3] Save the current image
    |       |
    |       |---> PROMPT USER FOR OUTPUT FILENAME
    |       |
    |       |---> SAVE IMAGE
    |
    |--- IF [4] Exit the program
            |
            |---> DISPLAY EXIT MESSAGE
            |
            |---> END PROGRAM

END
*/

#include "Image_Class.h"
#include <iostream>
#include <algorithm>
#include <string>
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
            } catch (const std::invalid_argument& e) {
                cerr << e.what() << endl; // Print the error message
                cerr << "Please provide a valid filename." << endl;
                cin.clear(); // Clear the error flag
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
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
void applyGrayscaleFilter(Image& image) {
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
//    return result;
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

    toy1 =  Enlarge_To(toy1,max(toy1.width,toy2.width),max(toy1.height,toy2.height));
    toy2 =  Enlarge_To(toy2,max(toy1.width,toy2.width),max(toy1.height,toy2.height));
    int maxWidth = std::max(toy1.width, toy2.width);
    int maxHeight = std::max(toy1.height, toy2.height);
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
    toy1 =img2;
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
//            return original;
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
        cout << "7) EXIT" << endl;

        choice = Validmenu_Choice(7);

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
            cout << "Please insert the second filename of your photo: ";
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
        else if (choice == 7) {
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
        cout << "==> CHOOSE AN OPTION:" << endl;
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
            system(output_filename.c_str());
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