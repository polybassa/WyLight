// SIZE

#pragma library 1
/*
uns16 operator* _mult8x8( uns8 arg1, uns8 arg2);
int16 operator* _multS8x8( int8 arg1, int8 arg2);
uns16 operator* _multU16x8( uns16 arg1, uns8 arg2);
uns16 operator* _mult16x16( uns16 arg1, uns16 arg2);
uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2);
uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2);
int16 operator/ _divS16_8( int16 arg1, int8 arg2);
int16 operator/ _divS16_16( int16 arg1, int16 arg2);
uns8 operator% _remU16_8( uns16 arg1, uns8 arg2);
uns16 operator% _remU16_16( uns16 arg1, uns16 arg2);
int8 operator% _remS16_8( int16 arg1, int8 arg2);
int16 operator% _remS16_16( int16 arg1, int16 arg2);
*/

#if __CoreSet__ < 1410
 #define genAdd(r,a) W=a; btsc(Carry); W=incsz(a); r+=W
 #define genSub(r,a) W=a; btss(Carry); W=incsz(a); r-=W
 #define genAddW(r,a) W=a; btsc(Carry); W=incsz(a); W=r+W
 #define genSubW(r,a) W=a; btss(Carry); W=incsz(a); W=r-W
#else
 #define genAdd(r,a) W=a; r=addWFC(r)
 #define genSub(r,a) W=a; r=subWFB(r)
 #define genAddW(r,a) W=a; W=addWFC(r)
 #define genSubW(r,a) W=a; W=subWFB(r)
#endif


int8 operator*( int8 arg1, int8 arg2)  @

uns16 operator* _mult8x8( uns8 arg1, uns8 arg2)
{
    uns16 rval;
    char counter = sizeof(arg2)*8;
    rval.high8 = 0;
    W = arg1;
    do  {
        arg2 = rr( arg2);
        if (Carry)
            rval.high8 += W;
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    return rval;
}


int16 operator* _multS8x8( int8 arg1, int8 arg2)
{
    uns16 rval;
    char counter = sizeof(arg2)*8;
    int8 tmpArg2 = arg2;
    rval.high8 = 0;
    W = arg1;
    do  {
        tmpArg2 = rr( tmpArg2);
        if (Carry)
            rval.high8 += W;
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    W = arg2;
    if (arg1 < 0)
        rval.high8 -= W;
    W = arg1;
    if (arg2 < 0)
        rval.high8 -= W;
    return rval;
}


uns16 operator*( uns8 arg1, uns16 arg2) exchangeArgs @

uns16 operator* _multU16x8( uns16 arg1, uns8 arg2)
{
    uns16 rval;
    uns8 rvalH = 0;
    char counter = sizeof(arg1)*8;
    W = arg2;
    do  {
        arg1 = rr( arg1);
        if (Carry)
            rvalH += W;
        rvalH = rr(rvalH);
        rval = rr(rval);
        counter = decsz(counter);
    } while (1);
    return rval;
}


int16 operator*( int16 arg1, int16 arg2) @

uns16 operator* _mult16x16( uns16 arg1, uns16 arg2)
{
    uns16 rval;
    char counter = sizeof(arg1)*8;
    do  {
        Carry = 0;
        rval = rl( rval);
        arg1 = rl( arg1);
        if (Carry)
            rval += arg2;
        counter = decsz(counter);
    } while (1);
    return rval;
}



uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY_ML;
    do  {
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
       ENTRY_ML:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY_ML;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY_ML;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        Carry = 1;
       ENTRY_ML:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


int8  operator/ (int8 arg1, int8 arg2) @

int16 operator/ _divS16_8( int16 arg1, int8 arg2)
{
    uns8 rm = 0;
    char counter = 16+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT_ML:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY_ML;
    do  {
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
       ENTRY_ML:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT_ML;
    return arg1;
}


int16 operator/ _divS16_16( int16 arg1, int16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT_ML:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY_ML;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY_ML;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        Carry = 1;
       ENTRY_ML:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT_ML;
    return arg1;
}


uns8 operator% _remU16_8( uns16 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    return rm;
}


uns16 operator% _remU16_16( uns16 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
      NOSUB:
        counter = decsz(counter);
    } while (1);
    return rm;
}


int8 operator% (int8 arg1, int8 arg2) @

int8 operator% _remS16_8( int16 arg1, int8 arg2)
{
    int8 rm = 0;
    char counter = 16;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}


int16 operator% _remS16_16( int16 arg1, int16 arg2)
{
    int16 rm = 0;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
      NOSUB:
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}

#undef genAdd
#undef genSub
#undef genAddW
#undef genSubW

#pragma library 0

