#!/usr/bin/env python3
"""
contact.py — CGI Contact Form Handler
Processes POST data from forms/contact_form.html.

Fields received:
  fname    — first name (required)
  lname    — last name  (required)
  email    — sender's email address (required)
  subject  — message subject (optional)
  message  — message body (required)
  topic    — checkbox list: webdev, css, collab, other (optional, multiple)

What this script does:
  1. Validates all required fields
  2. Sanitizes input (strips HTML, trims whitespace)
  3. Sends an email notification to the site owner  ← configure SETTINGS below
  4. Saves the submission as a text file in uploads/
  5. Returns a styled HTML success or error page
"""

import cgi
import cgitb
import os
import sys
import re
import datetime
import hashlib
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from html import escape

cgitb.enable()

# ══════════════════════════════════════════════════════════════════════════════
#  SETTINGS  —  edit these before deploying
# ══════════════════════════════════════════════════════════════════════════════

SITE_NAME       = "MyBlog"
OWNER_EMAIL     = "hello@myblog.io"          # where notifications are sent TO
FROM_EMAIL      = "noreply@myblog.io"        # the From: address on emails
SMTP_HOST       = "localhost"                # SMTP server (e.g. smtp.gmail.com)
SMTP_PORT       = 25                         # 25=plain, 587=STARTTLS, 465=SSL
SMTP_USE_TLS    = False                      # True for port 587 (STARTTLS)
SMTP_USE_SSL    = False                      # True for port 465 (SSL)
SMTP_USERNAME   = ""                         # leave empty if no auth needed
SMTP_PASSWORD   = ""                         # leave empty if no auth needed
SEND_EMAIL      = False                      # set True once SMTP is configured

SCRIPT_DIR  = os.path.dirname(os.path.abspath(__file__))
UPLOAD_DIR  = os.path.join(SCRIPT_DIR, "..", "uploads")
MAX_MESSAGE = 5000   # character limit for the message field

# ══════════════════════════════════════════════════════════════════════════════
#  VALIDATION HELPERS
# ══════════════════════════════════════════════════════════════════════════════

EMAIL_RE = re.compile(r"^[^@\s]+@[^@\s]+\.[^@\s]+$")

def is_valid_email(addr):
    return bool(EMAIL_RE.match(addr))

def sanitize(value, max_len=500):
    """Strip leading/trailing whitespace and truncate."""
    return str(value).strip()[:max_len]

def strip_html(text):
    """Remove any HTML tags from user input."""
    return re.sub(r"<[^>]+>", "", text)

def validate(fields):
    """
    Returns a list of error strings.
    Empty list means all fields are valid.
    """
    errors = []
    if not fields["fname"]:
        errors.append("First name is required.")
    if not fields["lname"]:
        errors.append("Last name is required.")
    if not fields["email"]:
        errors.append("Email address is required.")
    elif not is_valid_email(fields["email"]):
        errors.append(f"'{escape(fields['email'])}' does not look like a valid email address.")
    if not fields["message"]:
        errors.append("Message cannot be empty.")
    elif len(fields["message"]) > MAX_MESSAGE:
        errors.append(f"Message is too long ({len(fields['message'])} chars). Maximum is {MAX_MESSAGE}.")
    return errors

# ══════════════════════════════════════════════════════════════════════════════
#  EMAIL
# ══════════════════════════════════════════════════════════════════════════════

def build_email_body(fields, timestamp):
    topics = ", ".join(fields["topics"]) if fields["topics"] else "—"
    return f"""New contact form submission from {SITE_NAME}
{'=' * 52}

Submitted : {timestamp}
Name      : {fields['fname']} {fields['lname']}
Email     : {fields['email']}
Subject   : {fields['subject'] or '—'}
Topics    : {topics}

Message
-------
{fields['message']}

{'=' * 52}
Reply directly to: {fields['email']}
"""

