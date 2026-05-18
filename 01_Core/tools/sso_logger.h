// ==============================================================================
// SSOEngine v1.7 - OFFICIAL LICENSE AND ATTRIBUTION FILE //
// ==============================================================================
// // AUTHOR & LEAD DEVELOPER: // Rozak // // CONTACT: //
// Email: spctacularstudio@gmail.com // GitHub: https://github.com/Rozaq125 //
// Website: https://ssoengine.pages.dev // // SUPPORT THE PROJECT: // Donationware
// Link: https://saweria.co/omedonation // //
// ------------------------------------------------------------------------------
// MIT LICENSE //
// ------------------------------------------------------------------------------
// Copyright (c) 2026 SSOFoundation// // Permission
// is hereby granted, free of charge, to any person obtaining a copy // of this
// software and associated documentation files (the "Software"), to deal // in the
// software without restriction, including without limitation the rights // to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell // copies of
// the Software, and to permit persons to whom the Software is // furnished to do
// so, subject to the following conditions: // // The above copyright notice and
// this permission notice shall be included in all // copies or substantial
// portions of the Software. // // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT
// WARRANTY OF ANY KIND, EXPRESS OR // IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, // FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER // LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE // SOFTWARE. // //
// ------------------------------------------------------------------------------
// BUILT WITH & SPECIAL THANKS //
// ------------------------------------------------------------------------------
// RAYLIB: (c) Ramon Santamaria (https://www.raylib.com) // COMPILER: MinGW-w64
// Project (GCC Suite) // CONTRIBUTOR: Nathan (Feedback Provider) // // COMMUNITY
// HALL OF FAME (Reddit Feedback & Support): // u/rileygrham // u/Jimmy-M-420 //
// u/gabriel_aplok // u/InitialComfortable70 // u/ghulmar // //
// ------------------------------------------------------------------------------
// PROJECT DESCRIPTION //
// ------------------------------------------------------------------------------
// SSOEngine is a professional-grade, cross-platform 2D/3D game engine SDK //
// built on top of Raylib //
// high-precision physics, advanced camera systems, and a professional build //
// pipeline for Windows and Android distribution. // // VERSION: 1.7 //
// ==============================================================================

#ifndef SSO_LOGGER_H
#define SSO_LOGGER_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

namespace SSO {
    class Logger {
    public:
        enum class Level { INFO, WARNING, ERROR, SUCCESS };

        /**
         * @brief Logs a message to the console with color and timestamp.
         */
static void Log(Level level, const std::string& message) {
            // Get current time
            auto now = std::chrono::system_clock::now();
            auto timeT = std::chrono::system_clock::to_time_t(now);
            
            // Use localtime_s (Windows) or localtime (Cross-platform)
            // For simplicity and headers, we use localtime:
            std::tm* localTime = std::localtime(&timeT);

            // Print timestamp
            std::cout << "[" << std::put_time(localTime, "%H:%M:%S") << "] ";

            // Print level with ANSI colors
            switch (level) {
                case Level::INFO:    std::cout << "\033[36m[INFO] "; break;    // Cyan
                case Level::WARNING: std::cout << "\033[33m[WARNING] "; break; // Yellow
                case Level::ERROR:   std::cout << "\033[31m[ERROR] "; break;   // Red
                case Level::SUCCESS: std::cout << "\033[32m[SUCCESS] "; break; // Green
            }

            // Print message and reset color
            std::cout << message << "\033[0m" << std::endl;
        }
    };
}

#endif