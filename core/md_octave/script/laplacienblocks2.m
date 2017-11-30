%	Construction �l�ment par �l�ment de la matrice mat 
%	Appel de linecoeftemp qui calcule les derivees de la pression
% 	et de la temperature (une constante)

function [mat]=laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)
        ia = [];        ja = [];        s = [];
         mat = sparse ( ia, ja, s, NDOF, NDOF,15);
%            mat=zeros(nn,nn);
        k=zeros(4,1);xn=zeros(4,1);yn=xn;zn=xn;
for j=1:NBLOCKS
  for  n=1:size(Tet_Dof{j},1)
             
               for l=1:4
                             
                              k(l)=Tet_Dof{j}(n,l);
                              xn(l)=x(k(l));  yn(l)=y(k(l));   zn(l)=z(k(l));

 %                 ks(l)=el_1(n,l);xn(l)=x(ks(l));yn(l)=y(ks(l));
              end
                [~,b,c,d,v]=line3d(xn,yn,zn);
           
 
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
    mat(k(1),k(l))=mat(k(1),k(l))+anm(1)*CoeffDiff(j);
    mat(k(2),k(l))=mat(k(2),k(l))+anm(2)*CoeffDiff(j);
		mat(k(3),k(l))=mat(k(3),k(l))+anm(3)*CoeffDiff(j);
		mat(k(4),k(l))=mat(k(4),k(l))+anm(4)*CoeffDiff(j);
  
        
   end
   
end
end

%================================================================
return