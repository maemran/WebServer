#!/usr/bin/env python3
# Simple CGI test script

import os
import sys

# Read environment
method = os.environ.get('REQUEST_METHOD', 'GET')
script = os.environ.get('SCRIPT_FILENAME', 'unknown')
uri = os.environ.get('REQUEST_URI', '/')
content_length = int(os.environ.get('CONTENT_LENGTH', 0))

# Read body if POST
body = ""
if content_length > 0:
    body = sys.stdin.read(content_length)

# Build HTML response
html = """<html>
<head><title>CGI Test</title></head>
<body style="font-family: Arial; margin: 20px;">
<h1 style="color: #333;">✅ CGI Script Executed Successfully!</h1>

<div style="background: #f0f0f0; padding: 15px; border-radius: 5px;">
  <h2>Request Information:</h2>
  <ul>
    <li><b>HTTP Method:</b> %s</li>
    <li><b>Script Path:</b> %s</li>
    <li><b>URI:</b> %s</li>
    <li><b>Content Length:</b> %d bytes</li>
  </ul>
</div>

<div style="background: #e8f5e9; padding: 15px; margin-top: 15px; border-radius: 5px;">
  <h2>Available Environment Variables:</h2>
  <ul>
    <li>REQUEST_METHOD (Method): %s</li>
    <li>SCRIPT_FILENAME (Script): %s</li>
    <li>REQUEST_URI (URI): %s</li>
  </ul>
</div>
""" % (method, script, uri, content_length, method, script, uri

if body:
    html += """<div style="background: #fff3e0; padding: 15px; margin-top: 15px; border-radius: 5px;">
  <h2>POST Body:</h2>
  <pre style="background: white; padding: 10px; border-left: 3px solid orange;">%s</pre>
</div>
""" % body

html += """</body>
</html>"""

# Output HTTP headers and body
print("Content-Type: text/html")
print("Content-Length: %d" % len(html))
print("")
print(html)
