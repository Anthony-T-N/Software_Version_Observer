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

void download_file(std::string url)
{
    // Function uses: <iostream>

    CURL* curl;
    FILE* fp;
    CURLcode response;
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen("temp_html.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        response = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (response != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(response));
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

void info_extraction(std::string downloaded_file_line)
{
    int temp_int = 0;
    for (int i = 0; i <= downloaded_file_line.length(); i++)
    {
        if (downloaded_file_line[i] == '/' && i < downloaded_file_line.find_last_of("0123456789"))
        {
            temp_int = i;
        }
    }
    //std::cout << "================================================================================" << "\n";
    //std::cout << downloaded_file_line << "\n";
    int pos = 0;
    for (int i = downloaded_file_line.find("href=\"") + 6; i <= downloaded_file_line.length(); i++)
    {
        //std::cout << i << " " << downloaded_file_line[i] << " " << downloaded_file_line[i];
        if (downloaded_file_line[i] == '\"')
        {
            pos = i;
            break;
        }
    }
    // //std::cout << "\n";
    // //std::cout << downloaded_file_line.find("href=\"") + 6 << " pos: " << pos << "\n";;
    pos -= (downloaded_file_line.find("href=\"") + 6);
    std::string temp_str = "";
    temp_str = downloaded_file_line.substr(downloaded_file_line.find("href=\"") + 6, pos);
    if (temp_str.find_first_of("0123456789") != std::string::npos)
    {
        std::cout << "[Info]: " << temp_str << "\n";
    }
    //std::cout << "================================================================================" << "\n";
    //downloaded_file_line.erase(0, downloaded_file_line.find_first_of("h"));
    //downloaded_file_line.erase(0, temp_int);
}

bool source_search(std::string &downloaded_file_line)
{
    /*
    * Conditions:
    * - Contains a number.
    * - Has at least one period.
    * - Numbers are after "href".
    * - May contain an exe.
    * Considerations:
    * - Desired version numbers and file extensions appear after "href" and a "/".
    * < href | version # | exe >
    */

    // Only show any HTML lines containing numbers (Possible version number).
    if (downloaded_file_line.find_first_of("0123456789") != std::string::npos)
    {
        // Show any HTML lines containing the "exe" extension (Possible executable).
        if (downloaded_file_line.find("href") != std::string::npos && (downloaded_file_line.find("exe") != std::string::npos) || (downloaded_file_line.find("msi") != std::string::npos))
        {
            //std::cout << "DEBUG: " << downloaded_file_line.find("href") << "\n";
            if (downloaded_file_line.find("href") < downloaded_file_line.find_last_of("0123456789"))
            {
                return true;
            }
        }
        // For example: Python download page does not display exe(s) on main download page.
        else if (downloaded_file_line.find("href") != std::string::npos && downloaded_file_line.find("download") != std::string::npos)
        {
            if (downloaded_file_line.find("href") < downloaded_file_line.find_last_of("0123456789"))
            {
                //downloaded_file_line.erase(0, downloaded_file_line.find_first_of("h"));
                return true;
            }
        }
    }
    return false;
}

std::string user_input_validation(std::string &user_input)
{
    // Function uses: <iostream>,

    std::getline(std::cin, user_input);
    while (std::cin.fail() || user_input.find_first_not_of("12") != std::string::npos || user_input.empty())
    {
        if (user_input == "e" || user_input == "exit")
        {
            break;
        }
        std::cout << "[-] Invalid input - Please try again: ";
        std::getline(std::cin, user_input);
    }
    return user_input;
}

int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Welcome to the Software_Version_Observer console application" << "\n";
    std::cout << "- Console Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    std::string user_input;
    std::cout << "Select mode \"1\" to enter URLs to store" << "\n";
    std::cout << "Select mode \"2\" to scan URLs for updates" << "\n";
    std::cout << "Mode ? (1/2)" << "\n";
    user_input_validation(user_input);
    if (user_input == "1")
    {
        std::string URL_input;
        while (true)
        {
            std::cout << "[>] Enter a valid URL: " << "\n";
            std::getline(std::cin, URL_input);

            if (URL_input == "e" || URL_input == "exit")
            {
                break;
            }
            // Check whether URL is valid here:
            else if (url_validation(URL_input) == false)
            {
                std::cout << "[-] Invalid URL. Please try again: " << "\n\n";
            }
            else
            {
                // Function to accept and store URLs here:
                write_to_text_collection(URL_input);
            }
        }
    }
    if (user_input == "2")
    {
        std::ifstream input_file;
        std::string input_file_line;
        input_file.open("software_version_collection.txt");
        while (std::getline(input_file, input_file_line))
        {
            std::cout << "\n" << "[!] Reading: " << input_file_line << "\n";
            download_file(input_file_line);
            std::ifstream downloaded_file;
            std::string downloaded_file_line;
            downloaded_file.open("temp_html.txt");
            while (std::getline(downloaded_file, downloaded_file_line))
            {
                // Identifies whether current line meets any of the conditions to be a potential line of interest.
                if (source_search(downloaded_file_line) == true)
                {
                    info_extraction(downloaded_file_line);
                    //std::cout << "[+] " << downloaded_file_line << "\n";
                }
            }
            downloaded_file.close();
        }
        input_file.close();
    }
    std::cout << "\n";
    std::cout << "[!] END" << "\n";
    std::cout << "[!] Exiting..." << "\n\n";
    system("pause");
    return 0;


    // Two modes: 1) Accept a range of download links and store in a text file 2) Loop through all links in a text file and scan HTML source code for specific strings/keywords.

    // Allow user to input a download page for a specific software.
    // Read HTML source page for the specific software.
    // Identify download link and note version number.
    // Output version number.

    // Common elements: -Within "A" HTML element -Version contained in "href" tag -Download hyperlink thus far remains static.
    // href tags that contain exe are more likely to the correct file. (Consider implementing "confidence" levels)
}
