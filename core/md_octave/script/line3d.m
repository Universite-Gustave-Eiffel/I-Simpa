% les coefficients de l'interpolation
%lineaire d'une variable P pour un espace tri-dimensionnel.
%     P(x,y)=(ai+bi*x+ci*y+di*z)Pi+(aj+bj*x+cj*y+dj*z)Pj
%            +(ak+bk*x+ck*y+dk*z)Pk+(al+bl*x+cl*y+dl*z)Pl

%   ENTREE :

%  xn,yn,zn : Vecteurs des coordonnees des sommets i,j,k du triangle e.

%  SORTIE :
%   a,b,c,d : Vecteurs des coefficients de l'interpolation pour les
%                 sommets i,j,k,l du tetrahedre e.
%   v       : Volume du tetrahedre e.
%   x13,x21,x32 : Differences d'abscisses.
%   y12,y23,y31 : Differences d'ordonnees.

% *   Vecteurs des coordonnees des sommets i,j,k,l du
% *              tetrahedre e.
% *
% *   SORTIE :
% *
% *   a,b,c,d : Vecteurs des coefficients de l'interpolation pour les
% *             sommets i,j,k,l
% *
%c----$---1---------2---------3---------4---------5---------6---------7-c

function [a,b,c,d,v]=line3d(xn,yn,zn)

          x21=xn(2)-xn(1);  x31=xn(3)-xn(1);  x41=xn(4)-xn(1);
          y21=yn(2)-yn(1);  y31=yn(3)-yn(1);  y41=yn(4)-yn(1);
          z21=zn(2)-zn(1);  z31=zn(3)-zn(1);  z41=zn(4)-zn(1);

          z43=zn(4)-zn(3);          z14=zn(1)-zn(4);
          z32=zn(3)-zn(2);          z24=zn(2)-zn(4);
          z42=zn(4)-zn(2);          z13=zn(1)-zn(3);
          z34=zn(3)-zn(4);          z12=zn(1)-zn(2);
          z23=zn(2)-zn(3);

          y43=yn(4)-yn(3);          y14=yn(1)-yn(4);
          y32=yn(3)-yn(2);          y24=yn(2)-yn(4);
          y42=yn(4)-yn(2);          y13=yn(1)-yn(3);

          
          v=(x21*(y31*z41-y41*z31)-x31*(y21*z41-y41*z21)+...
              x41*(y21*z31-y31*z21))/6.;
%c----$---1---------2---------3---------4---------5---------6---------7-c

          a(1)=(xn(2)*(yn(3)*zn(4)-yn(4)*zn(3))...
      +xn(3)*(yn(4)*zn(2)-yn(2)*zn(4))...
      +xn(4)*(yn(2)*zn(3)-yn(3)*zn(2)))/(6.*v);
          a(2)=(-xn(3)*(yn(4)*zn(1)-yn(1)*zn(4))...
     -xn(4)*(yn(1)*zn(3)-yn(3)*zn(1))...
     -xn(1)*(yn(3)*zn(4)-yn(4)*zn(3)))/(6.*v);
 
          a(3)=(xn(4)*(yn(1)*zn(2)-yn(2)*zn(1))...
     +xn(1)*(yn(2)*zn(4)-yn(4)*zn(2))...
     +xn(2)*(yn(4)*zn(1)-yn(1)*zn(4)))/(6.*v);
 
          a(4)=(-xn(1)*(yn(2)*zn(3)-yn(3)*zn(2))...
     -xn(2)*(yn(3)*zn(1)-yn(1)*zn(3))...
     -xn(3)*(yn(1)*zn(2)-yn(2)*zn(1)))/(6.*v);

          b(1)=(yn(2)*z43+yn(3)*z24+yn(4)*z32)/(6.*v);
          b(2)=(-yn(3)*z14-yn(4)*z31-yn(1)*z43)/(6.*v);
          b(3)=(yn(4)*z21+yn(1)*z42+yn(2)*z14)/(6.*v);
          b(4)=(-yn(1)*z32-yn(2)*z13-yn(3)*z21)/(6.*v);

          c(1)=(xn(2)*z34+xn(3)*z42+xn(4)*z23)/(6.*v);
          c(2)=(-xn(3)*z41-xn(4)*z13-xn(1)*z34)/(6.*v);
          c(3)=(xn(4)*z12+xn(1)*z24+xn(2)*z41)/(6.*v);
          c(4)=(-xn(1)*z23-xn(2)*z31-xn(3)*z12)/(6.*v);

          d(1)=(xn(2)*y43+xn(3)*y24+xn(4)*y32)/(6.*v);
          d(2)=(-xn(3)*y14-xn(4)*y31-xn(1)*y43)/(6.*v);
          d(3)=(xn(4)*y21+xn(1)*y42+xn(2)*y14)/(6.*v);
          d(4)=(-xn(1)*y32-xn(2)*y13-xn(3)*y21)/(6.*v);


 


return
