PROGRAM test1;
VAR
    aa : char;
    bb : integer;
    cc : real;
BEGIN
    aa := 4;
    aa := 5000;
    aa := cc;
    bb := aa;
    bb := 5;
    bb := 7000;
    bb := 3.5;
    bb := cc;
    cc := aa;
    cc := bb;
    cc := 3.5;
END.
