#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

using namespace boost::algorithm;
using namespace std;

using boost::property_tree::ptree;

using std::stringstream;
using std::string;

vector<string> readCSVFile(string filename) {

	std::ifstream data(filename.c_str());
	std::string line;
	
	string xml="";
	
	long lngBatchSize = 5000;
	long lngIter=0;
	
	const int metColumnCount = 4;
	std::string metColumns[metColumnCount] = { "metTime", "Ta", "Ua", "Th" };
	
	std::vector<std::string> cols(metColumns, metColumns + metColumnCount);
	std::vector<std::string> cells;
	std::vector<std::string> vectXml;
	
	map<std::string, int> columnMap;
	
	// first row is column headings so find positions
	std::getline(data, line);
	boost::tokenizer<boost::escaped_list_separator<char> > tok(line);
	cells.assign(tok.begin(), tok.end());
	
	std::vector<std::string>::iterator iter_cells;
	std::vector<std::string>::iterator iter_cols;
	
	for (iter_cells = cells.begin(); iter_cells != cells.end(); ++iter_cells) {
		std::string cell = *iter_cells;
		cout << cell << "VAL" << endl;
		for(iter_cols = cols.begin(); iter_cols != cols.end(); ++iter_cols) {
			std::string col = *iter_cols;
			cout << cell << "=" << col;
			if(col.compare(cell) == 0) {
				cout << " MATCH" << endl;
				columnMap.insert(std::make_pair(*iter_cols,iter_cells-cells.begin()));
				break;
			}
			cout << endl;
		}
	}
	
	while (std::getline(data, line)) {
		boost::tokenizer<boost::escaped_list_separator<char> > tok(line);
		cells.assign(tok.begin(), tok.end());
		cols.assign(metColumns, metColumns + metColumnCount);		
		map<std::string, int>::iterator iter_map;
		
		// create values for record where available
		xml.append("\n(");
		for (iter_cols = cols.begin(); iter_cols != cols.end(); ++iter_cols) {
			std:string col = cols[iter_cols-cols.begin()];
			iter_map = columnMap.find(col);
			if (iter_cols != cols.begin()) {
				xml.append(",");
			}
			if (iter_map != columnMap.end()) {
				xml.append(cells[iter_map->second]);
			} else {
				xml.append("NULL");
			}
		}		
		xml.append("),");
		
		lngIter+=1;
		
		if((lngIter%lngBatchSize)==0) {
			vectXml.push_back(xml.substr(0, xml.size()-1));
			xml="";
		}
	}
	
	if ((lngIter%lngBatchSize)) {
		vectXml.push_back(xml.substr(0, xml.size()-1));
	}
	
    return vectXml;
}

int main(int argc, char** argv) {

	string folder = argv[1];
	
	boost::filesystem::directory_iterator dirItr(folder);

	for (dirItr; dirItr != boost::filesystem::directory_iterator(); ++dirItr) {
		filename = dirItr->path().string();
		ext =  dirItr->path().extension();
		if (ext == ".csv") {				
			cout << "Process File: " << filename << endl;
			std::vector<std::string> vectFiles = readCSVFile(filename);
		}
	}
	
	return 1;   
}
