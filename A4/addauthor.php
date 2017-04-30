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

    $author = (!empty($_POST["userName"]) ? $_POST["userName"] : null);
    $stream = (!empty($_POST["stream"]) ? $_POST["stream"] : null);
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
