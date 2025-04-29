//
// Copyright 2015-2016 Jonathan Bennett <jon@autoitscript.com>
// 
// https://www.autoitscript.com 
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

#include <stdio.h>
#include <tchar.h>

#include "text_encoding_detect.h"
using namespace AutoIt::Common;


int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 2)
	{
		_tprintf(_T("\nUsage: %s filename."), argv[0]);
		return 1;
	}
	
	// Open file in binary mode
	FILE *file = _tfopen(argv[1], _T("rb") );

	if (file == NULL)
	{
		_tprintf(_T("\nCould not open file.\n"));
		return 1;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read it all in
	unsigned char *buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	// Detect the encoding
	TextEncodingDetect textDetect;
	TextEncodingDetect::Encoding encoding = textDetect.DetectEncoding(buffer, fsize);

	_tprintf(_T("\nEncoding: "));
	if (encoding == TextEncodingDetect::None)
		_tprintf(_T("Binary"));
	else if (encoding == TextEncodingDetect::ASCII)
		_tprintf(_T("ASCII (chars in the 0-127 range)"));
	else if (encoding == TextEncodingDetect::ANSI)
		_tprintf(_T("ANSI (chars in the range 0-255 range)"));
	else if (encoding == TextEncodingDetect::UTF8_BOM || encoding == TextEncodingDetect::UTF8_NOBOM)
		_tprintf(_T("UTF-8"));
	else if (encoding == TextEncodingDetect::UTF16_LE_BOM || encoding == TextEncodingDetect::UTF16_LE_NOBOM)
		_tprintf(_T("UTF-16 Little Endian"));
	else if (encoding == TextEncodingDetect::UTF16_BE_BOM || encoding == TextEncodingDetect::UTF16_BE_NOBOM)
		_tprintf(_T("UTF-16 Big Endian"));

	// Free up
	delete[] buffer;

	return 0;
}

