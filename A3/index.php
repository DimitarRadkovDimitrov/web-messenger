<html>
<body>

<?php
    exec("./parser index.wpml", $html, $status);

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
?>


</body>
</html>
