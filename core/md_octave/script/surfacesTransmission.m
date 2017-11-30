
function [NEW12]=surfacesTransmission(x,y,z,el2dT,pts12)

NEW12=zeros(size(pts12,1));
 
for ji=1: size(pts12,1)
 %[row,col] = find(NEWel2dT2==pts123(ji,3));
    [row,col] = find(el2dT== pts12(ji,1) );
    AireVC=0;
    row
    max(size(row)) ,pause
 for n=1: max(size(row))   
    ks=el2dT(n,:);     
              XA=[x(ks(1)); y(ks(1)); z(ks(1))];
              XB=[x(ks(2)); y(ks(2)); z(ks(2))];
              XC=[x(ks(3)); y(ks(3)); z(ks(3))];
                    
      % Calcul de l'aire du  VC sur les Triangles
     AireVC= AireVC+1/6*norm(cross((XB-XA), (XC-XA)));
    
    
 end
  NEW12(ji,1)= AireVC;
 
end