def send_notification(fields, timestamp):
    """Send an email to the site owner. Returns (success: bool, error: str)."""
    if not SEND_EMAIL:
        return True, ""  # Email disabled — skip silently

    subject = f"[{SITE_NAME}] New message: {fields['subject'] or 'No subject'}"
    body    = build_email_body(fields, timestamp)

    msg = MIMEMultipart("alternative")
    msg["Subject"] = subject
    msg["From"]    = FROM_EMAIL
    msg["To"]      = OWNER_EMAIL
    msg["Reply-To"] = fields["email"]
    msg.attach(MIMEText(body, "plain", "utf-8"))

    try:
        if SMTP_USE_SSL:
            server = smtplib.SMTP_SSL(SMTP_HOST, SMTP_PORT, timeout=10)
        else:
            server = smtplib.SMTP(SMTP_HOST, SMTP_PORT, timeout=10)

        if SMTP_USE_TLS:
            server.starttls()

        if SMTP_USERNAME and SMTP_PASSWORD:
            server.login(SMTP_USERNAME, SMTP_PASSWORD)

        server.sendmail(FROM_EMAIL, [OWNER_EMAIL], msg.as_string())
        server.quit()
        return True, ""

    except smtplib.SMTPException as e:
        return False, str(e)
    except OSError as e:
        return False, str(e)

# ══════════════════════════════════════════════════════════════════════════════
#  LOGGING
# ══════════════════════════════════════════════════════════════════════════════

def log_submission(fields, timestamp):
  """Save one submission per file in the uploads directory."""
  os.makedirs(UPLOAD_DIR, exist_ok=True)
  topics = ", ".join(fields["topics"]) if fields["topics"] else "None"
  payload = (
    f"Timestamp: {timestamp}\n"
    f"Name: {fields['fname']} {fields['lname']}\n"
    f"Email: {fields['email']}\n"
    f"Subject: {fields['subject'] or 'None'}\n"
    f"Topics: {topics}\n"
    f"Message length: {len(fields['message'])} chars\n\n"
    f"Message:\n{fields['message']}\n"
  )
  file_hash = hashlib.md5(payload.encode("utf-8")).hexdigest()[:8]
  safe_stamp = timestamp.replace(" ", "_").replace(":", "-")
  filename = f"contact_{safe_stamp}_{file_hash}.txt"
  save_path = os.path.join(UPLOAD_DIR, filename)
  try:
    with open(save_path, "w", encoding="utf-8") as f:
      f.write(payload)
  except OSError:
    pass  # Save failure must never break the response

# ══════════════════════════════════════════════════════════════════════════════
#  HTML RESPONSE PAGES
# ══════════════════════════════════════════════════════════════════════════════

LAYOUT_TOP = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>{page_title} – {site}</title>
  <link rel="stylesheet" href="../assets/style.css">
</head>
<body>
  <header>
    <a href="../index.html" class="logo-wrap">
      <div class="logo-icon">MB</div>
      <span class="logo-text">{site}</span>
    </a>
    <nav>
      <a href="../index.html">Home</a>
      <a href="../blog/post1.html">Blog</a>
      <a href="../forms/contact_form.html">Contact</a>
    </nav>
  </header>
  <main class="narrow">"""

LAYOUT_BOTTOM = """
  </main>
  <footer>
    <p>© 2025 {site} &nbsp;·&nbsp; <a href="../index.html">Home</a></p>
  </footer>
