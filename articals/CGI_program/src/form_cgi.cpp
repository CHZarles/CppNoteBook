#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <map>

// URL解码函数
std::string urlDecode(const std::string& encoded) {
    std::string decoded;
    for (size_t i = 0; i < encoded.length(); i++) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            int value;
            std::istringstream is(encoded.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                decoded += static_cast<char>(value);
                i += 2;
            } else {
                decoded += encoded[i];
            }
        } else if (encoded[i] == '+') {
            decoded += ' ';
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

// 解析查询字符串
std::map<std::string, std::string> parseQueryString(const std::string& query) {
    std::map<std::string, std::string> params;
    
    std::istringstream iss(query);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = urlDecode(pair.substr(0, pos));
            std::string value = urlDecode(pair.substr(pos + 1));
            params[key] = value;
        }
    }
    
    return params;
}

int main() {
    // 输出 HTTP 头部
    std::cout << "Content-Type: text/html; charset=utf-8\r\n\r\n";
    
    // 获取请求方法
    char* method = getenv("REQUEST_METHOD");
    std::map<std::string, std::string> params;
    
    if (method && strcmp(method, "GET") == 0) {
        // 处理 GET 请求
        char* queryString = getenv("QUERY_STRING");
        if (queryString) {
            params = parseQueryString(std::string(queryString));
        }
    } else if (method && strcmp(method, "POST") == 0) {
        // 处理 POST 请求
        char* contentLength = getenv("CONTENT_LENGTH");
        if (contentLength) {
            int length = atoi(contentLength);
            if (length > 0) {
                std::string postData(length, '\0');
                std::cin.read(&postData[0], length);
                params = parseQueryString(postData);
            }
        }
    }
    
    // 输出 HTML 内容
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html lang=\"zh-CN\">\n";
    std::cout << "<head>\n";
    std::cout << "    <meta charset=\"UTF-8\">\n";
    std::cout << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    std::cout << "    <title>表单处理结果</title>\n";
    std::cout << "    <style>\n";
    std::cout << "        body {\n";
    std::cout << "            font-family: Arial, sans-serif;\n";
    std::cout << "            max-width: 800px;\n";
    std::cout << "            margin: 50px auto;\n";
    std::cout << "            padding: 20px;\n";
    std::cout << "            background-color: #f5f5f5;\n";
    std::cout << "        }\n";
    std::cout << "        .container {\n";
    std::cout << "            background-color: white;\n";
    std::cout << "            padding: 30px;\n";
    std::cout << "            border-radius: 10px;\n";
    std::cout << "            box-shadow: 0 2px 10px rgba(0,0,0,0.1);\n";
    std::cout << "        }\n";
    std::cout << "        h1 {\n";
    std::cout << "            color: #333;\n";
    std::cout << "            text-align: center;\n";
    std::cout << "        }\n";
    std::cout << "        .result {\n";
    std::cout << "            background-color: #e8f5e8;\n";
    std::cout << "            padding: 15px;\n";
    std::cout << "            border-radius: 5px;\n";
    std::cout << "            margin: 20px 0;\n";
    std::cout << "            border-left: 4px solid #4CAF50;\n";
    std::cout << "        }\n";
    std::cout << "        .param {\n";
    std::cout << "            margin: 10px 0;\n";
    std::cout << "            padding: 10px;\n";
    std::cout << "            background-color: #f9f9f9;\n";
    std::cout << "            border-radius: 3px;\n";
    std::cout << "        }\n";
    std::cout << "    </style>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "    <div class=\"container\">\n";
    std::cout << "        <h1>📝 表单数据处理结果</h1>\n";
    
    if (!params.empty()) {
        std::cout << "        <div class=\"result\">\n";
        std::cout << "            <h3>接收到的数据：</h3>\n";
        
        for (const auto& param : params) {
            std::cout << "            <div class=\"param\">\n";
            std::cout << "                <strong>" << param.first << ":</strong> " << param.second << "\n";
            std::cout << "            </div>\n";
        }
        
        std::cout << "        </div>\n";
        
        // 特殊处理某些字段
        if (params.find("name") != params.end()) {
            std::cout << "        <div class=\"result\">\n";
            std::cout << "            <h3>个性化响应：</h3>\n";
            std::cout << "            <p>你好, <strong>" << params["name"] << "</strong>! 欢迎使用我们的 CGI 程序。</p>\n";
            std::cout << "        </div>\n";
        }
        
    } else {
        std::cout << "        <div class=\"result\">\n";
        std::cout << "            <p>没有接收到表单数据。</p>\n";
        std::cout << "        </div>\n";
    }
    
    std::cout << "        <div style=\"margin-top: 30px; text-align: center;\">\n";
    std::cout << "            <p>请求方法: <strong>" << (method ? method : "未知") << "</strong></p>\n";
    std::cout << "        </div>\n";
    
    std::cout << "        <p style=\"text-align: center;\">\n";
    std::cout << "            <a href=\"/form.html\">重新填写表单</a> | \n";
    std::cout << "            <a href=\"/\">返回主页</a>\n";
    std::cout << "        </p>\n";
    std::cout << "    </div>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";
    
    return 0;
}
