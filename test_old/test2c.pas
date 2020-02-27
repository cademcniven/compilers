PROGRAM test5a ;
VAR
    aaa : integer;

FUNCTION func1(a : integer) : integer; forward;

FUNCTION func1(a : integer) : integer; 
    BEGIN
        WRITE(a);
        func1 := a;
    END;

FUNCTION func2(a : integer) : integer; 
    BEGIN
        WRITE(a);
        func1 := a;
    END;

BEGIN
    aaa := 5;
    aaa := func1(aaa);
    aaa := func2(aaa);
END.