</body>
</html>"""

def render_page(page_title, body_html):
    top    = LAYOUT_TOP.format(page_title=page_title, site=SITE_NAME)
    bottom = LAYOUT_BOTTOM.format(site=SITE_NAME)
    print("Content-Type: text/html; charset=utf-8")
    print()
    print(top + body_html + bottom)

def success_page(fields, timestamp):
    topics_html = ""
    if fields["topics"]:
        topic_labels = {"webdev": "Web Dev", "css": "CSS", "collab": "Collaboration", "other": "Other"}
        topic_colors = {"webdev": "#0000CC", "css": "#FF6600", "collab": "#008000", "other": "#800080"}
        pills = "".join(
            f'<span style="background:{topic_colors.get(t,"#555")}; color:#fff; '
            f'font-family:\'Syne\',sans-serif; font-size:.7rem; font-weight:700; '
            f'letter-spacing:.1em; text-transform:uppercase; padding:3px 10px;">'
            f'{topic_labels.get(t, t)}</span>'
            for t in fields["topics"]
        )
        topics_html = f'<div style="display:flex; gap:6px; flex-wrap:wrap; margin-top:.5rem;">{pills}</div>'

    body = f"""
    <div style="background:#F0FFF0; border:2px solid #008000; border-top:6px solid #008000;
                padding:2rem; box-shadow:6px 6px 0 #004000; margin-bottom:2rem;">
      <div style="display:flex; align-items:center; gap:12px; margin-bottom:.75rem;">
        <div style="width:44px; height:44px; background:#008000; border-radius:50%;
                    display:flex; align-items:center; justify-content:center;
                    color:#fff; font-size:1.4rem; flex-shrink:0;">✓</div>
        <p style="font-family:'Syne',sans-serif; font-size:1.3rem;
                  font-weight:800; color:#008000; margin:0;">Message Sent!</p>
      </div>
      <p style="color:#2D5A2D;">
        Thanks <strong>{escape(fields['fname'])}</strong>! We received your message
        and will reply to <strong>{escape(fields['email'])}</strong> within 24 hours.
      </p>
    </div>

    <!-- Summary card -->
    <div style="background:#fff; border:2px solid #ddd; margin-bottom:2rem;">
      <table style="width:100%; border-collapse:collapse;">
        <thead>
          <tr>
            <th colspan="2" style="background:#000080; color:#fff; text-align:left;
                font-family:'Syne',sans-serif; font-size:.75rem; letter-spacing:.1em;
                text-transform:uppercase; padding:10px 14px;">
              Submission Summary
            </th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td style="font-family:monospace; color:#000080; width:35%;
                       padding:9px 14px; border-bottom:1px solid #eee; font-weight:bold;">Name</td>
            <td style="padding:9px 14px; border-bottom:1px solid #eee;">
              {escape(fields['fname'])} {escape(fields['lname'])}
            </td>
          </tr>
          <tr>
            <td style="font-family:monospace; color:#000080; padding:9px 14px;
                       border-bottom:1px solid #eee; font-weight:bold;">Email</td>
            <td style="padding:9px 14px; border-bottom:1px solid #eee;">
              {escape(fields['email'])}
            </td>
          </tr>
          <tr>
            <td style="font-family:monospace; color:#000080; padding:9px 14px;
                       border-bottom:1px solid #eee; font-weight:bold;">Subject</td>
            <td style="padding:9px 14px; border-bottom:1px solid #eee;">
              {escape(fields['subject']) or '<em style="color:#aaa;">No subject</em>'}
            </td>
          </tr>
          <tr>
            <td style="font-family:monospace; color:#000080; padding:9px 14px;
                       border-bottom:1px solid #eee; font-weight:bold;">Topics</td>
            <td style="padding:9px 14px; border-bottom:1px solid #eee;">
              {topics_html or '<em style="color:#aaa;">None selected</em>'}
            </td>
          </tr>
          <tr>
            <td style="font-family:monospace; color:#000080; padding:9px 14px;
                       border-bottom:1px solid #eee; font-weight:bold;">Message</td>
            <td style="padding:9px 14px; border-bottom:1px solid #eee;
                       font-size:.9rem; white-space:pre-wrap; color:#333;">
              {escape(fields['message'][:300])}{'…' if len(fields['message']) > 300 else ''}
            </td>
          </tr>
          <tr>
            <td style="font-family:monospace; color:#000080; padding:9px 14px; font-weight:bold;">Timestamp</td>
            <td style="padding:9px 14px; font-family:monospace; font-size:.85rem;">{timestamp}</td>
          </tr>
        </tbody>
      </table>
    </div>

    <div style="display:flex; gap:1rem; flex-wrap:wrap;">
      <a href="../index.html" class="btn">← Back to Home</a>
      <a href="../blog/post1.html" class="btn btn-secondary">Read the Blog</a>
    </div>
    """
    render_page("Message Sent", body)


def error_page(errors, fields):
    error_items = "".join(
        f'<li style="padding:6px 0; border-bottom:1px solid #FFD0D0; color:#800000;">'
        f'⚠ {e}</li>'
        for e in errors
    )

    # Re-populate form fields so the user doesn't lose their input
    fname   = escape(fields.get("fname",   ""))
    lname   = escape(fields.get("lname",   ""))
    email   = escape(fields.get("email",   ""))
    subject = escape(fields.get("subject", ""))
    message = escape(fields.get("message", ""))

    body = f"""
    <div style="background:#FFF0F0; border:2px solid #CC0000; border-top:6px solid #CC0000;
                padding:1.75rem; box-shadow:6px 6px 0 #800000; margin-bottom:2rem;">
      <p style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem;
                color:#CC0000; margin-bottom:1rem;">
        Please fix the following errors:
      </p>
      <ul style="list-style:none; padding:0; margin:0;">
        {error_items}
      </ul>
    </div>

    <!-- Pre-filled form so the user can correct and resubmit -->
    <form action="/cgi-bin/contact.py" method="POST" class="form-wrapper">
      <div style="display:grid; grid-template-columns:1fr 1fr; gap:1.25rem;">
        <div class="field">
          <label for="fname">First Name</label>
          <input type="text" id="fname" name="fname" value="{fname}" placeholder="Jane" required>
        </div>
        <div class="field">
          <label for="lname">Last Name</label>
          <input type="text" id="lname" name="lname" value="{lname}" placeholder="Doe" required>
        </div>
      </div>
      <div class="field">
        <label for="email">Email Address</label>
        <input type="email" id="email" name="email" value="{email}" placeholder="jane@example.com" required>
      </div>
      <div class="field">
        <label for="subject">Subject</label>
        <input type="text" id="subject" name="subject" value="{subject}" placeholder="What's on your mind?">
      </div>
      <div class="field">
        <label for="message">Message</label>
        <textarea id="message" name="message" rows="6" required>{message}</textarea>
      </div>
      <button type="submit" class="btn">Try Again →</button>
    </form>
    """
    render_page("Submission Error", body)


def bad_method_page():
    body = """
    <div style="background:#FFF0F0; border:2px solid #CC0000; border-top:6px solid #CC0000;
                padding:2rem; box-shadow:6px 6px 0 #800000; margin-bottom:2rem;">
      <p style="font-family:'Syne',sans-serif; font-weight:800; font-size:1.1rem; color:#CC0000;">
        Invalid Request Method
      </p>
      <p style="margin-top:.75rem; color:#333;">
        This endpoint only accepts <code>POST</code> requests submitted via the contact form.
      </p>
    </div>
    <a href="../forms/contact_form.html" class="btn">← Go to Contact Form</a>
    """
    render_page("Invalid Request", body)

# ══════════════════════════════════════════════════════════════════════════════
#  MAIN
# ══════════════════════════════════════════════════════════════════════════════

def handle():
    method = os.environ.get("REQUEST_METHOD", "GET").upper()
    if method != "POST":
        bad_method_page()
        return

    # ── Parse form ────────────────────────────────────────────────────────────
    form = cgi.FieldStorage(
        fp=sys.stdin.buffer,
        environ=os.environ,
        keep_blank_values=True
    )

    fields = {
        "fname":   sanitize(strip_html(form.getfirst("fname",   ""))),
        "lname":   sanitize(strip_html(form.getfirst("lname",   ""))),
        "email":   sanitize(form.getfirst("email",   ""), max_len=254),
        "subject": sanitize(strip_html(form.getfirst("subject", "")), max_len=200),
        "message": sanitize(strip_html(form.getfirst("message", "")), max_len=MAX_MESSAGE),
        "topics":  form.getlist("topic"),   # list of checked values
    }

    # ── Validate ──────────────────────────────────────────────────────────────
    errors = validate(fields)
    if errors:
        error_page(errors, fields)
        return

    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # ── Log ───────────────────────────────────────────────────────────────────
    log_submission(fields, timestamp)

    # ── Email ─────────────────────────────────────────────────────────────────
    email_ok, email_err = send_notification(fields, timestamp)
    # Email failures are silent to the user — the submission still succeeds.
    # Check the log file if notifications stop arriving.

    # ── Respond ───────────────────────────────────────────────────────────────
    success_page(fields, timestamp)


if __name__ == "__main__":
    handle()
