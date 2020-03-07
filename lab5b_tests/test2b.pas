PROGRAM test5a ;
CONST
    ccc = 5;
TYPE 
    aaa = ARRAY [0..5] of integer;
VAR
    vvv : integer;

FUNCTION ccc(a : integer) : integer; 
    BEGIN
        WRITE(a);
        ccc := a;
    END;

FUNCTION vvv(a : integer) : integer; 
    BEGIN
        WRITE(a);
        vvv := a;
    END;

FUNCTION func1(a : integer) : integer; 
    BEGIN
        WRITE(a);
        func1 := a;
    END;

BEGIN
    vvv := 5;
    vvv := func1(vvv);
END.
