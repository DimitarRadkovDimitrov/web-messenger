<html>
<body>

<?php
    $arg = $_POST["userName"];

    $cmd = "./findStreams.py \"$arg\"";
    exec($cmd, $output, $status);

    if ($status)
    {
        echo "findStreams failed to run";
    }
    else
    {
        if (file_exists($fp))
        {
            $fp = fopen("./streamList.wpml", "r");
            $wpml = "";

            while (($curr = fgetc($fp)) != EOF)
            {
                if ($curr == '.')
                {
                    $curr = fgetc($fp);
                    if ($curr == 'r')
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

        $fp = fopen("./streamList.wpml", "w");
        fwrite($fp, $wpml);

        if ($output)
        {
            fwrite($fp, ".t(text=\"<center>\")\n");
            fwrite($fp, ".h(text=\"Authorized Streams\", size=1)\n");
            fwrite($fp, ".n(link=\"streamViewer.php\", label=\"\", input=\"hidden\", name=\"userName\", value=\"$arg\", required, ><br");

            foreach($output as $index=>$stream)
            {
                fwrite($fp, " , label=\"$stream\", input=\"radio\", name=\"stream\", value=\"$stream\"");
            }
            fwrite($fp, ", label=\"all\", input=\"radio\", name=\"stream\", value=\"all\"");
            fwrite($fp, ", label=\"\", input=\"Submit\", name=\"submit\", value=\"View\")\n");
            fwrite($fp, ".t(text=\"</center>\")\n");
            fwrite($fp, ".S()\n");
        }
        else
        {
            fwrite($fp, ".t(text=\"<center>\")\n");
            fwrite($fp, ".h(text=\"$arg is not subsribed to any streams\", size=1)\n");
            fwrite($fp, ".b(name=\"Back\", link=\"index.php\")");
            fwrite($fp, ".t(text=\"</center>\")\n");
            fwrite($fp, ".S()\n");
        }

        fclose($fp);

        exec("./parser streamList.wpml", $html, $status);

        if ($status)
        {
            echo "parser failed to run";
        }
        else
        {
            foreach($html as $key=>$tag)
            {
                echo $tag;
            }
        }
    }
?>

</body>
</html>
