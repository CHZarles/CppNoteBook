# CGI 程序项目 Makefile

CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2
SRCDIR = src
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
TARGETS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%,$(SOURCES))

.PHONY: all clean install

all: $(BINDIR) $(TARGETS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

install: all
	@echo "Installing CGI programs..."
	sudo mkdir -p /usr/lib/cgi-bin
	sudo cp $(BINDIR)/* /usr/lib/cgi-bin/
	sudo chmod +x /usr/lib/cgi-bin/*
	@echo "Installing HTML files..."
	sudo cp html/* /var/www/html/
	@echo "Installation complete!"

clean:
	rm -rf $(BINDIR)

help:
	@echo "Available targets:"
	@echo "  all     - Build all CGI programs"
	@echo "  install - Install CGI programs and HTML files"
	@echo "  clean   - Remove compiled files"
	@echo "  help    - Show this help message"
