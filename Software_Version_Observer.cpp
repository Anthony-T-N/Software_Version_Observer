// Software_Version_Observer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Welcome to the Software_Version_Observer console application" << "\n";
    std::cout << "- Console Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    // Allow user to input a download page for a specific software.
    // Read HTML source page for the specific software.
    // Identify download link and note version number.
    // Output version number.

    // Common elements: -Within "A" HTML element -Version contained in "href" tag -Download hyperlink thus far remains static.
    // href tags that contain exe are more likely to the correct file.



}
