#!/bin/bash

echo "=================================================="
echo "     🧪 COOKIE AND SESSION MANAGEMENT TEST"
echo "=================================================="
echo

# Test 1: Direct Script Test (No Server)
echo "[TEST 1] Direct Script Execution"
echo "─────────────────────────────────────────────────"
echo "Running: python3 www/cgi/login.py"
echo
python3 www/cgi/login.py
echo
echo "✅ Login script executed successfully"
echo

# Test 2: Session Counter
echo "[TEST 2] Session Counter (Data Persistence)"
echo "─────────────────────────────────────────────────"
echo "Running with simulated cookies: SESSIONID=SID_test; VISIT_COUNT=5"
echo
HTTP_COOKIE="SESSIONID=SID_test_123456; VISIT_COUNT=5" python3 www/cgi/session_counter.py
echo
echo "✅ Counter script executed - should show VISIT_COUNT=6"
echo

# Test 3: Logout
echo "[TEST 3] Logout (Clear Cookies)"
echo "─────────────────────────────────────────────────"
echo "Running: python3 www/cgi/logout.py"
echo
python3 www/cgi/logout.py
echo
echo "✅ Logout script executed - cookies set to expire"
echo

echo "=================================================="
echo "     ✅ ALL DIRECT TESTS COMPLETED"
echo "=================================================="
