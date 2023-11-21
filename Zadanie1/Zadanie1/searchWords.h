#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 32

/// <summary>
/// Searches for words in a text file that match a specified pattern.
/// This function reads the contents of the given file, tokenizes each line into words,
/// and compares each word against the provided pattern.The pattern may contain '_' (underscore)
/// as a wildcard character, matching any single character in the corresponding position.
/// </summary>
/// <param name="pattern">The pattern to match against words in the file.</param>
/// <param name="length">The length of the pattern.</param>
/// <param name="filename">he name of the file to search for matching words.</param>
/// <returns>
/// Returns a dynamically allocated string containing the first matched word found in the file,
/// or NULL if no matches are found or if there is an error opening the file.
/// </returns>
char* searchWords(const char* pattern, int length, const char* filename);
