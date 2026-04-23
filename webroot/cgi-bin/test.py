#!/usr/bin/env python3
"""
test.py — CGI Diagnostic Script
Displays server environment, request headers, and CGI variables.
Access via: http://yourserver/cgi-bin/test.py
"""

import cgi
import cgitb
import os
import sys
import platform
import datetime

# Enable detailed error reporting in browser
cgitb.enable()

# ── OUTPUT HTML ───────────────────────────────────────────────────────────────

print("Content-Type: text/html; charset=utf-8")
print()  # Required blank line between headers and body

now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

print(f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>CGI Test – Server Diagnostic</title>
  <link rel="stylesheet" href="../assets/style.css">
  <style>
    table {{ width: 100%; border-collapse: collapse; font-size: .9rem; }}
    th    {{ background: var(--navy, #000080); color: #fff; text-align: left;
             font-family: 'Syne', sans-serif; font-size: .75rem;
             letter-spacing: .1em; text-transform: uppercase; padding: 10px 14px; }}
    td    {{ padding: 9px 14px; border-bottom: 1px solid #ddd; vertical-align: top; }}
    tr:nth-child(even) td {{ background: #F8F8F2; }}
    td:first-child {{ font-family: 'Courier New', monospace; font-size: .85rem;
                      color: #000080; width: 38%; font-weight: bold; }}
    td:last-child  {{ font-family: 'Courier New', monospace; font-size: .85rem;
                      word-break: break-all; }}
    .section-wrap {{ background: #fff; border: 2px solid #ddd;
                     border-top: 5px solid var(--blue, #0000CC);
                     box-shadow: 6px 6px 0 #000080; margin-bottom: 2rem; }}
    .ok   {{ color: #008000; font-weight: bold; }}
    .warn {{ color: #FF6600; font-weight: bold; }}
    .err  {{ color: #CC0000; font-weight: bold; }}
  </style>
</head>
<body>

  <header>
    <a href="../index.html" class="logo-wrap">
      <div class="logo-icon">MB</div>
      <span class="logo-text">MyBlog</span>
    </a>
    <nav>
      <a href="../index.html">Home</a>
      <a href="../blog/post1.html">Blog</a>
      <a href="../forms/contact_form.html">Contact</a>
    </nav>
  </header>

  <main class="container">

    <div class="page-title-block">
      <h1>CGI Diagnostic</h1>
      <p>Server environment and request inspection &nbsp;·&nbsp; {now}</p>
    </div>

    <!-- STATUS STRIP -->
    <div style="display:flex; gap:1px; background:#ddd; border:1px solid #ddd; margin-bottom:2.5rem;">
      <div style="flex:1; background:#008000; color:#fff; padding:1rem; text-align:center;">
        <div style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem;">✓ CGI</div>
        <div style="font-size:.75rem; opacity:.8; margin-top:2px;">Running</div>
      </div>
      <div style="flex:1; background:#0000CC; color:#fff; padding:1rem; text-align:center;">
        <div style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem;">Python {sys.version_info.major}.{sys.version_info.minor}</div>
        <div style="font-size:.75rem; opacity:.8; margin-top:2px;">Interpreter</div>
      </div>
      <div style="flex:1; background:#800080; color:#fff; padding:1rem; text-align:center;">
        <div style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem;">{platform.system()}</div>
        <div style="font-size:.75rem; opacity:.8; margin-top:2px;">OS</div>
      </div>
      <div style="flex:1; background:#FF6600; color:#fff; padding:1rem; text-align:center;">
        <div style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem;">{os.environ.get('REQUEST_METHOD', 'N/A')}</div>
        <div style="font-size:.75rem; opacity:.8; margin-top:2px;">Method</div>
      </div>
    </div>
""")

# ── HELPER: render a table section ───────────────────────────────────────────

def render_table(title, rows, color="#0000CC"):
    print(f'<div class="section-wrap" style="border-top-color:{color};">')
    print(f'<table><thead><tr><th colspan="2">{title}</th></tr></thead><tbody>')
    if rows:
        for k, v in rows:
            v_safe = str(v).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
            print(f"<tr><td>{k}</td><td>{v_safe if v_safe else '<em style=\"color:#aaa;\">—</em>'}</td></tr>")
    else:
        print('<tr><td colspan="2" style="color:#aaa; font-style:italic;">No data</td></tr>')
    print("</tbody></table></div>")

# ── PYTHON INFO ───────────────────────────────────────────────────────────────

python_info = [
    ("sys.version",        sys.version),
    ("sys.executable",     sys.executable),
    ("sys.platform",       sys.platform),
    ("platform.node()",    platform.node()),
    ("platform.release()", platform.release()),
    ("os.getcwd()",        os.getcwd()),
    ("__file__",           os.path.abspath(__file__) if '__file__' in dir() else 'N/A'),
    ("Script PID",         str(os.getpid())),
    ("Current Time (UTC)", datetime.datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")),
]
render_table("Python &amp; System Info", python_info, "#800080")

# ── CGI / REQUEST VARIABLES ───────────────────────────────────────────────────

cgi_keys = [
    "REQUEST_METHOD", "QUERY_STRING", "CONTENT_TYPE", "CONTENT_LENGTH",
    "PATH_INFO", "PATH_TRANSLATED", "SCRIPT_NAME", "SCRIPT_FILENAME",
    "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL", "SERVER_SOFTWARE",
    "DOCUMENT_ROOT", "GATEWAY_INTERFACE", "REDIRECT_STATUS",
]

cgi_rows = [(k, os.environ.get(k, "")) for k in cgi_keys]
render_table("CGI / Request Variables", cgi_rows, "#0000CC")

# ── HTTP HEADERS ──────────────────────────────────────────────────────────────

header_rows = [
    (k, v) for k, v in sorted(os.environ.items())
    if k.startswith("HTTP_")
]
render_table("HTTP Request Headers", header_rows, "#CC0000")

# ── GET PARAMETERS ────────────────────────────────────────────────────────────

form = cgi.FieldStorage()
get_rows = []
for key in form.keys():
    val = form.getlist(key)
    get_rows.append((key, ", ".join(val)))

render_table("GET / POST Parameters", get_rows, "#FF6600")

# ── FULL ENVIRONMENT ──────────────────────────────────────────────────────────

env_rows = [(k, v) for k, v in sorted(os.environ.items())]
render_table("Full Environment Variables", env_rows, "#008080")

# ── FOOTER ────────────────────────────────────────────────────────────────────

print("""
  </main>

  <footer>
    <p>CGI Diagnostic &nbsp;·&nbsp; <a href="../index.html">Back to Site</a></p>
  </footer>

</body>
</html>
""")
