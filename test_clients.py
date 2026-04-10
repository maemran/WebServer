#!/usr/bin/env python3
"""
Multi-client concurrent test for the webserver.
Usage: python3 test_clients.py [--host 127.0.0.1] [--port 8080] [--clients 10] [--requests 5]
"""

import socket
import threading
import time
import argparse
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed

# ── colour helpers ────────────────────────────────────────────────────────────
GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
CYAN   = "\033[96m"
RESET  = "\033[0m"

def ok(msg):   print(f"{GREEN}[PASS]{RESET} {msg}")
def fail(msg): print(f"{RED}[FAIL]{RESET} {msg}")
def info(msg): print(f"{CYAN}[INFO]{RESET} {msg}")
def warn(msg): print(f"{YELLOW}[WARN]{RESET} {msg}")

# ── raw HTTP helpers ──────────────────────────────────────────────────────────

def raw_request(host, port, request_bytes, timeout=5):
    """Open a TCP connection, send raw bytes, read response."""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(timeout)
    try:
        s.connect((host, port))
        s.sendall(request_bytes)
        chunks = []
        while True:
            try:
                chunk = s.recv(4096)
                if not chunk:
                    break
                chunks.append(chunk)
            except socket.timeout:
                break
        return b"".join(chunks)
    finally:
        s.close()

def make_get(host, path="/"):
    return (
        f"GET {path} HTTP/1.0\r\n"
        f"Host: {host}\r\n"
        f"Connection: close\r\n"
        f"\r\n"
    ).encode()

def make_head(host, path="/"):
    return (
        f"HEAD {path} HTTP/1.0\r\n"
        f"Host: {host}\r\n"
        f"Connection: close\r\n"
        f"\r\n"
    ).encode()

def make_post(host, path="/", body="hello=world"):
    body_bytes = body.encode()
    return (
        f"POST {path} HTTP/1.0\r\n"
        f"Host: {host}\r\n"
        f"Content-Type: application/x-www-form-urlencoded\r\n"
        f"Content-Length: {len(body_bytes)}\r\n"
        f"Connection: close\r\n"
        f"\r\n"
    ).encode() + body_bytes

def status_code(response: bytes) -> int:
    try:
        first_line = response.split(b"\r\n", 1)[0].decode()
        return int(first_line.split()[1])
    except Exception:
        return -1

# ── individual test cases ─────────────────────────────────────────────────────

TESTS = [
    ("GET /",           lambda h, p: make_get(h, "/")),
    ("GET /index.html", lambda h, p: make_get(h, "/index.html")),
    ("GET /images/",    lambda h, p: make_get(h, "/images/")),
    ("HEAD /",          lambda h, p: make_head(h, "/")),
    ("GET /missing404", lambda h, p: make_get(h, "/this_path_does_not_exist")),
    ("POST /",          lambda h, p: make_post(h, "/")),
]

def run_test(client_id, test_name, req_bytes, host, port):
    t0 = time.time()
    try:
        response = raw_request(host, port, req_bytes)
        elapsed = (time.time() - t0) * 1000
        code = status_code(response)
        label = f"client#{client_id:02d}  {test_name:<22} → HTTP {code}  ({elapsed:.1f} ms)"
        if len(response) == 0:
            fail(label + "  [empty response]")
            return False
        elif code in (200, 201, 301, 302, 304):
            ok(label)
        elif code == 404:
            warn(label)
        elif code == 405:
            warn(label + "  [method not allowed]")
        else:
            fail(label)
        return True
    except Exception as e:
        elapsed = (time.time() - t0) * 1000
        fail(f"client#{client_id:02d}  {test_name:<22} → EXCEPTION after {elapsed:.1f} ms: {e}")
        return False

# ── main ──────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Multi-client webserver tester")
    parser.add_argument("--host",     default="127.0.0.1")
    parser.add_argument("--port",     type=int, default=8080)
    parser.add_argument("--clients",  type=int, default=10,
                        help="Number of concurrent clients")
    parser.add_argument("--requests", type=int, default=1,
                        help="How many full test rounds each client runs")
    args = parser.parse_args()

    info(f"Target: {args.host}:{args.port}")
    info(f"Clients: {args.clients}  |  Rounds per client: {args.requests}")
    info(f"Tests per round: {len(TESTS)}")
    print()

    tasks = []
    for client_id in range(1, args.clients + 1):
        for _round in range(args.requests):
            for test_name, req_fn in TESTS:
                req_bytes = req_fn(args.host, args.port)
                tasks.append((client_id, test_name, req_bytes))

    passed = failed = 0
    t_start = time.time()

    with ThreadPoolExecutor(max_workers=args.clients) as pool:
        futures = {
            pool.submit(run_test, cid, name, req, args.host, args.port): (cid, name)
            for cid, name, req in tasks
        }
        for f in as_completed(futures):
            if f.result():
                passed += 1
            else:
                failed += 1

    elapsed = time.time() - t_start
    total = passed + failed
    print()
    info(f"Done in {elapsed:.2f}s  —  {total} requests  |  "
         f"{GREEN}{passed} passed{RESET}  |  "
         f"{RED}{failed} failed{RESET}")

    sys.exit(0 if failed == 0 else 1)

if __name__ == "__main__":
    main()
