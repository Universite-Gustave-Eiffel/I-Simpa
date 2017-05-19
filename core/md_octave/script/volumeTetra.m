function [v]=volumeTetra(xn,yn,zn)

%           x21=xn(2)-xn(1);  x31=xn(3)-xn(1);  x41=xn(4)-xn(1);
%           y21=yn(2)-yn(1);  y31=yn(3)-yn(1);  y41=yn(4)-yn(1);
%           z21=zn(2)-zn(1);  z31=zn(3)-zn(1);  z41=zn(4)-zn(1);
% 
%           z43=zn(4)-zn(3);          z14=zn(1)-zn(4);
%           z32=zn(3)-zn(2);          z24=zn(2)-zn(4);
%           z42=zn(4)-zn(2);          z13=zn(1)-zn(3);
%           z34=zn(3)-zn(4);          z12=zn(1)-zn(2);
%           z23=zn(2)-zn(3);
% 
%           y43=yn(4)-yn(3);          y14=yn(1)-yn(4);
%           y32=yn(3)-yn(2);          y24=yn(2)-yn(4);
%           y42=yn(4)-yn(2);          y13=yn(1)-yn(3);
% 
%           
%            v=(x21*(y31*z41-y41*z31)-x31*(y21*z41-y41*z21)+...
%                x41*(y21*z31-y31*z21))/6.;
          
              XA=[xn(1); yn(1); zn(1)];
              XB=[xn(2); yn(2); zn(2)];
              XC=[xn(3); yn(3); zn(3)];
              XD=[xn(4); yn(4); zn(4)];
          v=dot(XD-XA,cross((XB-XA),(XC-XA)))/6;


return
