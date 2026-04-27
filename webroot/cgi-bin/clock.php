#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=utf-8");
$time = date("H:i:s");
$date = date("l, F j, Y");
?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Server Clock</title>
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

  <main class="narrow" style="text-align:center; padding-top:4rem;">

    <p style="font-family:'Syne',sans-serif; font-size:.8rem; letter-spacing:.2em;
              text-transform:uppercase; color:#808080; margin-bottom:.5rem;">
      Server Time at Execution
    </p>

    <div style="font-family:'Syne',sans-serif; font-weight:800;
                font-size:5rem; color:#000080; letter-spacing:4px; line-height:1;">
      <?= $time ?>
    </div>

    <div style="font-family:'Syne',sans-serif; font-size:1rem;
                color:#555; margin-top:1rem;">
      <?= $date ?>
    </div>

    <div style="height:5px; width:120px; margin:2rem auto;
                background: repeating-linear-gradient(
                  90deg,
                  #CC0000 0 20px, #FFCC00 20px 40px,
                  #0000CC 40px 60px, #008000 60px 80px,
                  #800080 80px 100px, #FF6600 100px 120px
                );">
    </div>

    <a href="clock.php" class="btn">↻ Refresh</a>

  </main>

  <footer>
    <p>© 2025 MyBlog &nbsp;·&nbsp; <a href="../index.html">Home</a></p>
  </footer>

</body>
</html>
