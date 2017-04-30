<html>
<body>

<?php
    $user = (!empty($_POST["userName"]) ? $_POST["userName"] : null);
    $stream = (!empty($_POST["stream"]) ? $_POST["stream"] : null);
    $index = (!empty($_POST["index"]) ? $_POST["index"] : 0);
    $text = (!empty($_POST["textArea"]) ? $_POST["textArea"] : null);
    $byName = (!empty($_POST["byName"]) ? $_POST["byName"] : null);

    if (isset($_POST["posting"]))
    {
        exec("./post \"$user\" \"$stream\" \"$text\"", $out, $stat);
    }

    if (file_exists("./streamViewer.wpml"))
    {
        $fp = fopen("./streamViewer.wpml", "r");
        $wpml = "";

        while (!feof($fp))
        {
            $curr = fgetc($fp);
            if ($curr == '.')
            {
                $curr = fgetc($fp);
                if ($curr == 'a')
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

    $fp = fopen("./streamViewer.wpml", "w");

    fwrite($fp, $wpml);
    fwrite($fp, ".a(name=\"textArea\", rows=\"20\", cols=\"75\", form=\"post\")\n");
    fwrite($fp, ".b(name=\"Change Author\", link=\"index.php\")\n");
    fwrite($fp, ".b(name=\"Add/Remove Author\", link=\"addauthor.php\")\n");
    fwrite($fp, ".n(link=\"post.php\", label=\"\", input=\"hidden\", name=\"userName\", value=\"$user\",\n");
    fwrite($fp, "label=\"\", input=\"hidden\", name=\"stream\", value=\"$stream\",\n");
    fwrite($fp, "label=\"\", input=\"hidden\", name=\"index\", value=\"$index\",\n");
    fwrite($fp, "label=\"\", input=\"hidden\", name=\"byName\", value=\"$byName\",\n");
    fwrite($fp, "label=\"\", input=\"Submit\", name=\"post\", value=\"Post\")\n");
    fwrite($fp, ".V()\n");
    fclose($fp);

    exec("./parser streamViewer.wpml", $html, $status);

    if ($status)
    {
        echo "parser failed to run";
    }
    else
    {
        foreach($html as $tag)
        {
            if (strncmp($tag, "<textarea", 9) == 0)
            {
                echo "<form action=\"streamViewer.php\", method=\"POST\">\n";
                echo "<input type=\"Submit\", name=\"sort\", value=\"Sort\">\n";
                echo "<input type=\"Submit\", name=\"markAll\", value=\"Mark All\">\n";
                echo "<input type=\"Submit\", name=\"checkNew\", value=\"Check New\">\n";
                echo "<input type=\"hidden\", name=\"userName\", value=\"$user\">\n";
                echo "<input type=\"hidden\", name=\"stream\", value=\"$stream\">\n";
                echo "<input type=\"hidden\", name=\"index\", value=\"$index\">\n";
                echo "<input type=\"hidden\", name=\"byName\", value=\"$byName\">\n";
                echo "</form>\n";

                echo "<form action=\"streamList.php\", method=\"POST\">\n";
                echo "<input type=\"Submit\", name=\"selectNew\", value=\"Select Stream\">\n";
                echo "<input type=\"hidden\", name=\"userName\", value=\"$user\">\n";
                echo "</form>\n";
                echo "<hr>\n";

                echo $tag;

                if (($user) && ($stream))
                {
                    if (isset($_POST["Prev"]))
                    {
                        exec("./view.py \"$user\" \"$stream\" \"$index\" \"-p\" \"$byName\"", $message, $stat);
                    }
                    else if (isset($_POST["Next"]))
                    {
                        exec("./view.py \"$user\" \"$stream\" \"$index\" \"-n\" \"$byName\"", $message, $stat);
                    }
                    else if (isset($_POST["Check New"]))
                    {
                        exec("./view.py \"$user\" \"$stream\" \"$index\"", $message, $stat);
                    }
                    else if (isset($_POST["markAll"]))
                    {
                        exec("./view.py \"$user\" \"$stream\" \"$index\" \"-m\" \"$byName\"", $message, $stat);
                    }
                    else
                    {
                        exec("./view.py \"$user\" \"$stream\" \"$index\"", $message, $stat);
                    }

                    if ($status)
                    {
                        echo "view failed to run";
                    }
                    else
                    {
                        foreach($message as $i=>$line)
                        {
                            if ($i == 0)
                            {
                                $index = $line;
                            }
                            else
                            {
                                echo "$line\n";
                            }
                        }
                    }
                }

                echo "</textArea>\n";
                echo "<hr>\n";
                echo "<form action=\"streamViewer.php\", method=\"POST\">\n";
                echo "<input type=\"Submit\", name=\"Prev\", value=\"Prev\">\n";
                echo "<input type=\"Submit\", name=\"Next\", value=\"Next\">\n";
                echo "<input type=\"hidden\", name=\"userName\", value=\"$user\">\n";
                echo "<input type=\"hidden\", name=\"stream\", value=\"$stream\">\n";
                echo "<input type=\"hidden\", name=\"index\", value=\"$index\">\n";
                echo "<input type=\"hidden\", name=\"byName\", value=\"$byName\">\n";
                echo "</form>\n";
            }
            else
            {
                echo $tag;
            }
        }
    }
?>

</body>
</html>
