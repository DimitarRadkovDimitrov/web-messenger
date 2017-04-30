<html>
<body>

<?php
    exec("./parser addauthor.wpml", $html, $status);

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

    $author = $_POST["userName"];
    $stream = $_POST["streamName"];
    $URL = "index.php";

    if (isset($_POST["remove"]))
    {
        exec("./addauthor -r \"$author\" \"$stream\"", $out, $stat);
        echo "<script type='text/javascript'>document.location.href='{$URL}';</script>";
    }
    if (isset($_POST["add"]))
    {
        exec("./addauthor \"$author\" \"$stream\"", $out, $stat);
        echo "<script type='text/javascript'>document.location.href='{$URL}';</script>";
    }
?>

</body>
</html>
