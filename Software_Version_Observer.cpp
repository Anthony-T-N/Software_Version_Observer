// Software_Version_Observer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>

// 

void write_to_text_collection(std::string& URL_input)
{
    // output file stream allows you to write contents to a file.
    std::ofstream output_file;
    if (std::filesystem::exists("software_version_collection.txt") == false)
    {
        std::cout << "[!] Creating new \"software_version_collection.txt\";" << "\n";
    }
    output_file.open("software_version_collection.txt", std::ios::app);
    std::cout << "[+] Opened \"software_version_collection.txt\" successfully;" << "\n";
    output_file << URL_input << "\n";
    output_file.close();
}

int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Welcome to the Software_Version_Observer console application" << "\n";
    std::cout << "- Console Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    std::string URL_input;
    std::getline(std::cin, URL_input);
    // Function to accept URL here.
    write_to_text_collection(URL_input);

    // Two modes: 1) Accept a range of download links and store in a text file 2) Loop through all links in a text file and scan HTML source code for specific strings/keywords.

    // Allow user to input a download page for a specific software.
    // Read HTML source page for the specific software.
    // Identify download link and note version number.
    // Output version number.

    // Common elements: -Within "A" HTML element -Version contained in "href" tag -Download hyperlink thus far remains static.
    // href tags that contain exe are more likely to the correct file. (Consider implementing "confidence" levels)
}
