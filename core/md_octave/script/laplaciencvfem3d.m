%	Construction �l�ment par �l�ment de la matrice mat 
%	Appel de linecoeftemp qui calcule les derivees de la pression
% 	et de la temperature (une constante)

function [mat]=laplaciencvfem3d(x,y,z,kne,nbel,nn)
        ia = [];        ja = [];        s = [];
         mat = sparse ( ia, ja, s, nn, nn,12 );
%            mat=zeros(nn,nn);
        k=zeros(4,1);xn=zeros(4,1);yn=xn;zn=xn;
             
            
            for  n=1:nbel
               for l=1:4
                              k(l)=kne(n,l);
              xn(l)=x(k(l));  yn(l)=y(k(l));   zn(l)=z(k(l));

 %                 ks(l)=kne(n,l);xn(l)=x(ks(l));yn(l)=y(ks(l));
              end
                [~,b,c,d,v]=line3d(xn,yn,zn);
           
%             x31=xn(3)-xn(1);    x12=xn(1)-xn(2);       x23=xn(2)-xn(3);      
%             y21=yn(2)-yn(1);    y32=yn(3)-yn(2);       y13=yn(1)-yn(3);

%	tempa=( 5.*tempo(ks(1))+5.*tempo(ks(2))+2.*tempo(ks(3)) )/12.
%*	tempb=( 2.*tempo(ks(1))+5.*tempo(ks(2))+5.*tempo(ks(3)) )/12.
%	tempc=( 5.*tempo(ks(1))+2.*tempo(ks(2))+5.*tempo(ks(3)) )/12.
% tempmoy=(tempo(ks(1))+tempo(ks(2))+tempo(ks(3)) )/3.;
% 	tempa=( 2.*tempo(ks(1))+tempo(ks(2))+tempo(ks(3)) )/4.;
% 	tempb=( tempo(ks(1))+2.*tempo(ks(2))+tempo(ks(3)) )/4.;
% 	tempc=( tempo(ks(1))+tempo(ks(2))+2.*tempo(ks(3)) )/4.;
%	tempa=tempmoy;	tempb=tempmoy;	tempc=tempmoy;
            xa=(xn(1)+xn(2))/2.;  ya=(yn(1)+yn(2))/2.;    za=(zn(1)+zn(2))/2.;
            xb=(xn(2)+xn(3))/2.;  yb=(yn(2)+yn(3))/2.;    zb=(zn(2)+zn(3))/2.;
            xc=(xn(3)+xn(1))/2.;  yc=(yn(3)+yn(1))/2.;    zc=(zn(3)+zn(1))/2.;
            xd=(xn(4)+xn(1))/2.;  yd=(yn(4)+yn(1))/2.;    zd=(zn(4)+zn(1))/2.;

            xe=(xn(4)+xn(2))/2.; ye=(yn(4)+yn(2))/2.;    ze=(zn(4)+zn(2))/2.;
            xf=(xn(4)+xn(3))/2.; yf=(yn(4)+yn(3))/2.;    zf=(zn(4)+zn(3))/2.;

            ani1=(2/3.)*((yc-ya)*(zd-za)-(zc-za)*(yd-ya));
            anj1=(2/3.)*((zc-za)*(xd-xa)-(xc-xa)*(zd-za));
            ank1=(2/3.)*((xc-xa)*(yd-ya)-(yc-ya)*(xd-xa));

            ani2=(2/3.)*((ye-ya)*(zb-za)-(ze-za)*(yb-ya));
            anj2=(2/3.)*((ze-za)*(xb-xa)-(xe-xa)*(zb-za));
            ank2=(2/3.)*((xe-xa)*(yb-ya)-(ye-ya)*(xb-xa));

            ani3=(2/3.)*((yb-yc)*(zf-zc)-(zb-zc)*(yf-yc));
            anj3=(2/3.)*((zb-zc)*(xf-xc)-(xb-xc)*(zf-zc));
            ank3=(2/3.)*((xb-xc)*(yf-yc)-(yb-yc)*(xf-xc));

            ani4=(2/3.)*((yf-yd)*(ze-zd)-(zf-zd)*(ye-yd));
            anj4=(2/3.)*((zf-zd)*(xe-xd)-(xf-xd)*(ze-zd));
            ank4=(2/3.)*((xf-xd)*(ye-yd)-(yf-yd)*(xe-xd));
   for l=1:4
            
    anm(1)=b(l)*ani1+c(l)*anj1+d(l)*ank1;
		anm(2)=b(l)*ani2+c(l)*anj2+d(l)*ank2;
		anm(3)=b(l)*ani3+c(l)*anj3+d(l)*ank3;
		anm(4)=b(l)*ani4+c(l)*anj4+d(l)*ank4;
%........
    mat(k(1),k(l))=mat(k(1),k(l))+anm(1);
    mat(k(2),k(l))=mat(k(2),k(l))+anm(2);
		mat(k(3),k(l))=mat(k(3),k(l))+anm(3);
		mat(k(4),k(l))=mat(k(4),k(l))+anm(4);
  
        
   end
   
            end

%================================================================

%================================================================
return