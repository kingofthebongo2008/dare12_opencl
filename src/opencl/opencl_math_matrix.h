#pragma once

#include "opencl_math_vector.h"

typedef struct _float4x4
{
    float4 r[4];    //todo: check whether it goes into the registers, since it is too big
} float4x4;

float4x4 matrix44_identity_matrix()
{
    float4x4 m;

    m.r[0] = identity_r0();
    m.r[1] = identity_r1();
    m.r[2] = identity_r2();
    m.r[3] = identity_r3();

    return m;
}

float4x4 matrix44_zero()
{
    float4x4 m;

    m.r[0] = zero();
    m.r[1] = zero();
    m.r[2] = zero();
    m.r[3] = zero();

    return m;
}

float4x4 matrix44_set( float4 v0, float4 v1, float4 v2, float4 v3 )
{
    float4x4 m;

    m.r[0] = v0;
    m.r[1] = v1;
    m.r[2] = v2;
    m.r[3] = v3;

    return m;
}

float4x4 matrix44_transpose(float4x4 m)
{
    float4x4 m1;

    float4 v1 = (float4)(m.r[0].xy, m.r[1].xy);// shuffle<x, y, x, y>(m.r[0], m.r[1]);
    float4 v2 = (float4)(m.r[2].xy, m.r[3].xy);// shuffle<x, y, x, y>(m.r[2], m.r[3]);
    float4 v3 = (float4)(m.r[0].zw, m.r[1].zw);// shuffle<z, w, z, w>(m.r[0], m.r[1]);
    float4 v4 = (float4)(m.r[2].zw, m.r[3].zw);// shuffle<z, w, z, w>(m.r[2], m.r[3]);

    m1.r[0] = (float4)(v1.xz, v2.xz);// shuffle<x, z, x, z>(v1, v2);
    m1.r[1] = (float4)(v1.yw, v2.yw);// shuffle<y, w, y, w>(v1, v2);
    m1.r[2] = (float4)(v3.xz, v4.xz);// shuffle<x, z, x, z>(v3, v4);
    m1.r[3] = (float4)(v3.yw, v4.yw);// shuffle<y, w, y, w>(v3, v4);

    return m1;
}

float4x4 matrix44_inverse(float4x4 m)
{
    float4 tmp;
    float4 det;
    float4 minor0; 
    float4 minor1; 
    float4 minor2; 
    float4 minor3;
    float4 row0;
    float4 row1;
    float4 row2;
    float4 row3;

    //------ Transposition
    float4x4 m1 = matrix44_transpose(m);

    row0 = m1.r[0];
    row1 = m1.r[1].zwxy;    // swizzle<z, w, x, y>(m1.r[1]);
    row2 = m1.r[2];
    row3 = m1.r[3].zwxy;    // swizzle<z, w, x, y>(m1.r[3]);

    //------ 
    tmp = mul( row2, row3);
    tmp = tmp.yxwz; // swizzle<y, x, w, z>(tmp); //b1

    minor0 = mul(row1, tmp);
    minor1 = mul(row0, tmp);

    tmp = tmp.zwxy;// swizzle<z, w, x, y>(tmp); //4e

    minor0 = sub(mul(row1, tmp), minor0);
    minor1 = sub(mul(row0, tmp), minor1);

    minor1 = minor1.zwxy;   //swizzle<z, w, x, y>(minor1); //4e
            
    //------ 
    tmp = mul(row1, row2);
    tmp = tmp.yxwz;         //swizzle<y, x, w, z>(tmp);

    minor0 = add( mul(row3, tmp), minor0 );
    minor3 = mul(row0, tmp);

    tmp = tmp.zwxy;// swizzle<z, w, x, y>(tmp);

    minor0 = sub(minor0, mul(row3, tmp));
    minor3 = sub(mul(row0,tmp), minor3);
    minor3 = minor3.zwxy;   // swizzle<z, w, x, y>(minor3);

    //------ 
    tmp = mul( row1.zwxy, row3);    // mul( swizzle<z,w,x,y>(row1), row3);
    tmp = tmp.yxwz;                 // swizzle<y, x, w, z>(tmp);
    row2 = row2.zwxy;               // swizzle<z, w, x, y>(row2);

    minor0 = add(mul(row2,tmp), minor0);
    minor2 = mul(row0, tmp);

    tmp = tmp.zwxy;                 // swizzle<z, w, x, y>(tmp);

    minor0 = sub(minor0, mul(row2, tmp));
    minor2 = sub(mul(row0, tmp), minor2);
    minor2 = minor2.zwxy;           // swizzle<z, w, x, y>(minor2);

    //------ 

    tmp = mul(row0, row1);
    tmp = tmp.yxwz;                 // swizzle<y, x, w, z>(tmp);

    minor2 = add(mul(row3, tmp), minor2);
    minor3 = sub(mul(row2, tmp), minor3);

    tmp = tmp.zwxy;                 // swizzle<z, w, x, y>(tmp);
    minor2 = sub( mul(row3, tmp), minor2);
    minor3 = sub(minor3, mul(row2, tmp));

    //------ 

    tmp = mul(row0, row3);
    tmp = tmp.yxwz;                 // swizzle<y, x, w, z>(tmp);

    minor1 = sub(minor1, mul(row2, tmp));
    minor2 = add(mul(row1, tmp), minor2 );

    tmp = tmp.zwxy;                 // swizzle<z, w, x, y>(tmp);
    minor1 = add( mul(row2, tmp), minor1 );
    minor2 = sub(minor2, mul(row1, tmp) );

    //------ 
    tmp = mul(row0, row2);
    tmp = tmp.yxwz;                 // swizzle<y, x, w, z>(tmp);

    minor1 = add( mul(row3, tmp), minor1);
    minor3 = sub( minor3, mul(row1, tmp));

    tmp = tmp.zwxy;                 // swizzle<z, w, x, y>(tmp);

    minor1 = sub(minor1, mul(row3, tmp));
    minor3 = add( mul(row1, tmp), minor3);
    //------ 

    det = mul(row0, minor0);
    det = add(det.zwxy, det); //add( swizzle<z,w,x,y>(det), det );
    det = add(det.yxzw, det); //add(swizzle<y,x,z,w>(det),det);
    tmp = rcp(det);

    det = sub(add(tmp, tmp), mul(det, mul(tmp, tmp)));
    det = det.xxxx;//swizzle<x,x,x,x>(det);

    float4x4 m2;
            
    m2.r[0] = mul(det, minor0);
    m2.r[1] = mul(det, minor1);
    m2.r[2] = mul(det, minor2);
    m2.r[3] = mul(det, minor3);

    return m2;

}

