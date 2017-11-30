figure
%colormap jet
%waf=wi_saved(:,end);
%XYZ2=XYZ(sortedndf(:,1),:);
%waf=w{N_Toct};
LpdB=10*log10(waf*rhoco2/(2e-5)^2);
 for i=1:NBLOCKS
figure
waf=w{i};
rhoco2=1.2*c0^2;    
LpdB=10*log10(waf*rhoco2/(2e-5)^2);
patch('Faces',el2di_Dof{i}(:,1:3),'Vertices',XYZ2,'FaceVertexCData',LpdB,'FaceColor','interp'); colorbar
hold on
xlabel('X'), ylabel('Y'),zlabel('Z')
titre = strcat('Energy density (dB) Block :' ,num2str(i));
title(titre)
end 
