<html>
<body>

<?php
    $user = $_POST["userName"];
    $stream = $_POST["streamName"];
    $text = $_POST["textArea"];
    $index = $_POST["index"];
    $byName = $_POST["byName"];

    $URL = "streamViewer.php";

    if (file_exists("./post.wpml"))
    {
        $fp = fopen("./post.wpml", "r");
        $wpml = "";

        while (($curr = fgetc($fp)) != EOF)
        {
            if ($curr == '.')
            {
                $curr = fgetc($fp);
                if ($curr == 'd')
                {
                    break;
                }
                else
                {
                    $wpml .= '.';
                }
            }
            $wpml .= $curr;
        }
    }

    $fp = fopen("./post.wpml", "w");
    fwrite($fp, $wpml);
    fwrite($fp, ".d()\n");
    fwrite($fp, ".a(name=\"textArea\", form=\"postForm\", rows=\"15\", cols=\"50\", required)\n");
    fwrite($fp, ".P()\n");
    fclose($fp);

    exec("./parser post.wpml", $html, $status);

    if ($status)
    {
        echo "parser failed to run";
    }
    else
    {
        foreach($html as $key=>$tag)
        {
            if (strncmp($tag, "<textarea", 9) == 0)
            {
                echo $tag;
                echo "</textarea>";
            }
            else
            {
                echo $tag;
            }

        }

        echo "<form action=\"streamViewer.php\", method=\"POST\", id=\"postForm\">\n";
        echo "<input type=\"Submit\" name=\"posting\" value=\"Post\">\n";
        echo "<input type=\"hidden\" name=\"userName\" value=\"$user\">\n";
        echo "<input type=\"hidden\" name=\"stream\" value=\"$stream\">\n";
        echo "<input type=\"hidden\" name=\"index\" value=\"$index\">\n";
        echo "<input type=\"hidden\" name=\"byName\" value=\"$byName\">\n";
        echo "</center>\n";
    }
?>

</body>
</html>
