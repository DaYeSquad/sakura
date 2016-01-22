//
//  http_multipart_request_test.cpp
//  WorktileCore
//
//  Created by Frank Lin on 11/10/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include <gtest/gtest.h>
#include "sakura/http_multipart_request.h"

USING_NS_SKR;
using std::unique_ptr;
using std::string;
using std::vector;

class HttpMultipartRequestTest : public testing::Test {
protected:
  
};

TEST_F(HttpMultipartRequestTest, TestAddPart) {
  HttpMultipartRequest request;
  request.AddPart("target", "avatar");
  
  vector<char> data = request.raw_data();
  string data_str(data.begin(), data.end());
  
  string expected_data = "------WebKitFormBoundaryQH5P0ytGqOFkQEh6\r\nContent-Disposition: form-data; name=\"target\"\r\n\r\navatar\r\n\r\n------WebKitFormBoundaryQH5P0ytGqOFkQEh6--\r\n";
  
  EXPECT_STREQ(expected_data.c_str(), data_str.c_str());
}
