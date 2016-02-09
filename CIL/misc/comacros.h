/*
 * filename : comacros.h
 * author   : Takahiro Sugiyama
 * redate   : 1993/09/18
 */



#ifndef __comacros_h
#define __comacros_h



/*===========================================================================*

C $B%W%m%0%i%`$N4X?t$N=q$-J}(B:

     $B4X?t$NA0@k8@$O!"4X?t$N7?%A%'%C%/$J$I=EMW$G$9!#(B
     $B$7$+$7!"(BANSI $B$H(B K&R $B$N(B CC $B$+6&B8$9$kCf$G$3$l$r9T$J$&$N$O:$Fq$G$9!#(B
     $B$I$A$i$+$r;&$9$7$+$J$$$G$7$g$&!#(B
     $B$3$N%^%/%m$O!"(BANSI C $B$N$H$-%W%m%H%?%$%W@k8@$r9T$J$$!"(BK&R C $B$N$H$-!"(B
     $BDL>o$N@k8@$@$1$r9T$J$&$?$a$N%^%/%m$G$9!#(B
     $B$^$?!"4X?t@k8@;~$K$*$1$k!"(BANSI C $B$H(B K&R C $B$N0c$$$b5[<}$7$^$9!#(B
     $B$3$l$G!"4X?t@k8@$NE}0l2=$,$G$-$^$9!#(B

$B=q$-J}(B:

     $B4X?tA0@k8@(B:
     <type> <funcname> P<n> (( <type@1>, <argname@1> ),
			     ( <type@2>, <argname@2> ),
			     ...
			     ( <type@n>, <argname@n> ));

$B4X?t@k8@(B:
     <type> <funcname> _P<n> (( <type@1>, <argname@1> ),
			      ( <type@2>, <argname@2> ),
			      ...
			      ( <type@n>, <argname@n> ))
     {
       <code>
     }

$BNc(B:$B4X?tA0@k8@(B

     (J\(B*==========================================================*
      * optinit : $B%3%^%s%I%i%$%s$r;XDj$5$l$?%*%W%7%g%s$G2r@O$9$k(B
      *==========================================================*(J\(B
     void optinit
       P4 (( int    ,op_argc ), (J\(B* $B%*%W%7%g%s$N?t(B     *(J\(B
           ( char **,op_argv ), (J\(B* $B%*%W%7%g%s$N%j%9%H(B *(J\(B 
	   ( int    ,argc    ), (J\(B* $B0z?t$N?t(B           *(J\(B
	   ( char **,argv    )) (J\(B* $B0z?t$N%j%9%H(B       *(J\(B

      *============================*
      * optaddarg : $B0z?t$rDI2C$9$k(B
      *============================*
     void optaddarg
       P1 (( char *, arg )) (J\(B* $B0z?t(B *(J\(B

$BNc(B:$B4X?t@k8@(B

     (J\(B*==========================================================*
      * optinit : $B%3%^%s%I%i%$%s$r;XDj$5$l$?%*%W%7%g%s$G2r@O$9$k(B
      *==========================================================*(J\(B
     void optinit
       _P4 (( int    ,op_argc ), (J\(B* $B%*%W%7%g%s$N?t(B     *(J\(B
            ( char **,op_argv ), (J\(B* $B%*%W%7%g%s$N%j%9%H(B *(J\(B 
	    ( int    ,argc    ), (J\(B* $B0z?t$N?t(B           *(J\(B
	    ( char **,argv    )) (J\(B* $B0z?t$N%j%9%H(B       *(J\(B
     {
       ...;
     }
 
     (J\(B*============================*
      * optaddarg : $B0z?t$rDI2C$9$k(B
      *============================*(J\(B
     void optaddarg
       _P1 (( char *, arg )) (J\(B* $B0z?t(B *(J\(B
     {
       ...;
     }

     $B4X?t$N0z?t$G$N%3%a%s%H$O!"$o$+$j$d$9$$8@MU$G=q$-$^$7$g$&!#(B
     '=' $B$G>e$H2<$r64$s$?$H$-$O!"%0%m!<%P%k$K;2>H$G$-$k$b$N$H$7$F!"(B
     '-' $B$G>e$H2<$r64$s$?$H$-$O!"%m!<%+%k$K;2>H$G$-$k$b$N$H$7$F!"(B
     $B7h$a$F$*$/$H$$$$$G$7$g$&!#%3%a%s%H$N=q$-J}$r5,@)$9$k$o$1$G$O(B
     $B$"$j$^$;$s$,!"E}0l$5$l$F$$$l$P!":.Mp$O$J$/$J$k$G$7$g$&!#(B

     $B$^$?!"$5$i$K>\$7$$;H$$J}$N@bL@$,I,MW$H;W$o$l$k$b$N$O!"$3$l<+?H$N$h$&$K!"(B
     $B=q$/$HNI$$$G$7$g$&!#(B

     $B%X%C%@%U%!%$%k$=$N$b$N$,%j%U%!%l%s%9%^%K%e%"%k$H$J$k$H$$$$$G$9$M!#(B

$BCm0U(B:
     $B4X?tA0@k8@$H4X?t@k8@$N0c$$$O!"$[$H$s$I$J$$$N$G!"(B
     $B$^$:!"4X?tA0@k8@$r:n$C$F$+$i!"$=$l$r%3%T!<$7$F!"(B
     P<n> $B$r(B _P<n> $B$KJQ99$9$l$P!"$=$N$^$^;H$($^$9!#(B
     $BFsEY!"F1$8$3$H$r=q$/$N$O!"8zN($,0-$$$@$1$G$J$/!"(B
     $B;W$o$L%P%0$r:n$k$3$H$K$J$j$^$9$h!#(B

$BCm0U(B:
     $B%^%/%m$O!"0z?t$,!"(B0 $B!A(B 15 $B$^$G$7$+MQ0U$7$F$^$;$s!#(B
     $B$3$l0J>e$?$/$5$s$N0z?t$r;XDj$9$k$h$&$J4X?t$O:n$i$J$$J}$,$$$$$G$7$g$&!#(B
     $B$^$?!"2DJQ0z?t$K4X$7$F$N%^%/%m$OMQ0U$7$F$^$;$s$,!"(B
     $B2DJQ0z?t$N=q$-J}$N>\:Y$rCN$C$F$$$l$P!"=q$/$N$OFq$7$/$J$$$G$7$g$&!#(B
     $B;29M$H$7$F$O!"(B<varargs.h> $B$r8+$k$+!"(Bman varargs $B$r8+$^$7$g$&!#(B
 *===========================================================================*/




