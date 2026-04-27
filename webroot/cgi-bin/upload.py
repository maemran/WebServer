#!/usr/bin/env python3
"""
upload.py — CGI File Upload Handler
Receives multipart/form-data from upload_form.html,
validates the file, saves it, and returns an HTML response.

Expected form fields:
  uploader    — uploader's name (text)
  email       — uploader's email (text, optional)
  description — file description (text, optional)
  file        — the uploaded file (file input)
"""

import cgi
import cgitb
import os
import sys
import datetime
import hashlib
import mimetypes

cgitb.enable()

# ── CONFIGURATION ─────────────────────────────────────────────────────────────

# Directory where uploaded files will be saved (relative to this script)
SCRIPT_DIR   = os.path.dirname(os.path.abspath(__file__))
UPLOAD_DIR   = os.path.join(SCRIPT_DIR, "..", "uploads")
MAX_SIZE     = 1024 * 1024 * 1024   # 10 MB in bytes

ALLOWED_EXTENSIONS = {
  ".jpg", ".jpeg", ".png", ".pdf", ".zip",
  ".mp4", ".mov", ".avi", ".mkv", ".webm", ".m4a",
}
ALLOWED_MIMETYPES  = {
    "image/jpeg", "image/png",
    "application/pdf", "application/zip",
    "application/x-zip-compressed",
  "video/mp4", "video/quicktime", "video/x-msvideo",
  "video/x-matroska", "video/webm",
  "audio/mp4", "audio/x-m4a",
}

# ── HELPERS ───────────────────────────────────────────────────────────────────

def sanitize_filename(name):
    """Strip path components and replace unsafe characters."""
    name = os.path.basename(name)
    safe = "".join(c if (c.isalnum() or c in "._- ") else "_" for c in name)
    return safe.strip() or "upload"

def unique_filename(original):
    """Prefix filename with timestamp + short hash to avoid collisions."""
    ts    = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    short = hashlib.md5(original.encode()).hexdigest()[:6]
    name, ext = os.path.splitext(original)
    return f"{ts}_{short}_{name}{ext}"

def human_size(n):
    for unit in ("B", "KB", "MB"):
        if n < 1024:
            return f"{n:.1f} {unit}"
        n /= 1024
    return f"{n:.1f} GB"

