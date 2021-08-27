// Software_Version_Observer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>

// https://stackoverflow.com/questions/21873048/getting-an-error-fopen-this-function-or-variable-may-be-unsafe-when-complin/21873153
#pragma warning(disable:4996);

// Stores entered URLs into a text file.
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
    std::cout << "[+] Successfully stored " << URL_input << "\n\n";
    output_file.close();
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}

void download_file(const char* url)
{
    // Function uses: <iostream>

    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen("text.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        fclose(fp);
    }
}

bool url_validation(std::string URL_input)
{
    // https://stackoverflow.com/questions/35068252/how-can-i-verify-if-an-url-https-exists-in-c-language/35068818
    CURL* curl;
    CURLcode response;
    curl = curl_easy_init();

    if (curl) 
    {
        // Understand c_str() please.
        curl_easy_setopt(curl, CURLOPT_URL, URL_input.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        response = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    if (response != CURLE_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
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
    while (URL_input != "e" && URL_input != "exit")
    {
        std::cout << "[>] Enter a valid URL: " << "\n";
        std::getline(std::cin, URL_input);
        
        // Check whether URL is valid here:
        if (url_validation(URL_input) == false)
        {
            std::cout << "[-] Invalid URL. Please try again: " << "\n\n";
        }
        else
        {
            // Function to accept and store URLs here:
            write_to_text_collection(URL_input);
        }
    }
    download_file("Test");

    // Two modes: 1) Accept a range of download links and store in a text file 2) Loop through all links in a text file and scan HTML source code for specific strings/keywords.

    // Allow user to input a download page for a specific software.
    // Read HTML source page for the specific software.
    // Identify download link and note version number.
    // Output version number.

    // Common elements: -Within "A" HTML element -Version contained in "href" tag -Download hyperlink thus far remains static.
    // href tags that contain exe are more likely to the correct file. (Consider implementing "confidence" levels)
}
