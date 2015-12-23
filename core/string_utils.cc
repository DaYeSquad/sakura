////
////  string_utils.cpp
////  LessChatCore
////
////  Created by Frank Lin on 12/9/14.
////
////
//
//#include "string_utils.h"
//
//USING_NS_LCC;
//
//std::string string_vector_join(const std::vector<std::string>& v, const std::string& token) {
//	std::ostringstream result;
//	for (auto i = v.begin(); i != v.end(); i++){
//		if (i != v.begin()) result << token;
//		result << *i;
//	}
//	return result.str();
//}
//
//std::vector<std::string> string_split(const std::string& s, const std::string& delimiter) {
//	std::vector<std::string> result;
//	std::string::size_type from = 0;
//	std::string::size_type to = 0;
//
//	while (to != std::string::npos){
//		to = s.find(delimiter, from);
//		if (from < s.size() && from != to){
//			result.push_back(s.substr(from, to - from));
//		}
//		from = to + delimiter.size();
//	}
//	return result;
//}