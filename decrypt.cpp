#include <opencv4/opencv2/opencv.hpp>

void decryptImage(std::string inputImagePath, std::string outputImagePath, char key) {
    // Load the encrypted image
    cv::Mat image = cv::imread(inputImagePath, cv::IMREAD_UNCHANGED);

    // Check if the image was loaded successfully
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }

    // Decrypt the image with XOR cipher
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                image.at<cv::Vec3b>(y, x)[c] = image.at<cv::Vec3b>(y, x)[c] ^ key;
            }
        }
    }

    // Save the decrypted image
    cv::imwrite(outputImagePath, image);
}

int main() {
    decryptImage("Try decypher this, it just an image representation of a string.png", "decrypted_image.png", "khususproglan");
    return 0;
}