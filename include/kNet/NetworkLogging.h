/* Copyright 2010 Jukka Jyl�nki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
#pragma once

/** @file NetworkLogging.h
	@brief The LOGNET and LOGNETVERBOSE macros. Provides an unified mechanism for logging. */

namespace kNet
{

/// A bitfield type that describes single or multiple log channels (each bit represents a channel).
typedef unsigned int LogChannel;

namespace
{
const LogChannel LogUser = 1;        ///< This log channel is free to use for user-level application messages.
const LogChannel LogInfo = 2;        ///< Successful connects and disconnects.
const LogChannel LogError = 4;       ///< Successful connects and disconnects.
const LogChannel LogObjectAlloc = 8; ///< Successful connects and disconnects.
const LogChannel LogData = 16;       ///< Successful connects and disconnects.
const LogChannel LogVerbose = 32;    ///< Print all information for debugging purposes.
}

/// Prints a variadic line to log.
/// @param logChannel The log channel to print to. This variable must have exactly one bit set, and is ANDed against
///                   the currently active channels that can be set with a call to kNet::SetLogChannels.
/// @param filename The C++ file from which the log message occurs, pass in __FILE__.
/// @param lineNumber Pass in __LINE__.
/// @param msg The printf-style message format specifier for the text to print.
void TimeOutputDebugStringVariadic(LogChannel logChannel, const char *filename, int lineNumber, const char *msg, ...);

/// Prints a message to the log. Same as kNet::TimeOutputDebugStringVariadic, but does not use printf formatting.
void TimeOutputDebugString(LogChannel logChannel, const char *filename, int lineNumber, const char *msg);

/// Sets the currently enabled log channels. Pass in a bitwise OR of the log channels you want to enable.
/// If you pass in 0, all logging is disabled. By default, only the channel LogChannelUser is enabled.
void SetLogChannels(LogChannel logChannel);

/// Returns the currently active log channels.
LogChannel GetLogChannels();

/// Sets the output for log messages. By default all logging is output to std::cout. Setting a log file
/// redirects all logging to that file. Calling this function with a null filename pointer restores
/// logging to target std::cout.
void SetLogFile(const char *filename);

} // ~kNet

/// Prints out a variadic message to the log channel User.
#define LOGUSER(msg, ...) kNet::TimeOutputDebugStringVariadic(LogUser, __FILE__, __LINE__, msg, __VA_ARGS__)
/// Prints out a message to the log channel User.
#define LOGUSER1(msg) kNet::TimeOutputDebugString(LogUser, __FILE__, __LINE__, msg)

/// Prints out a variadic message to the log channel Info.
#define LOGNET(msg, ...) kNet::TimeOutputDebugStringVariadic(LogInfo, __FILE__, __LINE__, msg, __VA_ARGS__)
/// Prints out a variadic message to the log channel Info.
#define LOGNET1(msg) kNet::TimeOutputDebugString(LogInfo, __FILE__, __LINE__, msg)

/// Prints out a variadic message to the log channel Verbose.
#define LOGNETVERBOSE(msg, ...) kNet::TimeOutputDebugStringVariadic(LogVerbose, __FILE__, __LINE__, msg, __VA_ARGS__)
/// Prints out a variadic message to the log channel Verbose.
#define LOGNETVERBOSE1(msg) kNet::TimeOutputDebugString(LogVerbose, __FILE__, __LINE__, msg)

/// Prints out a variadic message to the log channel Verbose.
#define LOG(channel, msg, ...) kNet::TimeOutputDebugStringVariadic(channel, __FILE__, __LINE__, msg, __VA_ARGS__)
/// Prints out a variadic message to the log channel Verbose.
#define LOG1(channel, msg) kNet::TimeOutputDebugString(channel, __FILE__, __LINE__, msg)