/*=======================*
 * Number : $BG[Ns$NMWAG?t(B
 *=======================*/
#define Number(arr) (sizeof(arr)/sizeof(arr[0]))



/*=========================================*
 * P : $B@N$N%W%m%H%?%$%W@k8@MQ$N%^%/%m(B
 *     $B?7$7$/:n$k?M$O$3$l$r;H$o$J$$$h$&$K!#(B
 *=========================================*/
#ifdef __STDC__
#define P(X) X
#else
#define P(X) ()
#endif //__STDC__



/*====================================================*
 * P#  : $B4X?t$NA0@k8@$GMQ$$$k%W%m%H%?%$%W@k8@MQ%^%/%m(B
 * _P# : $B4X?t$N@k8@$GMQ$$$k%W%m%H%?%$%W@k8@MQ%^%/%m(B
 *====================================================*/

#ifdef __STDC__

#define _Z(type,value) type value
#define _Y(type,value)
#define _P0(X) (X)

#define P0(X) _P0(X);
#define P1(a) _P1(a);
#define P2(a,b) _P2(a,b);
#define P3(a,b,c) _P3(a,b,c);
#define P4(a,b,c,d) _P4(a,b,c,d);
#define P5(a,b,c,d,e) _P5(a,b,c,d,e);
#define P6(a,b,c,d,e,f) _P6(a,b,c,d,e,f);
#define P7(a,b,c,d,e,f,g) _P7(a,b,c,d,e,f,g);
#define P8(a,b,c,d,e,f,g,h) _P8(a,b,c,d,e,f,g,h);
#define P9(a,b,c,d,e,f,g,h,i) _P9(a,b,c,d,e,f,g,h,i);
#define P10(a,b,c,d,e,f,g,h,i,j) _P10(a,b,c,d,e,f,g,h,i,j);
#define P11(a,b,c,d,e,f,g,h,i,j,k) _P11(a,b,c,d,e,f,g,h,i,j,k);
#define P12(a,b,c,d,e,f,g,h,i,j,k,l) _P12(a,b,c,d,e,f,g,h,i,j,k,l);
#define P13(a,b,c,d,e,f,g,h,i,j,k,l,m) _P13(a,b,c,d,e,f,g,h,i,j,k,l,m);
#define P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) _P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n);
#define P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) _P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o);

#else

#define _Z(type,value) value
#define _Y(type,value) type value;
#define _P0(X) ()

#define P0(X) ();
#define P1(a) ();
#define P2(a,b) ();
#define P3(a,b,c) ();
#define P4(a,b,c,d) ();
#define P5(a,b,c,d,e) ();
#define P6(a,b,c,d,e,f) ();
#define P7(a,b,c,d,e,f,g) ();
#define P8(a,b,c,d,e,f,g,h) ();
#define P9(a,b,c,d,e,f,g,h,i) ();
#define P10(a,b,c,d,e,f,g,h,i,j) ();
#define P11(a,b,c,d,e,f,g,h,i,j,k) ();
#define P12(a,b,c,d,e,f,g,h,i,j,k,l) ();
#define P13(a,b,c,d,e,f,g,h,i,j,k,l,m) ();
#define P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) ();
#define P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) ();

#endif /* __STDC__ */



#define _P1(a)\
  (_Z a)\
  _Y a
#define _P2(a,b)\
  (_Z a,_Z b)\
  _Y a _Y b
#define _P3(a,b,c)\
  (_Z a,_Z b,_Z c)\
  _Y a _Y b _Y c
#define _P4(a,b,c,d)\
  (_Z a,_Z b,_Z c,_Z d)\
  _Y a _Y b _Y c _Y d
#define _P5(a,b,c,d,e)\
  (_Z a,_Z b,_Z c,_Z d,_Z e)\
  _Y a _Y b _Y c _Y d _Y e
#define _P6(a,b,c,d,e,f)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f)\
  _Y a _Y b _Y c _Y d _Y e _Y f
#define _P7(a,b,c,d,e,f,g)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g
#define _P8(a,b,c,d,e,f,g,h)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h
#define _P9(a,b,c,d,e,f,g,h,i)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i
#define _P10(a,b,c,d,e,f,g,h,i,j)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j
#define _P11(a,b,c,d,e,f,g,h,i,j,k)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k
#define _P12(a,b,c,d,e,f,g,h,i,j,k,l)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l
#define _P13(a,b,c,d,e,f,g,h,i,j,k,l,m)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m
#define _P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m,_Z n)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m _Y n
#define _P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m,_Z n,_Z o)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m _Y n _Y o



#endif //__comacros_h
