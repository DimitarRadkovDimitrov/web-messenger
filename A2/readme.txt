************************************************************************
Dimitar Dimitrov                                                 0922289
CIS2750                                                               A2
February 19, 2017
************************************************************************

Compilation
-----------
    -Compile in root directory with "make"

    -Run "chmod +x view.py"
        To ensure python file is executable

    -At this point, there should be 2 .c files, 1 .cc file and 1 .py
    file

Usage
-----
    -Run post program with executable "./post" <username>
        -Where <username> is a name that exists in a stream user file

    -Run addauthor program with executable "./addauthor" <username>

    -Run viewer program script with "./view.py" <username>
        -Where <username> is a name that exists in the stream user file
         for the streams selected

        -Use the up and down array keys to navigate through posts

        -All input must be followed by enter/return key

Known Limitations
-----------------
    -Program is suited to handle only reasonable data input
    -Multiple selected streams for addauthor program must be separated by
     only commas
    -User file must have specific formatting