float4 matrix44_mul_l(float4 v, float4x4 m)
{
    float4  v1 = v.xxxx;// swizzle<x, x, x, x>(v);
    float4  v2 = v.yyyy;// swizzle<y, y, y, y>(v);
    float4  v3 = v.zzzz;// swizzle<z, z, z, z>(v);
    float4  v4 = v.wwww;// swizzle<w, w, w, w>(v);

    float4  v5 = mul(v1, m.r[0]);
    float4  v6 = mul(v2, m.r[1]);
    float4  v7 = mul(v3, m.r[2]);
    float4  v8 = mul(v4, m.r[3]);

    float4  v9 = add(v5, v6);
    float4  v10 = add(v7, v8);
    float4  v11 = add(v9, v10);

    return v11;
}

float4 matrix44_mul_r(float4x4 m, float4 v)
{
    float4x4 trans = matrix44_transpose(m);
    return matrix44_mul_l(v, trans);
}

float4x4 matrix44_mul(float4x4 m1, float4x4 m2)
{
    float4x4 m;

    m.r[0] = matrix44_mul_l(m1.r[0], m2);
    m.r[1] = matrix44_mul_l(m1.r[1], m2);
    m.r[2] = matrix44_mul_l(m1.r[2], m2);
    m.r[3] = matrix44_mul_l(m1.r[3], m2);

    return m;
}

float4x4 matrix44_add( float4x4 m1, float4x4 m2 )
{
    float4x4 m;

    m.r[0] = add(m1.r[0], m2.r[0]);
    m.r[1] = add(m1.r[1], m2.r[1]);
    m.r[2] = add(m1.r[2], m2.r[2]);
    m.r[3] = add(m1.r[3], m2.r[3]);

    return m;
}

float4x4 matrix44_sub( float4x4 m1, float4x4 m2 )
{
    float4x4 m;

    m.r[0] = sub(m1.r[0], m2.r[0]);
    m.r[1] = sub(m1.r[1], m2.r[1]);
    m.r[2] = sub(m1.r[2], m2.r[2]);
    m.r[3] = sub(m1.r[3], m2.r[3]);

    return m;
}

float4x4 matrix44_mad(float4x4 m1, float4x4 m2, float4x4 m3)
{
    float4x4 m4 = matrix44_mul(m1, m2);
    float4x4 m5 = matrix44_add(m4, m3);
    return m5;
}

