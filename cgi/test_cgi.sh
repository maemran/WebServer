#!/bin/bash

# Test script for multiple CGI types

echo "=========================================="
echo "   Testing Multiple CGI Types"
echo "=========================================="
echo ""

cd /home/saabo-sh/webserveMyPart

# Test 1: Python CGI
echo "1️⃣  Testing Python CGI Script (.py)"
echo "===================="
echo "Script: ./www/cgi/hello.py"
./www/cgi/hello.py
echo ""
echo ""

# Test 2: Bash CGI
echo "2️⃣  Testing Bash CGI Script (.sh)"
echo "===================="
echo "Script: ./www/cgi/hello.sh"
./www/cgi/hello.sh
echo ""
echo ""

# Test 3: Perl CGI
echo "3️⃣  Testing Perl CGI Script (.pl)"
echo "===================="
echo "Script: ./www/cgi/hello.pl"
./www/cgi/hello.pl
echo ""
echo ""

echo "=========================================="
echo "   ✅ All CGI Scripts Executed Successfully!"
echo "=========================================="