def html_page(title, body_html, success=True):
    """Wrap content in the site's shared layout."""
    accent = "#008000" if success else "#CC0000"
    border_color = accent
    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print("Content-Type: text/html; charset=utf-8")
    print()
    print(f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>{title} – MyBlog</title>
  <link rel="stylesheet" href="../assets/style.css">
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

  <main class="narrow">
    <div class="page-title-block" style="border-left-color:{border_color};">
      <h1>{title}</h1>
      <p style="color:#888; font-size:.85rem;">{now}</p>
    </div>
    {body_html}
  </main>

  <footer>
    <p>© 2025 MyBlog &nbsp;·&nbsp; <a href="../index.html">Home</a></p>
  </footer>

</body>
</html>""")

def error_page(message, detail=""):
    detail_html = f'<p style="font-family:monospace; font-size:.85rem; color:#555; margin-top:.75rem;">{detail}</p>' if detail else ""
    body = f"""
    <div style="background:#FFF0F0; border:2px solid #CC0000; border-top:6px solid #CC0000; padding:2rem; box-shadow:6px 6px 0 #800000;">
      <p style="font-family:'Syne',sans-serif; font-size:1.1rem; font-weight:700; color:#CC0000;">Upload Failed</p>
      <p style="margin-top:.75rem; color:#333;">{message}</p>
      {detail_html}
    </div>
    <div style="margin-top:1.5rem; display:flex; gap:1rem;">
      <a href="../forms/upload_form.html" class="btn">← Try Again</a>
      <a href="../forms/contact_form.html" class="btn btn-secondary">Contact Us</a>
    </div>
    """
    html_page("Upload Failed", body, success=False)

def success_page(info):
    rows = "".join(
        f'<tr><td style="font-family:monospace; color:#000080; width:40%; padding:9px 14px; border-bottom:1px solid #eee; font-weight:bold;">{k}</td>'
        f'<td style="padding:9px 14px; border-bottom:1px solid #eee; font-family:monospace; font-size:.9rem;">{v}</td></tr>'
        for k, v in info.items()
    )
    body = f"""
    <!-- Success banner -->
    <div style="background:#F0FFF0; border:2px solid #008000; border-top:6px solid #008000; padding:2rem; box-shadow:6px 6px 0 #004000; margin-bottom:2rem;">
      <div style="display:flex; align-items:center; gap:12px; margin-bottom:.5rem;">
        <div style="width:36px; height:36px; background:#008000; border-radius:50%; display:flex; align-items:center; justify-content:center; color:#fff; font-size:1.2rem; flex-shrink:0;">✓</div>
        <p style="font-family:'Syne',sans-serif; font-size:1.2rem; font-weight:800; color:#008000;">File Uploaded Successfully</p>
      </div>
      <p style="color:#2D5A2D; font-size:.95rem;">Your file has been received and saved securely.</p>
    </div>

    <!-- File details table -->
    <div style="background:#fff; border:2px solid #ddd; margin-bottom:2rem;">
      <table style="width:100%; border-collapse:collapse;">
        <thead>
          <tr>
            <th colspan="2" style="background:#000080; color:#fff; text-align:left; font-family:'Syne',sans-serif; font-size:.75rem; letter-spacing:.1em; text-transform:uppercase; padding:10px 14px;">
              Upload Details
            </th>
          </tr>
        </thead>
        <tbody>{rows}</tbody>
      </table>
    </div>

    <div style="display:flex; gap:1rem; flex-wrap:wrap;">
      <a href="../forms/upload_form.html" class="btn">Upload Another</a>
      <a href="../index.html" class="btn btn-secondary">← Back to Home</a>
    </div>
    """
    html_page("Upload Successful", body, success=True)

# ── MAIN HANDLER ──────────────────────────────────────────────────────────────

def handle_upload():
    # Only accept POST requests
    method = os.environ.get("REQUEST_METHOD", "GET").upper()
    if method != "POST":
        error_page(
            "This endpoint only accepts POST requests.",
            f"Received: {method}"
        )
        return

    # Parse POST form data (multipart or urlencoded)
    form = cgi.FieldStorage(
        fp=sys.stdin.buffer,
        environ=os.environ,
        keep_blank_values=True
    )

    # Common metadata
    uploader    = form.getfirst("uploader",    "Anonymous").strip()
    email       = form.getfirst("email",       "").strip()
    description = form.getfirst("description", "").strip()

    # ── TEXT submission mode (new upload_form.html) ─────────────────────────
    text_value = form.getfirst("text", "")
    if text_value is not None and text_value != "":
        text_bytes = text_value.encode("utf-8")
        text_size = len(text_bytes)

        if text_size == 0:
            error_page("The submitted text is empty.")
            return

        if text_size > MAX_SIZE:
            error_page(
                f"Submitted text is too large ({human_size(text_size)}).",
                f"Maximum allowed size: {human_size(MAX_SIZE)}"
            )
            return

        os.makedirs(UPLOAD_DIR, exist_ok=True)

        original_name = "submission.txt"
        saved_name = unique_filename(original_name)
        save_path = os.path.join(UPLOAD_DIR, saved_name)

        md5_hash = hashlib.md5(text_bytes).hexdigest()
        sha256 = hashlib.sha256(text_bytes).hexdigest()

        try:
            with open(save_path, "w", encoding="utf-8") as f:
                f.write(text_value)
        except OSError as e:
            error_page("Could not save the text on the server.", str(e))
            return

        log_path = os.path.join(UPLOAD_DIR, "upload_log.txt")
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log_line = (
            f"{timestamp} | {uploader} | {email} | "
            f"TEXT -> {saved_name} | "
            f"{human_size(text_size)} | MD5:{md5_hash[:12]}\n"
        )
        try:
            with open(log_path, "a", encoding="utf-8") as log:
                log.write(log_line)
        except OSError:
            pass

        success_page({
            "Submission type":   "Text",
            "Saved as":          saved_name,
            "Size":              human_size(text_size),
            "MD5 checksum":      md5_hash,
            "SHA-256 (short)":   sha256[:32] + "…",
            "Uploaded by":       uploader,
            "Email":             email or "—",
            "Description":       description or "—",
            "Saved at":          save_path,
            "Timestamp":         timestamp,
        })
        return

    # ── Validate file field ───────────────────────────────────────────────────
    if "file" not in form:
        error_page("No text content was submitted.", "Expected form field: text")
        return

    file_item = form["file"]

    if not file_item.filename:
        error_page("No file was selected. Please choose a file before submitting.")
        return

    original_name = sanitize_filename(file_item.filename)
    _, ext = os.path.splitext(original_name.lower())

    # ── Validate extension ────────────────────────────────────────────────────
    if ext not in ALLOWED_EXTENSIONS:
        error_page(
            f"File type <strong>{ext}</strong> is not allowed.",
            f"Allowed types: {', '.join(sorted(ALLOWED_EXTENSIONS))}"
        )
        return

    # ── Read file data ────────────────────────────────────────────────────────
    file_data = file_item.file.read()
    file_size = len(file_data)

    # ── Validate size ─────────────────────────────────────────────────────────
    if file_size == 0:
        error_page("The uploaded file is empty.")
        return

    if file_size > MAX_SIZE:
        error_page(
            f"File is too large ({human_size(file_size)}).",
            f"Maximum allowed size: {human_size(MAX_SIZE)}"
        )
        return

    # ── Compute checksum ──────────────────────────────────────────────────────
    md5_hash = hashlib.md5(file_data).hexdigest()
    sha256   = hashlib.sha256(file_data).hexdigest()

    # ── Ensure upload directory exists ────────────────────────────────────────
    os.makedirs(UPLOAD_DIR, exist_ok=True)

    # ── Save file ─────────────────────────────────────────────────────────────
    saved_name = unique_filename(original_name)
    save_path  = os.path.join(UPLOAD_DIR, saved_name)

    try:
        with open(save_path, "wb") as f:
            f.write(file_data)
    except OSError as e:
        error_page("Could not save the file on the server.", str(e))
        return

    # ── Append log entry ──────────────────────────────────────────────────────
    log_path = os.path.join(UPLOAD_DIR, "upload_log.txt")
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_line = (
        f"{timestamp} | {uploader} | {email} | "
        f"{original_name} -> {saved_name} | "
        f"{human_size(file_size)} | MD5:{md5_hash[:12]}\n"
    )
    try:
        with open(log_path, "a", encoding="utf-8") as log:
            log.write(log_line)
    except OSError:
        pass  # Logging failure should not block the response

    # ── Detect MIME type ──────────────────────────────────────────────────────
    mime, _ = mimetypes.guess_type(original_name)
    mime = mime or "application/octet-stream"

    # ── Return success page ───────────────────────────────────────────────────
    success_page({
        "Original filename": original_name,
        "Saved as":          saved_name,
        "Size":              human_size(file_size),
        "Type (MIME)":       mime,
        "MD5 checksum":      md5_hash,
        "SHA-256 (short)":   sha256[:32] + "…",
        "Uploaded by":       uploader,
        "Email":             email or "—",
        "Description":       description or "—",
        "Saved at":          save_path,
        "Timestamp":         timestamp,
    })

# ── ENTRY POINT ───────────────────────────────────────────────────────────────

if __name__ == "__main__":
    handle_upload()
