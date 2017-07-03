// CsvParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream
#include <istream>
#include <string>
#include <vector>

enum class CSVState {
	UnquotedField,
	QuotedField,
	QuotedQuote
};

std::vector<std::wstring> readCSVRow(const std::wstring &row, const wchar_t &sep, const wchar_t &quote) {
	CSVState state = CSVState::UnquotedField;
	std::vector<std::wstring> fields{ L"" };
	size_t i = 0; // index of the current field
	char c;
	for (char c : row) {
		if (state == CSVState::UnquotedField) {
			if (c == sep) { // end of field
				fields.push_back(L""); i++;
			}
			else if (c == quote) {
				state = CSVState::QuotedField;
			} else {
			    fields[i].push_back(c);
			}
		}
		else if (state == CSVState::QuotedField) {
			if (c == quote) {
				state = CSVState::QuotedQuote;
			} else {
				fields[i].push_back(c);
		    }
	    }
		else if (state == CSVState::QuotedQuote) {
			if (c == sep) {
				fields.push_back(L""); i++;
				state = CSVState::UnquotedField;
			}
			else if (c == quote) { // "" -> "
				fields[i].push_back(c);
				state = CSVState::QuotedField;
			}
			else {
				fields[i].push_back(c);
				state = CSVState::UnquotedField;
			}
		}
	}
	return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::wstring>> readCSV(std::wistream &in, const wchar_t &sep, const wchar_t &quote) {
	std::vector<std::vector<std::wstring>> table;
	std::wstring row;
	while (true) {
		std::getline(in, row);
		if (in.bad() || in.eof()) {
			break;
		}
		auto fields = readCSVRow(row, sep, quote);
		table.push_back(fields);
	}
	return table;
}

int _tmain(int argc, _TCHAR* argv[])
{

	std::wstringstream  stream;
	stream << L"q,\"aaa,,bbb,ccc\",\"aaa\",\"'\",mmm, \"aaa\"\"bb\"b\"" << std::endl;
	//for (int i = 0; i < 10000000; ++i) {
	std::vector<std::vector<std::wstring>> data = readCSV(stream, L',', L'"');
	//std::cout << i << std::endl;
	//}
	
	return 0;
}

