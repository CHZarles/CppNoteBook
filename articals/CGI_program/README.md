# CGI Program Demo

这是一个使用 C++ 实现的 CGI 程序示例，配合 Nginx 运行。

## 项目结构

```
CGI_program/
├── README.md           # 项目说明文档
├── TODO.md            # 任务清单
├── NOTE.md            # 学习笔记
├── Makefile           # 构建文件
├── src/               # 源代码目录
│   ├── hello_cgi.cpp  # 主 CGI 程序
│   └── form_cgi.cpp   # 表单处理 CGI 程序
├── bin/               # 编译后的可执行文件
├── html/              # 静态网页文件
│   ├── index.html     # 主页
│   └── form.html      # 表单页面
└── config/            # 配置文件
    └── nginx.conf     # Nginx 配置示例
```

## 环境要求

- Linux 系统
- GCC 编译器
- Nginx Web 服务器

## 安装和运行

### 1. 编译 CGI 程序

```bash
make
```

### 2. 安装 Nginx

在 Ubuntu/Debian 系统上：
```bash
sudo apt update
sudo apt install nginx
```

在 CentOS/RHEL 系统上：
```bash
sudo yum install nginx
```

### 3. 安装 fcgiwrap (CGI 支持)

```bash
sudo apt install fcgiwrap
sudo systemctl start fcgiwrap
sudo systemctl enable fcgiwrap
```

### 4. 配置 Nginx

将 `config/nginx.conf` 中的配置添加到你的 Nginx 配置文件中，或者使用以下快速配置：

```bash
# 备份原配置
sudo cp /etc/nginx/sites-available/default /etc/nginx/sites-available/default.bak

# 应用 CGI 配置
sudo tee /etc/nginx/sites-available/default > /dev/null << 'EOF'
server {
    listen 80 default_server;
    listen [::]:80 default_server;
    root /var/www/html;
    index index.html index.htm;
    server_name _;

    location / {
        try_files $uri $uri/ =404;
    }

    location /cgi-bin/ {
        gzip off;
        root /usr/lib;
        fastcgi_pass unix:/var/run/fcgiwrap.socket;
        include /etc/nginx/fastcgi_params;
        fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
        fastcgi_param PATH_INFO $fastcgi_path_info;
    }
}
EOF

# 测试配置并重载
sudo nginx -t && sudo systemctl reload nginx
```

### 5. 部署文件

```bash
# 部署 CGI 程序和 HTML 文件
make install
```

或者手动部署：

```bash
# 创建 CGI 目录
sudo mkdir -p /usr/lib/cgi-bin

# 复制 CGI 程序
sudo cp bin/* /usr/lib/cgi-bin/
sudo chmod +x /usr/lib/cgi-bin/*

# 复制 HTML 文件
sudo cp html/* /var/www/html/
```

### 6. 启动 Nginx

```bash
sudo systemctl start nginx
sudo systemctl enable nginx
```

### 6. 启动 Nginx

```bash
sudo systemctl start nginx
sudo systemctl enable nginx
```

### 7. 测试

访问 `http://localhost` 查看主页。

## 故障排除

### 404 错误调试步骤

如果遇到 404 错误，按以下步骤调试：

1. **检查服务状态**：
   ```bash
   sudo systemctl status nginx fcgiwrap
   ```

2. **检查文件权限**：
   ```bash
   ls -la /usr/lib/cgi-bin/
   ls -la /var/www/html/
   ```

3. **查看错误日志**：
   ```bash
   sudo tail -f /var/log/nginx/error.log
   ```

4. **测试 CGI 程序**：
   ```bash
   # 直接测试 CGI 程序
   curl -v http://localhost/cgi-bin/hello_cgi
   
   # 测试表单提交
   curl -X POST -d "name=test&email=test@example.com" http://localhost/cgi-bin/form_cgi
   ```

5. **检查 Nginx 配置**：
   ```bash
   sudo nginx -t
   ```

### 常见问题

- **fcgiwrap 未安装**：运行 `sudo apt install fcgiwrap`
- **权限问题**：确保 CGI 程序有执行权限 `chmod +x /usr/lib/cgi-bin/*`
- **配置错误**：检查 Nginx 配置中的 `/cgi-bin/` 位置块

## 功能特性

- 简单的 Hello World CGI 程序
- 表单数据处理
- GET 和 POST 请求支持
- 错误处理和日志记录

## 清理

```bash
make clean
```
