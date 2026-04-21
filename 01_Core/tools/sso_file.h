#ifndef SSO_FILE_H
#define SSO_FILE_H

#include <string>

std::string SSO_OpenFile(const char* filter = "All Files (*.*)\0*.*\0", const char* title = "Select File");
std::string SSO_SaveFile(const char* filter = "All Files (*.*)\0*.*\0", const char* title = "Save As");
std::string SSO_BrowseFolder(const char* title = "Select Folder");

#endif