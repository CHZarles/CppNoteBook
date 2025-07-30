#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    // 输出 HTTP 头部
    std::cout << "Content-Type: text/html\r\n\r\n";
    
    // 输出 HTML 内容
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html lang=\"zh-CN\">\n";
    std::cout << "<head>\n";
    std::cout << "    <meta charset=\"UTF-8\">\n";
    std::cout << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    std::cout << "    <title>Hello CGI</title>\n";
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
    std::cout << "        .info {\n";
    std::cout << "            background-color: #e8f4f8;\n";
    std::cout << "            padding: 15px;\n";
    std::cout << "            border-radius: 5px;\n";
    std::cout << "            margin: 20px 0;\n";
    std::cout << "        }\n";
    std::cout << "    </style>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "    <div class=\"container\">\n";
    std::cout << "        <h1>🎉 Hello from C++ CGI! 🎉</h1>\n";
    std::cout << "        <div class=\"info\">\n";
    std::cout << "            <h3>程序信息：</h3>\n";
    std::cout << "            <p><strong>语言：</strong>C++</p>\n";
    std::cout << "            <p><strong>协议：</strong>CGI (Common Gateway Interface)</p>\n";
    
    // 获取当前时间
    time_t now = time(0);
    char* timeStr = ctime(&now);
    std::cout << "            <p><strong>服务器时间：</strong>" << timeStr << "</p>\n";
    
    // 显示环境变量信息
    char* method = getenv("REQUEST_METHOD");
    char* uri = getenv("REQUEST_URI");
    char* userAgent = getenv("HTTP_USER_AGENT");
    char* remoteAddr = getenv("REMOTE_ADDR");
    
    std::cout << "            <h3>请求信息：</h3>\n";
    if (method) {
        std::cout << "            <p><strong>请求方法：</strong>" << method << "</p>\n";
    }
    if (uri) {
        std::cout << "            <p><strong>请求URI：</strong>" << uri << "</p>\n";
    }
    if (userAgent) {
        std::cout << "            <p><strong>用户代理：</strong>" << userAgent << "</p>\n";
    }
    if (remoteAddr) {
        std::cout << "            <p><strong>客户端IP：</strong>" << remoteAddr << "</p>\n";
    }
    
    std::cout << "        </div>\n";
    std::cout << "        <p style=\"text-align: center;\">\n";
    std::cout << "            <a href=\"/\">返回主页</a> | \n";
    std::cout << "            <a href=\"/form.html\">测试表单</a>\n";
    std::cout << "        </p>\n";
    std::cout << "    </div>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";
    
    return 0;
}
