# Assignment3_
PHOTOSHOP CODES
#include "Image_Class.h"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

bool isValidExtension(const string& filename) {
    // Get the position of the last dot in the filename
    size_t dotPos = filename.find_last_of('.');

    // Check if the dot was found and if it's not the last character in the filename
    if (dotPos != string::npos && dotPos != filename.length() - 1) {
        // Extract the extension from the filename
        string extension = filename.substr(dotPos + 1);

        // List of supported extensions
        const string supportedExtensions[] = { "jpg", "jpeg", "png", "bmp", "tga" };

        // Check if the extracted extension matches any of the supported extensions
        for (const string& ext : supportedExtensions) {
            if (extension == ext) {
                return true; // Extension is valid
            }
        }
    }

    return false; // Extension is not valid
}

int Validmenu_Choice(int start,int endd) {
    int Number;
    while(true) {
        string input1;
        cin.ignore();
        getline(cin , input1);
        if (input1.find_first_not_of("0123456789") != string::npos) {  // Check if the input contains only digits.
            cout << "Please enter a valid number :" << endl;
            continue;   //This line causes the loop to skip the rest of the current iteration and start a new one if the input is invalid.
        }
        //check validation.
        try {
            Number = stoi(input1); // convert input to an integer
            if (Number < start || Number > endd) {
                cout << "Please enter a valid number : " << endl;
                continue;
            } else
            {
                break;  //This line sets the flag to false once a valid number is entered, terminating the loop.
            }
        }
        catch (const invalid_argument &e){ //to prevent error when the user click enter without write anything
            cout << "Invalid input. Please enter a valid number : " <<endl;
            cin.clear();    // Clear error flags
            continue;
        }
        catch (const out_of_range &e) {   //to prevent error when the user input a very big ranged number.
            cout << "Input out of range. Please enter a valid number within the range :" <<endl;
            cin.clear();
            continue;
        }
    }

    return Number;
}

//filter 1

// Function to apply Filter 1: Convert image to grayscale
void applyGrayscaleFilter(string& filename) {
    Image image(filename);
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
    image.saveImage("grayscale_" + filename);
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

// int main() {
//     string filename;
//     cout << "Pls enter colored image name to turn to black and white: ";
//     cin >> filename;
//     Image image(filename);
//     convertToBlackAndWhite(image);
//     cout << "Pls enter image name to store new image\n";
//     cout << "and specify extension .jpg, .bmp, .png, .tga: ";
//     cin >> filename;
//     image.saveImage(filename);
//     system(filename.c_str());
//     return 0;
// }

//filter 3

void Inverted_Image(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int n = 0; n < image.height; n++) {
            for (int k = 0; k < 3; k++) {
                image.setPixel(i, n, k, 255 - image.getPixel(i, n, k));
            }
        }
    }
}

//filter 4

// Function to apply Filter 4: Blend two images
void blendImages(string& filename, string& filename2) {
    Image toy1(filename);
    Image toy2(filename2);
    int minWidth = std::min(toy1.width, toy2.width);
    int minHeight = std::min(toy1.height, toy2.height);
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

    img2.saveImage("blended_" + filename + "_" + filename2);
}

//filter 5

// Function to flip the image horizontally
Image flipHorizontally(Image& image) {
    for (int i = 0; i < image.width / 2; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                swap(image(i, j, k), image(image.width - i - 1, j, k));
            }
        }
    }
    return image;
}
// Function to flip the image vertically
Image flipVertically(Image& image) {
    for (int j = 0; j < image.height / 2; ++j) {
        for (int i = 0; i < image.width; ++i) {
            for (int k = 0; k < 3; ++k) {
                swap(image(i, j, k), image(i, image.height - j - 1, k));
            }
        }
    }
    return image;
}
//flip menu
void flip_menu(Image& original) {
    while (true) {
        cout << "Choose a flip :" << endl;
        cout << "1) Vertically" << endl;
        cout << "2) Flip horizontally" << endl;

        int choice = Validmenu_Choice(1,2);

        if (choice == 1) {
            Image flip_Vertically = flipVertically(original);
            flip_Vertically.saveImage("arrowVertically_image.jpg");
            break; // Exit the loop after processing the choice
        } else if (choice == 2) {
            Image flip_horizontally = flipHorizontally(original);
            flip_horizontally.saveImage("arrowhorizontally_image.jpg");
            break; // Exit the loop after processing the choice
        }
    }
}

//filter 6

Image Rotate_270(Image& original) {
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

    return rotated;
}

Image Rotate_90(Image& original) {
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

    return rotated;
}


Image Rotate_180(Image& original) {
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

    return rotated;
}

void Rotation_menu(Image& original) {
    while (true) {
        cout << endl << "Choose a degree :" << endl;
        cout << "1) 90's degree" << endl;
        cout << "2) 180's degree" << endl;
        cout << "3) 270's clockwise" << endl;
        cout << "4) 360's clockwise" << endl;

        int choice = Validmenu_Choice(1,4);

        if (choice == 1) {
            // Rotate the image 90 degrees clockwise
            Image rotated90 = Rotate_90(original);
            rotated90.saveImage("rotated90_image.jpg");
            break;
        } else if (choice == 2) {
            // Rotate the image 180 degrees clockwise
            Image rotated180 = Rotate_180(original);
            rotated180.saveImage("rotated180_image.jpg");
            break;
        } else if (choice == 3) {
            // Rotate the image 270 degrees clockwise
            Image rotated270 = Rotate_270(original);
            rotated270.saveImage("rotated270_image.jpg");
            break;
        } else if (choice == 4) {
            // Rotate the image 360 degrees clockwise
            Image rotated360 = original;
            rotated360.saveImage("rotated360_image.jpg");
            break;
        }
    }
}


//main program
int main() {
    cout << "WELCOME TO OUR PHOTO EDITOR PROGRAM" << endl;

    // Take the photo from the user and check the validation of it
    string filename;
    cout << "Please insert the filename of your photo: ";
    cin >> filename;

    // Load the photo into the Image object
    Image original(filename);

    if (!isValidExtension(filename)) {
        cerr << "Unsupported file format. Please provide a filename with a supported extension (jpg, jpeg, png, bmp, tga)." << endl;
        return 1; // Return error code indicating failure
    }

    // Check if the image was loaded successfully
    if (original.imageData == nullptr) {
        cerr << "Failed to load the image! Please check the filename." << endl;
        return 1; // Return error code indicating failure
    }

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

        choice = Validmenu_Choice(1,7);

        if (choice == 1) {
            applyGrayscaleFilter(filename);
        } else if (choice == 2) {
            convertToBlackAndWhite(original);
            original.saveImage("building.jpg");
        } else if (choice == 3) {
            Inverted_Image(original);
            original.saveImage("inverted1.jpg");
        } else if (choice == 4) {
            string filename2;
            cout << "Please insert the second filename of your photo: ";
            cin >> filename2;
            blendImages(filename, filename2);
        } else if (choice == 5) {
            flip_menu(original);
        } else if (choice == 6) {
            Rotation_menu(original);
        } else if (choice == 7) {
            cout << "PROGRAM HAS ENDED!" << endl;
            return 0; // End the program successfully
        } else {
            cout << "Invalid choice! Please choose a number between 1 and 7." << endl;
        }
    }
    return 0;
}
